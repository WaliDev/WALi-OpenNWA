#include "opennwa/Nwa.hpp"
#include "opennwa/construct/quotient.hpp"



namespace opennwa
{
  typedef Nwa::StateIterator StateIterator;
  typedef Nwa::InternalIterator InternalIterator;
  typedef Nwa::CallIterator CallIterator;
  typedef Nwa::ReturnIterator ReturnIterator;

  namespace construct
  {   

    void quotient( Nwa & out, Nwa const & nwa, wali::util::DisjointSets<State> partition )
    {

      //Clear all states(except the stuck state) and transitions from this machine.
      out.clear();

      // Map from the representative of an equivalence class of states of "nwa" NWA
      // to the corresponding state in "out" NWA.
      std::map<State, State> repKeyMap;  

      // For each equivalence class in the given partition...
      for (wali::util::DisjointSets<State>::const_iterator outer_iter = partition.begin(); 
	   outer_iter != partition.end(); outer_iter++)
      {   
	// Collect the set of states of the "nwa" NWA that are in the same equivalence class
	// into "equivalenceClass.
	std::set<State> equivalenceClass; 
	for (wali::util::DisjointSets<State>::InnerSet::const_iterator inner_iter = outer_iter->begin(); 
	     inner_iter != outer_iter->end(); ++inner_iter) {
	  equivalenceClass.insert(*inner_iter);
	}

	// Quotient the states belonging to the same equivalence class.
	// Note: The states in equivalenceClass consist of states of "nwa" NWA. 
	//       resSt is a state of the "out" NWA.
	State resSt;
	ClientInfoRefPtr resCI;
	out.statesQuotient(nwa, equivalenceClass, resSt, resCI);

	// Set client info.
	out.setClientInfo(resSt, resCI);

	// Map the representative of the equivalence class in "nwa" to the new state in "out".
	repKeyMap[ partition.representative( *(equivalenceClass.begin()) ) ] = resSt;
      }

      // Add initial states
      for (StateIterator sit = nwa.beginInitialStates(); sit != nwa.endInitialStates(); sit++) { 
	out.addInitialState( repKeyMap[ partition.representative(*sit) ] );
      }

      // Add final states
      for (StateIterator sit = nwa.beginFinalStates(); sit != nwa.endFinalStates(); sit++) { 
	out.addFinalState( repKeyMap[ partition.representative(*sit) ] );
      }

      //Add internal transitions
      for (InternalIterator iit = nwa.beginInternalTrans(); iit != nwa.endInternalTrans(); iit++ ) { 
	out.addInternalTrans( repKeyMap[ partition.representative( iit->first ) ], 
			      iit->second, 
			      repKeyMap[ partition.representative( iit->third) ] );
      }

      //Add call transitions
      for (CallIterator cit = nwa.beginCallTrans(); cit != nwa.endCallTrans(); cit++) {   
	out.addCallTrans( repKeyMap[ partition.representative( cit->first ) ], 
			  cit->second, 
			  repKeyMap[ partition.representative( cit->third) ] );
      }

      //Add return transitions
      for (ReturnIterator rit = nwa.beginReturnTrans(); rit != nwa.endReturnTrans(); rit++) {   
	out.addReturnTrans( repKeyMap[ partition.representative( rit->first ) ], 
			    repKeyMap[ partition.representative(rit->second) ], 
			    rit->third, 
			    repKeyMap[ partition.representative( rit->fourth) ] );
      }

      return;
    }


    NwaRefPtr quotient( Nwa const & nwa, wali::util::DisjointSets<State> partition )
    {
      NwaRefPtr out(new Nwa());
      quotient(*out, nwa, partition);
      return out;
    }

  } // end 'namespace construct'

} // end 'namespace opennwa'


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

