// This program will walk through creating the NWA shown in Figure 4 of the
// associated documentation, and reverse it (to get the result of Figure 12).
// We then make a NestedWord of the one word in Figure 4's language and
// another NestedWord of the one word in Figure 12's language, then test that
// each is a member of just the appropriate NWA.

#include <iostream>
using std::cout;

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NestedWord.hpp"
#include "wali/nwa/construct/reverse.hpp"
#include "wali/nwa/query/language.hpp"
using wali::getKey;
using namespace wali::nwa;
using wali::nwa::construct::reverse;
using wali::nwa::query::languageContains;

// These symbols are used in the NWA and both words
Symbol const sym_a    = getKey("a");
Symbol const sym_b    = getKey("b");
Symbol const sym_call = getKey("call");
Symbol const sym_ret  = getKey("ret");

/// Creates the NWA shown in Figure 4 of the Wali NWA documentation
NWA
create_figure_4()
{
    NWA out;
    
    // Translate the names of the states then symbols to Wali identifiers
    State start   = getKey("Start");
    State call    = getKey("Call");
    State entry   = getKey("Entry");
    State state   = getKey("State");
    State exit    = getKey("Exit");
    State return_ = getKey("Return");
    State finish  = getKey("Finish");

    // Add the transitions
    out.addInternalTrans(start, sym_a, call);
    out.addCallTrans(call, sym_call, entry);
    out.addInternalTrans(entry, sym_b, state);
    out.addInternalTrans(state, sym_b, exit);
    out.addReturnTrans(exit, call, sym_ret, return_);
    out.addInternalTrans(return_, sym_a, finish);

    // Set the initial and final states
    out.addInitialState(start);
    out.addFinalState(finish);

    return out;
}


/// Creates a (the one) word in the language of Figure 4's NWA
NestedWord
create_forwards_word()
{
    NestedWord out;
    out.appendInternal(sym_a);
    out.appendCall(sym_call);
    out.appendInternal(sym_b);
    out.appendInternal(sym_b);
    out.appendReturn(sym_ret);
    out.appendInternal(sym_a);
    return out;
}


/// Creates a (the one) word in the language of the reverse of Figure 4's NWA
NestedWord create_backwards_word()  { ... }


int main()
{
    // Create the NWA and reversed NWA
    NWA fig4 = create_figure_4();
    NWARefPtr fig4_reversed = reverse(fig4);

    // These are the words we are testing
    NestedWord forwards_word = create_forwards_word();
    NestedWord backwards_word = create_backwards_word();

    // Now do the tests
    cout << "fig4 contains:\n"
         << "     forwards_word  [expect 1] : "
         << languageContains(fig4, forwards_word) << "\n"
         << "     backwards_word [expect 0] : "
         << languageContains(fig4, backwards_word) << "\n"
        
         << "fig4_reversed contains:\n"
         << "     forwards_word  [expect 0] : "
         << languageContains(*fig4_reversed, forwards_word) << "\n"
         << "     backwards_word [expect 1] : "
         << languageContains(*fig4_reversed, backwards_word) << "\n";
}
