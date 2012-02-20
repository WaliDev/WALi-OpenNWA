#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <cctype>

#include <boost/shared_ptr.hpp>

#include "opennwa/NwaParser.hpp"
#include "opennwa/Nwa.hpp"
#include "wali/KeyContainer.hpp"

// The grammar:
//   There are some ambiguities that are resolved through slightly
//   ad-hoc lookahead decisions.
//
//
// nwa-description  ::= ('nwa' name? ':'?)? '{'? block+ '}'?
//        // braces are required if 'nwa' is present and name is not
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

namespace opennwa { namespace parser { namespace details {

////////////////////////////////////////////////////////////////////////////////
/// DISCARDWS
///
/// Discards any whitespace (see std::isspace) that appears at the start of 'is'.
      int lineno;

      void
      discardws(std::istream & is)
      {
        int c;
        while (std::isspace(c = is.peek())) {
          if (c == '\n') ++lineno;
          is.get();
        }
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
        size_t differing_pos;
        int read_char;
        int line;
        boost::shared_ptr<std::vector<char> const> message;


        CharactersDifferException(std::string const & l, size_t p, int c, int li)
          : lit(l)
          , differing_pos(p)
          , read_char(c)
          , line(li)
          , message(err_msg())              
        {
          assert(c!=-1);
        }

        const char * what() const throw()
        {
          if (message) {
            return &((*message)[0]);
          }
          else {
            return
              "The program raised a CharactersDifferException; in addition, the error message "
              "for that exception could not be constructed";
          }
        }

        std::vector<char> * err_msg() const
        {
          try {
            std::stringstream ss;
            ss << "CharactersDifferException when reading literal [" << lit << "]\n";
            ss << "               Read " << (char)read_char
               << " instead of " << lit[differing_pos]
               << " at position " << differing_pos
               << " of the literal; line " << line << "\n";

            std::string s = ss.str();
            std::vector<char> * v = new std::vector<char>(s.begin(), s.end());
            return v;
          }
          catch (...) {
            //std::cout << "CharactersDifferException could not allocate memory\n";
            //return "[CharactersDifferException: cannot allocate memory]\n";
            return NULL;
          }
        }

        virtual
        ~CharactersDifferException() throw()
        {}
      };

      struct StreamTooShortException   : std::exception {};

      std::string
      read_lit(std::istream & is, std::string const & lit)
      {
        // Read |lit| characters
        for (size_t i=0; i<lit.size(); ++i) {
          int c = is.get();

          if (!is.good()) {
            // Probably got to EOF
            throw StreamTooShortException();
          }
          if (c != lit[i]) {
            throw CharactersDifferException(lit, i, c, lineno);
          }
        }

        discardws(is);
        return lit;
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

      bool
      is_lparen(int c)
      {
        return c == '(' || c == '{' || c == '[' || c == '<';
      }

      bool
      is_rparen(int c)
      {
        return c == ')' || c == '}' || c == ']' || c == '>';
      }

      std::string
      read_token(std::istream & is)
      {
        int paren_count = 0;
        discardws(is);
        std::string ret;
        while (CONSTANT_CONDITION(true)) {
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

          // is.get() will either be a char or -1 (which we took care of above)
          ret += static_cast<char>(is.get());
        }
        discardws(is);
        return ret;
      }

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
      using opennwa::NwaRefPtr;

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
    
        for(;;) {
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

      std::vector<std::string>
      read_name_list(std::istream & is)
      {
        return read_list<std::string>(is, read_name);
      }

      std::vector<KeyTriple>
      read_triple_list(std::istream & is)
      {
        return read_list<KeyTriple>(is, read_triple);
      }

      std::vector<KeyQuad>
      read_quad_list(std::istream & is)
      {
        return read_list<KeyQuad>(is, read_quad);
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

      void
      read_sigma_block(std::istream & is, NwaRefPtr nwa)
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


      void
      read_state_block(std::istream & is, NwaRefPtr nwa)
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


      void
      read_delta_block(std::istream & is, NwaRefPtr nwa)
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



////////////////////////////////////////////////////////////////////////////////
/// READ_BLOCK
///
/// block  ::=  state-block     // lookahead = 'Q'
///          |  sigma-block     // lookahead = 's'
///          |  delta-block     // lookahead = 'd'
///
/// Reads a block (any kind) and as a semantic action adds the appropriate
/// state/letter/transition to the NWA.

      void
      read_block(std::istream & is, NwaRefPtr nwa)
      {
        switch (is.peek()) {
          case 'Q': read_state_block(is, nwa); break;
          case 'S': read_sigma_block(is, nwa); break;
          case 'D': read_delta_block(is, nwa); break;
          case 'n': return; // we got to the end of that NWA
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


    }}}

////////////////////////////////////////////////////////////////////////////////
/// READ_NWA
///
/// nwa-description  ::= ('nwa' name? ':'?)? '{'? block+ '}'?
///   with an opening brace required if 'nwa' is present and name is not
///   (and yes, there must be a space between the name and either : or {)
///
/// Reads in an nwa-description

namespace opennwa {
  using namespace parser::details;

  NwaRefPtr
  read_nwa(std::istream & is, std::string * nwa_name)
  {
    lineno = 0;
            
    // Create the NWA
    NwaRefPtr nwa = new Nwa();

    // Skip over a bunch of stuff: opening whitespace, the optional
    // 'nwa'-name-':' sequence, and the optional '{'
    discardws(is);
    if (is.peek() == 'n') {
      read_lit(is, "nwa");

      if (is.peek() != ':' && is.peek() != '{') {
        std::string name = read_name(is);
        if (nwa_name) {
          *nwa_name = name;
        }
      }

      if (is.peek() == ':') {
        read_lit(is, ":");
      }
    }
    if (is.peek() == '{') {
      read_lit(is, "{");
    }

    // Now read in the actual NWA, until we reach the end of that spec
    while(is.peek() != -1 && is.peek() != '}' && is.peek() != 'n') {
      read_block(is, nwa);
    }

    // Skip over the } if that's what ended us
    if (is.peek() == '}') {
      read_lit(is, "}");
    }

    return nwa;
  }


  ProcedureMap
  read_nwa_proc_set(std::istream & is)
  {
    discardws(is);
            
    ProcedureMap procs;

    while(is.peek() != -1) {
      std::string name;
      NwaRefPtr nwa = read_nwa(is, &name);

      procs[name] = nwa;
    }

    return procs;
  }

} // namespace opennwa

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
