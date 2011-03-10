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
            NWARefPtr finalnwa = new NWA(getKey("[stuck-procedure-nwas-13]"));

            ////////
            // We will set up the states, then fix up the transitions.
            
            // First, combine all of the procedures (to get all the states
            // and transitions.)
            for (ProcedureMap::const_iterator proc = procedures.begin();
                 proc != procedures.end(); ++proc)
            {
                //NWARefPtr min = minimize_internal_nwa(proc->second);
                NWARefPtr min = proc->second;
                finalnwa->combineWith(min);
            }

            // Initial and final states are (just) those of main; set them up.
            finalnwa->clearInitialStates();
            finalnwa->clearFinalStates();

            assert(procedures.find("main") != procedures.end());
            NWARefPtr main_nwa = procedures.find("main")->second;
            
            std::set<St> const & main_entries = main_nwa->getInitialStates();
            std::set<St> const & main_exits = main_nwa->getFinalStates();

            // This assertion isn't needed. TODO: think about removing
            // (rather, change to >= 1). -Evan 3/10/11
            assert(main_entries.size() == 1);
            assert(main_exits.size() == 1);
            
            for (std::set<St>::const_iterator entry = main_entries.begin();
                 entry != main_entries.end(); ++entry)
            {
                finalnwa->addInitialState(*entry);
            }
                    
            for (std::set<St>::const_iterator exit = main_exits.begin();
                 exit != main_exits.end(); ++exit)
            {
                finalnwa->addFinalState(*exit);
            }


            ////////
            // Now set up the transitions
            
            // Now, find each of the transitions on a symbol __call__*. These
            // are the transitions we will replace.
            NWA::Internals fake_call_transitions;
            
            for (NWA::internalIterator trans=finalnwa->beginInternalTrans();
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
                assert(procedures.find(callee_name) != procedures.end());
                NWARefPtr callee = procedures.find(callee_name)->second;
                std::set<St> const & entries = callee->getInitialStates();
                std::set<St> const & exits = callee->getFinalStates();

                // I don't think these assertions are necessary for the below
                // to work, but they do apply in my setting. TODO: consider
                // removing them when I make this code more general. -Evan
                // 3/10/11 (Actually change to >= 1 instead of remove
                // entirely.)
                assert(entries.size() == 1);
                assert(exits.size() == 1);

                // Now add the call transition(s)
                for (std::set<St>::const_iterator entry = entries.begin();
                     entry != entries.end(); ++entry)
                {
                    finalnwa->addCallTrans(call_site, call_key, *entry);
                }

                // Now add the return transition(s)
                for (std::set<St>::const_iterator exit = exits.begin();
                     exit != exits.end(); ++exit)
                {
                    finalnwa->addReturnTrans(*exit, call_site, return_key, return_site);
                }
            }

            return finalnwa;
        } // end assemble_nwa()
    }
}
