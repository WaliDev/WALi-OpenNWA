#include "ProcedureNWAs.hpp"

#include "OpenFstInterop.hpp"

namespace wali {
  namespace nwa {

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

        
    NWARefPtr
    assemble_nwa(ProcedureMap const & procedures, Key call_key, Key return_key)
    {
      NWARefPtr finalnwa = new NWA();

      std::map<std::string, std::set<State> > entries_map;
      std::map<std::string, std::set<State> > exits_map;

      ////////
      // We will set up the states, then fix up the transitions.
            
      // First, combine all of the procedures (to get all the states
      // and transitions.)
      for (ProcedureMap::const_iterator proc = procedures.begin();
           proc != procedures.end(); ++proc)
      {
        NWARefPtr min = minimize_internal_nwa(proc->second);
        //NWARefPtr min = proc->second;

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
      NWA::Internals fake_call_transitions;
            
      for (NWA::InternalIterator trans=finalnwa->beginInternalTrans();
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
      for (NWA::Internals::iterator fake_call = fake_call_transitions.begin();
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
          finalnwa->addCallTrans(call_site, call_key, *entry);
        }

        // Now add the return transition(s)
        for (std::set<State>::const_iterator exit = exits.begin();
             exit != exits.end(); ++exit)
        {
          finalnwa->addReturnTrans(*exit, call_site, return_key, return_site);
        }
      }

      return finalnwa;
    } // end assemble_nwa()
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
