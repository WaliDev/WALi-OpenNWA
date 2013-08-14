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
			State resSt;
			ClientInfoRefPtr resCI;
			std::map<State, State> repUniqueKeyMap;   // maps a representative to a fresh unique key

			for( wali::util::DisjointSets<State>::const_iterator outer_iter = partition.begin(); outer_iter != partition.end(); outer_iter++ )
			{   
				std::set<State> equivalenceClass;    // equivalence class of the partition
				for( wali::util::DisjointSets<State>::InnerSet::const_iterator inner_iter = outer_iter->begin(); inner_iter != outer_iter->end(); ++inner_iter ) {
					equivalenceClass.insert(*inner_iter);
				}

				/* equivalenceClass is for the states of first parameter i.e. nwa and 
				resSt and resCi belong to "this" nwa which is "out"
				*/
				out.statesQuotient(nwa, equivalenceClass, resSt, resCI);
				repUniqueKeyMap[ partition.representative( *(equivalenceClass.begin()) ) ] = resSt;
				out.setClientInfo(resSt, resCI);
			}

			// Add initial states
			for( StateIterator sit = nwa.beginInitialStates(); sit != nwa.endInitialStates(); sit++ ) {		 
				out.addInitialState( repUniqueKeyMap[ partition.representative(*sit) ] );
			}

			// Add final states
			for( StateIterator sit = nwa.beginFinalStates(); sit != nwa.endFinalStates(); sit++ ) {		 
				out.addFinalState( repUniqueKeyMap[ partition.representative(*sit) ] );
			}

			//Add internal transitions
			for(  InternalIterator iit = nwa.beginInternalTrans(); iit != nwa.endInternalTrans(); iit++ ) {   
				out.addInternalTrans( repUniqueKeyMap[ partition.representative( iit->first ) ], iit->second, repUniqueKeyMap[ partition.representative( iit->third) ] );
			}

			//Add call transitions
			for(  CallIterator cit = nwa.beginCallTrans(); cit != nwa.endCallTrans(); cit++ ) {   
				out.addCallTrans( repUniqueKeyMap[ partition.representative( cit->first ) ], cit->second, repUniqueKeyMap[ partition.representative( cit->third) ] );
			}

			//Add return transitions
			for(  ReturnIterator rit = nwa.beginReturnTrans(); rit != nwa.endReturnTrans(); rit++ ) {   
				out.addReturnTrans( repUniqueKeyMap[ partition.representative( rit->first ) ], repUniqueKeyMap[ partition.representative(rit->second) ], rit->third, repUniqueKeyMap[ partition.representative( rit->fourth) ] );
			}

			return;
		}


		NwaRefPtr quotient( Nwa const & nwa, wali::util::DisjointSets<State> partition )
		{
			NwaRefPtr out(new Nwa());
			quotient(*out, nwa, partition);
			return out;
		}

	} // end 'namespace construct'!!!

} // end 'namespace opennwa'!!!


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

