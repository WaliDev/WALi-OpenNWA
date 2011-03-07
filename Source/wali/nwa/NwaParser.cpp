#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <exception>

#include "wali/nwa/NWAParser.hpp"
#include "wali/nwa/NWA.hpp"
#include "wali/KeyContainer.hpp"

// The grammar:
//   There are some ambiguities that are resolved through slightly
//   ad-hoc lookahead decisions.
//
//
// nwa-description  ::= 'nwa:'? '{'? block+ '}'?
// 
// block  ::=  state-block     // lookahead = 'Q'
//          |  sigma-block     // lookahead = 's'
//          |  delta-block     // lookahead = 'd'
// 
// state-block  ::=  'Q:'   '{'?  name-list  '}'?
//                |  'Q0:'  '{'?  name-list  '}'?
//                |  'Qf:'  '{'?  name-list  '}'?
// 
// sigma-block  ::=  'sigma:'  '{'?  name-list  '}'?
// 
// delta-block  ::=  'delta_i:'  '{'?  triple-list  '}'?
//                |  'delta_c:'  '{'?  triple-list  '}'?
//                |  'delta_r:'  '{'?  quad-list    '}'?
// 
// name-list   ::=  (name  (','  name)*)?      // lookahead ',' means repeat
// triple-list ::=  (triple  (','  triple)*)?  // lookahead ',' means repeat
// quad-list   ::=  (quad  (','  quad)*)?      // lookahead ',' means repeat
// 
// triple  ::=  '('  name ','  name  ','  name  ')'
// quad  ::=  '('  name ','  name  ','  name  ','  name  ')'
// name  ::=  token  ('(' ~')'* ')')?   // lookahead '(' means take opt
//   ^^^ this is not very accurate right now
// 
// token ::=  ~(' ' | ',(')+
//   with balenced ( )


////////////////////////////////////////////////////////////////////////////////
/// DISCARDWS
///
/// Discards any whitespace (see std::isspace) that appears at the start of 'is'.
static
void
discardws(std::istream & is)
{
    while (std::isspace(is.peek())) {
        is.get();
    }
}

static
void
test_discardws()
{
    std::stringstream ss("  helllo");

    assert(ss.peek() == ' '); // mostly just testing .peek does what I think
    discardws(ss);
    assert(ss.peek() == 'h'); // make sure we discard up to the h
    discardws(ss);
    assert(ss.peek() == 'h'); // make sure we discard nothing if there's no WS
}


////////////////////////////////////////////////////////////////////////////////
/// READ_LIT
///
/// Read 'lit' and trailing WS from the stream 'is' (checking this assumption)
///
/// If 'lit' does not start the contents of 'is', then throws a
/// CharactersDifferException. If there are fewer than |lit| characters
/// in the stream, throw a StreamTooShortException.
///
/// For uniformity, return the literal that's read.

struct CharactersDifferException : std::exception {
    std::string lit;
    int differing_pos;
    int read_char;
    const char * message;

    CharactersDifferException(std::string const & l, int p, int c)
        : lit(l)
        , differing_pos(p)
        , read_char(c)
    {
        assert(c!=-1);
        message = err_msg();
    }

    const char * what() const throw()
    {
        return message;
    }

    const char * err_msg() const
    {
        try {
            std::stringstream ss;
            ss << "CharactersDifferException when reading literal [" << lit << "]\n";
            ss << "               Read " << (char)read_char
               << " instead of " << lit[differing_pos]
               << " at position " << differing_pos << "\n";

            std::string s = ss.str();
            char * pm = new char[s.size()];
            
            std::copy(s.begin(), s.end(), pm);
            return pm;
        }
        catch (std::exception const & e) {
            return "[CharactersDifferException: cannot allocate memory]\n";
        }
    }

    ~CharactersDifferException() throw()
    {
        if (message[0] != '[') {
            // We allocated this in err_msg
            delete message;
        }
    }
};

struct StreamTooShortException   : std::exception {};

static
std::string
read_lit(std::istream & is, std::string const & lit)
{
    // Read |lit| characters
    for (size_t i=0; i<lit.size(); ++i) {
        char c = is.get();

        if (!is.good()) {
            // Probably got to EOF
            throw StreamTooShortException();
        }
        if (c != lit[i]) {
            throw CharactersDifferException(lit, i, c);
        }
    }

    discardws(is);
    return lit;
}

static
void
test_read_lit()
{
    std::stringstream empty("");
    std::stringstream short_("he");
    std::stringstream exact("hello");
    std::stringstream long_("hello world");
    std::stringstream differ("goodbye world");

    std::string query = "hello";

    // Make sure if we're too short we get an exception
    bool exn = false;
    try {
        read_lit(empty, query);
    }
    catch(StreamTooShortException const & e) {
        exn = true;
    }
    assert(exn);

    exn = false;
    try {
        read_lit(short_, query);
    }
    catch(StreamTooShortException const & e) {
        exn = true;
    }
    assert(exn);

    // Make sure if we start with the query we get it right
    assert(read_lit(exact, query) == query);
    assert(read_lit(long_, query) == query);
    assert(long_.peek() == 'w'); // did it skip over WS?

    // Make sure if we start with stuff other than the query we get an
    // exception
    exn = false;
    try {
        read_lit(differ, query);
    }
    catch(CharactersDifferException const & e) {
        exn = true;
    }
    assert(exn);
}



////////////////////////////////////////////////////////////////////////////////
/// READ_TOKEN and READ_NAME
///
/// token ::=  ~(' ' | ',' | '(' | '}' | ')')+
/// name  ::=  token  ('(' ~')'* ')')?   // lookahead '(' means take opt
///
/// Both return the actual identifier as a string. In other words, in the 'name'
/// production, the optional (blahblah) stuff is ignored. Also eats trailing WS.
///
/// TODO: fix comment to take into account matched parens and different paren
/// types -Evan 3/4/11

static
bool
is_lparen(int c)
{
    return c == '(' || c == '{';
}

static
bool
is_rparen(int c)
{
    return c == ')' || c == '}';
}

static
std::string
read_token(std::istream & is)
{
    int paren_count = 0;
    discardws(is);
    std::string ret;
    while (true) {
        int c = is.peek();

        // If we're at the end of the stream there's nothing we can do
        if (c == -1) {
            break;
        }

        if (paren_count > 0) {
            // Inside parens the only special characters are those that
            // control the nesting depth.
            if (is_lparen(c)) {
                ++paren_count;
            }
            else if (is_rparen(c)) {
                --paren_count;
            }
        }
        else {
            // Outside parens, we have lots of special characters that can
            // terminate the loop. Whitespace can obviously terminate it. ','
            // terminate it because that means it's time for another item in
            // a list. '}' means that we're at the end of a list, and ')'
            // means we're at the end of a tuple. Finally, '(' takes us into
            // parens.
            if (std::isspace(c) || c == ',' || c == '}' || c == ')') {
                break;
            }
            if (is_lparen(c)) {
                ++paren_count;
            }
        }
        
        ret += is.get();
    }
    discardws(is);
    return ret;
}

static
std::string
read_name(std::istream & is)
{
    std::string token = read_token(is);
    if (is.peek() == '(') {
        while (is.get() != ')')
            ;
    }
    discardws(is);
    return token;
}

static
void
test_read_name()
{
    // Test several that look like they are a non-final name in a list
    std::stringstream ss1(" hello, world");
    std::stringstream ss2(" hello , world");
    std::stringstream ss3(" hello (ignore), world");
    std::stringstream ss4(" hello (ignore) , world");
    std::stringstream ss5(" hello (<.,h.439[0oetnh>), world");
    std::stringstream ss6(" hello (ighonetohaoey489) , world");

    // In every one of those, "hello" should be considered the key
    assert(read_name(ss1) == "hello");
    assert(read_name(ss2) == "hello");
    assert(read_name(ss3) == "hello");
    assert(read_name(ss4) == "hello");
    assert(read_name(ss5) == "hello");
    assert(read_name(ss6) == "hello");

    // And it should always remove up to but not including the comma
    assert(ss1.peek() == ',');
    assert(ss2.peek() == ',');
    assert(ss3.peek() == ',');
    assert(ss4.peek() == ',');
    assert(ss5.peek() == ',');
    assert(ss6.peek() == ',');

    // Test several that look like a final-one in a list
    std::stringstream ss7(" hello)");
    std::stringstream ss8(" hello (o,nthent9r34<>))");
    std::stringstream ss9(" hello (o,nthent9r34<>) )");
    std::stringstream ss10(" hello}");
    std::stringstream ss11(" hello (o,nthent9r34<>)}");
    std::stringstream ss12(" hello (o,nthent9r34<>)\n}");

    assert(read_name(ss7) == "hello");
    assert(read_name(ss8) == "hello");
    assert(read_name(ss9) == "hello");
    assert(read_name(ss10) == "hello");
    assert(read_name(ss11) == "hello");
    assert(read_name(ss12) == "hello");

    // Make sure it stops at the appropriate delimiter
    assert(ss7.peek() == ')');
    assert(ss8.peek() == ')');
    assert(ss9.peek() == ')');
    assert(ss10.peek() == '}');
    assert(ss11.peek() == '}');
    assert(ss12.peek() == '}');

    // Now test a hard case: parens as the key
    std::stringstream ss13("(key1,2) bar");
    std::stringstream ss14("{key1,2} bar");
    std::stringstream ss15("(<key1,2>) bar");
    std::stringstream ss16("({key1,2}) bar");
    std::stringstream ss17("({key1,2)) bar");
    
    assert(read_name(ss13) == "(key1,2)");
    assert(read_name(ss14) == "{key1,2}");
    assert(read_name(ss15) == "(<key1,2>)");
    assert(read_name(ss16) == "({key1,2})");
    assert(read_name(ss17) == "({key1,2))");
}



////////////////////////////////////////////////////////////////////////////////
/// READ_TRIPLE and READ_QUAD
///
/// triple  ::=  '('  name ','  name  ','  name  ')'
/// quad    ::=  '('  name ','  name  ','  name  ','  name  ')'
///
/// Returns either a KeyTriple or a KeyQuad as appropriate; these are triples
/// and quads of wali::Keys, not strings. Eats trailing WS.


typedef wali::Triple<wali::Key, wali::Key, wali::Key> KeyTriple;
typedef wali::Quad<wali::Key, wali::Key, wali::Key, wali::Key> KeyQuad;
using wali::getKey;
using wali::make_triple;
using wali::make_quad;
using wali::nwa::NWARefPtr;

static
KeyTriple
read_triple(std::istream & is)
{
    read_lit(is, "(");
    std::string fn = read_name(is);
    read_lit(is, ",");
    std::string sn = read_name(is);
    read_lit(is, ",");
    std::string tn = read_name(is);
    read_lit(is, ")");

    return make_triple(getKey(fn), getKey(sn), getKey(tn));
}

static
KeyQuad
read_quad(std::istream & is)
{
    read_lit(is, "(");
    std::string fn = read_name(is);
    read_lit(is, ",");
    std::string sn = read_name(is);
    read_lit(is, ",");
    std::string tn = read_name(is);
    read_lit(is, ",");
    std::string ln = read_name(is);
    read_lit(is, ")");

    return make_quad(getKey(fn), getKey(sn), getKey(tn), getKey(ln));
}


static
void
test_read_triple_quad()
{
    std::stringstream ss1("(a, b, c)");
    std::stringstream ss2("(a, b, c)z");
    std::stringstream ss3("(a,b, c)(d, e, f)");

    KeyTriple ans3(4, 3, 2);
    
    assert(read_triple(ss1) == ans3);
    assert(read_triple(ss2) == ans3);
    assert(read_triple(ss3) == ans3);
    
    std::stringstream ss4("(a (antohe), b (tnoehn), c (otnhento))z");
    std::stringstream ss5("(a (jab3,9030), b,\nc)\n}");

    assert(read_triple(ss4) == ans3);
    assert(read_triple(ss5) == ans3);
    
    std::stringstream ss6("(a, b, c, d)");
    std::stringstream ss7("(a, b, c, d)z");
    std::stringstream ss8("(a,b, c, d)(h, e, f)");

    KeyQuad ans4(4, 3, 2, 5);

    assert(read_quad(ss6) == ans4);
    assert(read_quad(ss7) == ans4);
    assert(read_quad(ss8) == ans4);
        
    std::stringstream ss9("(a (antohe), b (tnoehn), c (otnhento), d (otnheont349))z");
    std::stringstream ss10("(a (jab3,9030), b,\nc, d)\n}");

    assert(read_quad(ss9)  == ans4);
    assert(read_quad(ss10) == ans4);
}



////////////////////////////////////////////////////////////////////////////////
/// READ_LIST
///
/// This is a generic method for reading something of the form
///     ( NONTERM (',' NONTERM)* )?
/// (You can think of it as a parser combinator.) For each instaniation, a
/// lookahead of '}' or end-of-file at the beginning means to skip the whole
/// thing. When deciding whether to take another iteration of the "loop", a
/// lookahead of ',' means "yes" and a lookahead of anything else means no.
///
/// The parser calls 'unitParser' for each item in the list; the unitParser
/// should return a 'UnitType'. (For instance, to read a list of names, pass
/// 'std::string' for 'UnitType' and 'read_name' for 'unitParser'.) It then
/// returns a vector of all semantic objects read during the list.
///
/// We test a couple instantiations below; essentailly he 'read_name_list',
/// 'read_triple_list', and 'read_quad_list' functions below are just named
/// instantiations of this with 'unitParser' bound to the appropriate thing.
///
/// [There /may/ be a way to make this infer the 'UnitType' from the return
/// type of 'unitParser', but I don't think that's possible until the
/// alternate function declaration syntax in C++0x.]

template<typename UnitType>
std::vector<UnitType>
read_list(std::istream & is,  UnitType (*unitParser)(std::istream & is))
{
    std::vector<UnitType> ret;
    if (is.peek() == '}' || is.peek() == -1) return ret;
    
    while(true) {
        // Parse the current NONTERM
        ret.push_back(unitParser(is));

        if (is.peek() == ',') {
            // There's another NONTERM, so eat the comma to get to it
            read_lit(is, ",");
        }
        else {
            break;
        }
    }

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// READ_NAME_LIST, READ_TRIPLE_LIST, READ_QUAD_LIST
///
/// name-list   ::=  (name  (','  name)*)?
/// triple-list ::=  (triple  (','  triple)*)?
/// quad-list   ::=  (quad  (','  quad)*)?
///
/// Returns a vector of strings (for name-list), KeyTriples, and KeyQuads as
/// appropriate.

static
std::vector<std::string>
read_name_list(std::istream & is)
{
    return read_list<std::string>(is, read_name);
}

static
std::vector<KeyTriple>
read_triple_list(std::istream & is)
{
    return read_list<KeyTriple>(is, read_triple);
}

static
std::vector<KeyQuad>
read_quad_list(std::istream & is)
{
    return read_list<KeyQuad>(is, read_quad);
}

static
void
test_read_lists()
{
    // These should return a zero list for all three kinds of lists.  When we
    // do the corresponding asserts below, make sure that they (really, ss2)
    // is unchanged
    std::stringstream ss1("");
    std::stringstream ss2("}");

    assert(read_name_list(ss1).size() == 0);
    assert(read_name_list(ss2).size() == 0);
    ss1.str(""); // reset ss1 because C++ is dumb and 'peek()' modifes goodbit
    assert(ss2.peek() == '}');
    assert(read_triple_list(ss1).size() == 0);
    assert(read_triple_list(ss2).size() == 0);
    ss1.str("");
    assert(ss2.peek() == '}');
    assert(read_quad_list(ss1).size() == 0);
    assert(read_quad_list(ss2).size() == 0);
    ss1.str("");
    assert(ss2.peek() == '}');

    // Test read_name_list. We only test the sizes on the first four tests,
    // then test all the elements on the later ones.
    std::stringstream ss3("name1");
    std::stringstream ss4("name1 }");
    std::stringstream ss5("name1, name2");
    std::stringstream ss6("name1, name2 }");
    std::stringstream ss7("name1, name2, name3, name4");
    std::stringstream ss8("name1, name2, name3, name4 }");

    assert(read_name_list(ss3).size() == 1);
    assert(read_name_list(ss4).size() == 1);
    assert(read_name_list(ss5).size() == 2);
    assert(read_name_list(ss6).size() == 2);
    std::vector<std::string> v7 = read_name_list(ss7);
    assert(v7.size() == 4);
    assert(v7[0] == "name1");
    assert(v7[1] == "name2");
    assert(v7[2] == "name3");
    assert(v7[3] == "name4");
    std::vector<std::string> v8 = read_name_list(ss8);
    assert(v8.size() == 4);
    assert(v8[0] == "name1");
    assert(v8[1] == "name2");
    assert(v8[2] == "name3");
    assert(v8[3] == "name4");

    // (I skip ss9 for easy of typing)

    // Test read_triple_list. We only test a couple of these here, and only
    // sizes; other problems should have been shaken out by now.
    std::stringstream ss10("(a,b, c)");
    std::stringstream ss11("(a,b, c) }");
    std::stringstream ss12("(a,b, c), (d, e, f)");
    std::stringstream ss13("(a,b, c), (d, e, f), (g, h, i) }");

    assert(read_triple_list(ss10).size() == 1);
    assert(read_triple_list(ss11).size() == 1);
    assert(read_triple_list(ss12).size() == 2);
    assert(read_triple_list(ss13).size() == 3);
    
    // Test read_quad_list. Again fairly light on purpose
    std::stringstream ss14("(a,b, c,d)");
    std::stringstream ss15("(a,b, c,d) }");
    std::stringstream ss16("(a,b,c,d),(d,e,f,g), (oth, 97go, n.hy429,<0932g>)");
    std::stringstream ss17("(a,b, c,g), (d, e, f,gc) }");

    assert(read_quad_list(ss14).size() == 1);
    assert(read_quad_list(ss15).size() == 1);
    assert(read_quad_list(ss16).size() == 3);
    assert(read_quad_list(ss17).size() == 2);
}



////////////////////////////////////////////////////////////////////////////////
/// READ_SIGMA_BLOCK, READ_STATE_BLOCK, READ_DELTA_BLOCK
///
/// state-block  ::=  'Q:'   '{'?  name-list  '}'?
///                |  'Q0:'  '{'?  name-list  '}'?
///                |  'Qf:'  '{'?  name-list  '}'?
/// 
/// sigma-block  ::=  'sigma:'  '{'?  name-list  '}'?
/// 
/// delta-block  ::=  'delta_i:'  '{'?  triple-list  '}'?
///                |  'delta_c:'  '{'?  triple-list  '}'?
///                |  'delta_r:'  '{'?  quad-list    '}'?
///
/// These functions read the given block. This is where the actual semantic
/// action to add things to an NWA are carried out. (Unfortunately this
/// aspect is not unit tested at the moment, but it's relatively simple...)
///
/// read_sigma_block is the easiest because it doesn't have to make any
/// decisionns. read_state_block and read_delta_block read the common prefix
/// of the rules ("Q" and "delta_" resp.) since they are LL(k) but not LL(1),
/// then branch to the appropriate case.
///
/// Since we don't test the semantic action of adding to the NWA, we only
/// test the recognizing ability of these. I'd need a mock NWA to test the
/// semantic action.
//
// THIS IS INCORRECT:
//
// The closing } is needed after each name-list because there's no way (at
// least without looking far ahead in an annoying-to-implement way AND
// assuming things about, e.g., where ':' can appear) to tell a name apart
// from, say, a 'delta_i:' that follows.
//
// However, the } is optional after the triple-list and quad-list

static
void
read_sigma_block(std::istream & is, NWARefPtr nwa)
{
    // Read the 'Sigma:' block header, then the optional {
    read_lit(is, "Sigma:");
    if(is.peek() == '{') {
        read_lit(is, "{");
    }

    std::vector<std::string> sigmas = read_name_list(is);

    // Now insert those things into the NWA
    for (size_t i=0; i<sigmas.size(); ++i) {
        nwa->addSymbol(getKey(sigmas[i]));
    }

    // And eat the optional }
    if(is.peek() == '}') {
        read_lit(is, "}");
    }
}


static
void
read_state_block(std::istream & is, NWARefPtr nwa)
{
    // Figure out if it's "Q:", "Q0:", or "Qf:". Read over the block
    // intro. Set the initial/final variables appropriately.
    bool initial = false, final = false;
    read_lit(is, "Q");
    if(is.peek() == ':') {
        read_lit(is, ":");
    }
    else if(is.peek() == '0') {
        read_lit(is, "0:");
        initial = true;
    }
    else {
        read_lit(is, "f:");
        final = true;
    }

    // Skip over the optional { if it's present.
    if(is.peek() == '{') {
        read_lit(is, "{");
    }

    // Actually read the list of states, then add them to the NWA.
    std::vector<std::string> states = read_name_list(is);

    // Now insert those things into the NWA
    for (size_t i=0; i<states.size(); ++i) {
        wali::Key state = wali::getKey(states[i]);
        if (initial) {
            nwa->addInitialState(state);
        }
        else if (final) {
            nwa->addFinalState(state);
        }
        else {
            nwa->addState(state);
        }
    }
    

    // Skip over the optional } if it's present.
    if(is.peek() == '}') {
        read_lit(is, "}");
    }
}


static
void
read_delta_block(std::istream & is, NWARefPtr nwa)
{
    // Figure out if it's "delta_i", "delta_c", or "delta_r". Read over the
    // block intro. Set the call/return_ variable appropriately.
    bool call = false, return_ = false;
    read_lit(is, "Delta_");
    if(is.peek() == 'i') {
        read_lit(is, "i:");
    }
    else if(is.peek() == 'c') {
        read_lit(is, "c:");
        call = true;
    }
    else {
        read_lit(is, "r:");
        return_ = true;
    }

    // Skip the optional { if it's present
    if(is.peek() == '{') {
        read_lit(is, "{");
    }

    // Now read the actual transitions and add them to the NWA. Whether they
    // are triples or quads depends on whether we're reading return
    // transitions or not.
    if(!return_) {
        std::vector<KeyTriple> triples = read_triple_list(is);
        for (size_t i=0; i<triples.size(); ++i) {
            if (call) {
                nwa->addCallTrans(triples[i]);
            }
            else {
                nwa->addInternalTrans(triples[i]);
            }
        }
    }
    else {
        std::vector<KeyQuad> quads = read_quad_list(is);
        for (size_t i=0; i<quads.size(); ++i) {
            nwa->addReturnTrans(quads[i]);
        }
    }

    // Skip the optional } if it's present
    if(is.peek() == '}') {
        read_lit(is, "}");
    }
}


static
void
test_read_blocks()
{
    NWARefPtr nwa = new wali::nwa::NWA(getKey("ha ha"));
    
    // These tests call read_*_block (implicitly making sure there are no
    // exceptions or assertions), then make sure they read everything they
    // should. For the first few tests, that means reading to the
    // end-of-string; later it means reading until the Z.
    std::stringstream ss1("Q: { }");
    std::stringstream ss2("Q: ");
    std::stringstream ss3("Q: { a }");
    std::stringstream ss4("Q: { a, b }");
    std::stringstream ss5("Q: a, b ");

    read_state_block(ss1, nwa);
    read_state_block(ss2, nwa);
    read_state_block(ss3, nwa);
    read_state_block(ss4, nwa);
    read_state_block(ss5, nwa);

    assert(ss1.peek() == -1);
    assert(ss2.peek() == -1);
    assert(ss3.peek() == -1);
    assert(ss4.peek() == -1);
    assert(ss5.peek() == -1);
    
    std::stringstream ss6("Q: a, b Z");
    std::stringstream ss7("Qf: a, b Z");
    std::stringstream ss8("Q0: a, b Z");
    std::stringstream ss9("Sigma: a Z");

    read_state_block(ss6, nwa);
    read_state_block(ss7, nwa);
    read_state_block(ss8, nwa);
    read_sigma_block(ss9, nwa);

    assert(ss6.peek() == 'Z');
    assert(ss7.peek() == 'Z');
    assert(ss8.peek() == 'Z');
    assert(ss9.peek() == 'Z');

    std::stringstream ss10("Delta_i: (a, b, c), (a, b, c) Z");
    std::stringstream ss11("Delta_c: (a, b, c), (a, b, c) Z");
    std::stringstream ss12("Delta_r: (a, b, c, d), (a, b, c, d) Z");

    read_delta_block(ss10, nwa);
    read_delta_block(ss11, nwa);
    read_delta_block(ss12, nwa);

    assert(ss10.peek() == 'Z');
    assert(ss11.peek() == 'Z');
    assert(ss12.peek() == 'Z');
}


////////////////////////////////////////////////////////////////////////////////
/// READ_BLOCK
///
/// block  ::=  state-block     // lookahead = 'Q'
///          |  sigma-block     // lookahead = 's'
///          |  delta-block     // lookahead = 'd'
///
/// Reads a block (any kind) and as a semantic action adds the appropriate
/// state/letter/transition to the NWA.

static
void
read_block(std::istream & is, NWARefPtr nwa)
{
    switch (is.peek()) {
      case 'Q': read_state_block(is, nwa); break;
      case 'S': read_sigma_block(is, nwa); break;
      case 'D': read_delta_block(is, nwa); break;
      default: {
        std::cerr << "Lookahead char: " << (char)is.peek() << "\nNext lines:\n";
        std::string s;
        std::getline(is, s);
        std::cerr << "  " << s << "\n";
        std::getline(is, s);
        std::cerr << "  " << s << "\n";
        assert(false);
      }
    }
}


////////////////////////////////////////////////////////////////////////////////
/// READ_NWA
///
/// nwa-description  ::= 'nwa:'? '{'? block+ '}'?
///
/// Reads in an nwa-description

namespace wali {
    namespace nwa {

        NWARefPtr
        read_nwa(std::istream & is)
        {
            // Come up with a stuck state
            static int gen_number = 0;
            std::stringstream ss;
            ss << "[stuck-parse-" << gen_number << "]";
            Key stuck = getKey(ss.str());

            // Create the NWA
            NWARefPtr nwa = new NWA(stuck);

            // Skip over a bunch of stuff: opening whitespace, the optional
            // 'nwa:', and the optional '{'
            discardws(is);
            if (is.peek() == 'n') {
                read_lit(is, "nwa:");
            }
            if (is.peek() == '{') {
                read_lit(is, "{");
            }

            // Now read in the actual NWA, until we reach the end of that spec
            while(is.peek() != -1 && is.peek() != '}') {
                read_block(is, nwa);
            }

            // Skip over the } if that's what ended us
            if (is.peek() == '}') {
                read_lit(is, "}");
            }

            return nwa;
        }

        
        static
        void
        test_read_nwa()
        {
            std::string mattsNwa =
                "Q: {" "\n"
                "  stuck (=2)," "\n"
                "  (key#6,3) (=16)," "\n"
                "  (key#6,4) (=17)," "\n"
                "  (key#7,3) (=18)," "\n"
                "  (key#4,3) (=19)," "\n"
                "  (key#3,3) (=20)," "\n"
                "  (key#5,3) (=21)," "\n"
                "  (key#9,3) (=22)," "\n"
                "  (key#5,4) (=23)," "\n"
                "  (key#9,4) (=24)," "\n"
                "  (key#10,3) (=25)," "\n"
                "  (key#10,4) (=26)}" "\n"
                "Q0: {" "\n"
                "  (key#6,3) (=16)," "\n"
                "  (key#6,4) (=17)}" "\n"
                "Qf: {" "\n"
                "  (key#5,4) (=23)," "\n"
                "  (key#9,4) (=24)," "\n"
                "  (key#10,4) (=26)}" "\n"
                "Sigma: {" "\n"
                "  <start;0;expression>," "\n"
                "  <start;1;body;0>," "\n"
                "  <start;2;expression;1>," "\n"
                "  <{#fun634}>," "\n"
                "  <start;2;expression>}" "\n"
                "Delta_c: {" "\n"
                "  ((key#4,3) (=19) ,<{#fun634}>, (key#3,3) (=20) )" "\n"
                "}" "\n"
                "Delta_i:  {" "\n"
                "  ((key#6,3) (=16) ,<start;0;expression>, (key#7,3) (=18) )," "\n"
                "  ((key#6,4) (=17) ,<start;0;expression>, (key#7,3) (=18) )," "\n"
                "  ((key#7,3) (=18) ,<start;2;expression;1>, (key#4,3) (=19) )," "\n"
                "  ((key#9,3) (=22) ,<start;2;expression>, (key#10,3) (=25) )," "\n"
                "  ((key#9,3) (=22) ,<start;2;expression>, (key#10,4) (=26) )," "\n"
                "  ((key#9,4) (=24) ,<start;2;expression>, (key#10,3) (=25) )," "\n"
                "  ((key#9,4) (=24) ,<start;2;expression>, (key#10,4) (=26) )" "\n"
                "}" "\n"
                "Delta_r: {" "\n"
                "  ((key#3,3) (=20) , (key#4,3) (=19) ,<start;1;body;0>, (key#5,3) (=21) )," "\n"
                "  ((key#3,3) (=20) , (key#4,3) (=19) ,<start;1;body;0>, (key#9,3) (=22) )," "\n"
                "  ((key#3,3) (=20) , (key#4,3) (=19) ,<start;1;body;0>, (key#5,4) (=23) )," "\n"
                "  ((key#3,3) (=20) , (key#4,3) (=19) ,<start;1;body;0>, (key#9,4) (=24) )" "\n"
                "}" "\n";

            std::stringstream ss1(mattsNwa);
            std::stringstream ss2("nwa: " + mattsNwa);
            std::stringstream ss3("nwa: { " + mattsNwa + " } nwa: " + mattsNwa);

            read_nwa(ss1);
            read_nwa(ss2);
            read_nwa(ss3);
            read_nwa(ss3);

            assert(ss1.peek() == -1);
            assert(ss2.peek() == -1);
            assert(ss3.peek() == -1);

    
            std::string duplicates =
                "Q: { a }" "\n"
                "Q: b" "\n"
                "Delta_i: (a, b (c), c)" "\n"
                "Delta_r: (a, b, c, d)" "\n"
                "Delta_c: {}" "\n"
                "Delta_i: (a, b, d)" "\n";

            std::stringstream ss4(duplicates);
            read_nwa(ss4);
            assert(ss4.peek() == -1);
        }


        void
        test_all()
        {
            test_discardws();
            test_read_lit();
            test_read_name();
            test_read_triple_quad();
            test_read_lists();
            test_read_blocks();
            test_read_nwa();
        }
    } // namespace nwa
} // namespace wali
