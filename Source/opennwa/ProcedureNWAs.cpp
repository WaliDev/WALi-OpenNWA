#include "opennwa/ProcedureNwas.hpp"
#include "opennwa/OpenFstInterop.hpp"

#include "query/internals.hpp"
#include "query/calls.hpp"
#include "query/returns.hpp"

using wali::key2str;

namespace opennwa {

  bool
  string_starts_with(std::string const & str, std::string const & prefix)
  {
    return str.compare(0, prefix.size(), prefix) == 0;
  }

  std::string
  remove_prefix(std::string const & str, std::string const & prefix)
  {
    assert(string_starts_with(str, prefix));
    return str.substr(prefix.size());
  }


  static std::string call_prefix = "__call__";

        
  NwaRefPtr
  assemble_nwa(ProcedureMap const & procedures,
               boost::function<void (Nwa &, State, State)> call_inserter,
               boost::function<void (Nwa &, State, State, State)> return_inserter)
  {
    NwaRefPtr finalnwa = new Nwa();

    std::map<std::string, std::set<State> > entries_map;
    std::map<std::string, std::set<State> > exits_map;

    ////////
    // We will set up the states, then fix up the transitions.
            
    // First, combine all of the procedures (to get all the states
    // and transitions.)
    for (ProcedureMap::const_iterator proc = procedures.begin();
         proc != procedures.end(); ++proc)
    {
      NwaRefPtr min = minimize_internal_nwa(proc->second);
      //NwaRefPtr min = proc->second;

      entries_map[proc->first] = min->getInitialStates();
      exits_map[proc->first] = min->getFinalStates();

      if (proc->first != "main") {
        min->clearInitialStates();
        min->clearFinalStates();
      }

      finalnwa->combineWith(*min);
    }


    ////////
    // Now set up the transitions
            
    // Now, find each of the transitions on a symbol __call__*. These
    // are the transitions we will replace.
    Nwa::Internals fake_call_transitions;
            
    for (Nwa::InternalIterator trans=finalnwa->beginInternalTrans();
         trans != finalnwa->endInternalTrans(); ++trans)
    {
      std::string symbol = key2str(trans->second);

      if (string_starts_with(symbol, call_prefix)) {
        fake_call_transitions.insert(*trans);
      }
    }


    // Okay, now we have to do two things with each of those
    // transitions. The simpler one is to remove it. The more
    // complicated one is add a call and a return transition.  The
    // call transition goes from the source to the entry of the
    // procedure that corresponds to the transition's symbol. The
    // return goes from the exit of that procedure to the return
    // node, with the call node as the predecessor.
    //
    // Schematically:
    //                  C ------------------> R
    //                       __call__foo
    // turns into
    //                  C               R
    //                  |              /\     .
    //             call |               | return (C as call predecessor)
    //                  |               |
    //                  V               |
    //             entry_foo         exit_foo
    //
    for (Nwa::Internals::iterator fake_call = fake_call_transitions.begin();
         fake_call != fake_call_transitions.end(); ++fake_call)
    {
      // Remove the fake call.
      finalnwa->removeInternalTrans(*fake_call);

      // Prepare for the call and return transitions
      Key call_site = fake_call->first;
      Key return_site = fake_call->third;
                
      std::string symbol = key2str(fake_call->second);
      std::string callee_name = remove_prefix(symbol, call_prefix);
      assert(entries_map.find(callee_name) != entries_map.end());
      assert(exits_map.find(callee_name) != exits_map.end());
      std::set<State> const & entries = entries_map[callee_name];
      std::set<State> const & exits = exits_map[callee_name];

      // I don't think these assertions are necessary for the below
      // to work, but they do apply in my setting. TODO: consider
      // removing them when I make this code more general. -Evan
      // 3/10/11 (Actually change to >= 1 instead of remove
      // entirely.)
      assert(entries.size() == 1);

      // Now add the call transition(s)
      for (std::set<State>::const_iterator entry = entries.begin();
           entry != entries.end(); ++entry)
      {
        call_inserter(*finalnwa, call_site, *entry);
        //finalnwa->addCallTrans(call_site, call_key, *entry);
        //finalnwa->addInternalTrans(call_site, EPSILON, *entry);
      }

      // Now add the return transition(s)
      for (std::set<State>::const_iterator exit = exits.begin();
           exit != exits.end(); ++exit)
      {
        return_inserter(*finalnwa, *exit, call_site, return_site);
        //finalnwa->addReturnTrans(*exit, call_site, return_key, return_site);
        //finalnwa->addInternalTrans(*exit, EPSILON, return_site);
      }
    }


    // Finally, we remove the __call__ symbols from the automaton.
    std::set<Symbol> to_remove;
    for (Nwa::SymbolIterator symiter = finalnwa->beginSymbols();
         symiter != finalnwa->endSymbols(); ++symiter)
    {
      Symbol symbol = *symiter;

      if (string_starts_with(key2str(symbol), call_prefix)) {
        // It shouldn't be used in any transitions. Check that.
        assert(query::getSources_Sym(*finalnwa, symbol).size() == 0);
        assert(query::getCallSites_Sym(*finalnwa, symbol).size() == 0);
        assert(query::getExits_Sym(*finalnwa, symbol).size() == 0);

        // Now schedule it for removal
        to_remove.insert(symbol);
      }
    }

    for (std::set<Symbol>::iterator s=to_remove.begin(); s!=to_remove.end(); ++s){
      finalnwa->removeSymbol(*s);
    }


    return finalnwa;
  } // end assemble_nwa()

}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
