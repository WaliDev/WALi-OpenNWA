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
		

		void quotient( Nwa & out, Nwa const & nwa, wali::util::DisjointSets<State> partitionMap )
		{
			State resSt;
			ClientInfoRefPtr resCI;
			std::map<State, State> repUniqueKeyMap;

			for( wali::util::DisjointSets<State>::const_iterator outer_iterator = partitionMap.begin(); outer_iterator != partitionMap.end(); outer_iterator++ )
			{   
				out.statesQuotient(nwa, partitionMap, outer_iterator, resSt, resCI);
				repUniqueKeyMap[ partitionMap.representative( *(outer_iterator->begin()) ) ] = resSt;
				//  out.setClientInfo(resSt, resClientInfo);
			}

			// Add initial states
			for( StateIterator sit = nwa.beginInitialStates(); sit != nwa.endInitialStates(); sit++ ) {		 
				out.addInitialState( repUniqueKeyMap[ partitionMap.representative(*sit) ] );
			}

			// Add final states
			for( StateIterator sit = nwa.beginFinalStates(); sit != nwa.endFinalStates(); sit++ ) {		 
				out.addFinalState( repUniqueKeyMap[ partitionMap.representative(*sit) ] );
			}

			//Add internal transitions
			for(  InternalIterator iit = nwa.beginInternalTrans(); iit != nwa.endInternalTrans(); iit++ ) {   
				out.addInternalTrans( repUniqueKeyMap[ partitionMap.representative( iit->first ) ], iit->second, repUniqueKeyMap[ partitionMap.representative( iit->third) ] );   
			}

			//Add call transitions
			for(  CallIterator cit = nwa.beginCallTrans(); cit != nwa.endCallTrans(); cit++ ) {   
				out.addCallTrans( repUniqueKeyMap[ partitionMap.representative( cit->first ) ], cit->second, repUniqueKeyMap[ partitionMap.representative( cit->third) ] );   
			}

			//Add return transitions
			for(  ReturnIterator rit = nwa.beginReturnTrans(); rit != nwa.endReturnTrans(); rit++ ) {   
				out.addReturnTrans( repUniqueKeyMap[ partitionMap.representative( rit->first ) ], repUniqueKeyMap[ partitionMap.representative(rit->second) ], rit->third, repUniqueKeyMap[ partitionMap.representative( rit->fourth) ] );   
			}

			return;
		}


		NwaRefPtr quotient( Nwa const & nwa, wali::util::DisjointSets<State> partitionMap )
		{
			NwaRefPtr out(new Nwa());
			quotient(*out, nwa, partitionMap);
			return out;
		}

	} // end 'namespace construct'!!!

} // end 'namespace opennwa'!!!


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

