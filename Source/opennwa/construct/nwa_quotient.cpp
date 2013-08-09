#include "opennwa/Nwa.hpp"
#include "opennwa/construct/quotient.hpp"


namespace opennwa
{
	namespace construct
	{
		void quotient( Nwa & out, Nwa const & nwa, wali::util::DisjointSets<State> const & partitionMap )
		{

#if 0
			const  &State rep = partitionMap.representative(*sit);

			// Add initial states
			for( StateIterator sit = nwa.beginInitialStates(); sit != nwa.endInitialStates(); sit++ ) {		 
				out.addInitialState( partitionMap.representative(*sit) );
			}

			// Add final states
			for( StateIterator sit = nwa.beginFinalStates(); sit != nwa.endFinalStates(); sit++ ) {		 
				out.addFinalState( partitionMap.representative(*sit) );
			}

			//Add internal transitions
			for(  InternalIterator iit = beginInternalTrans(); iit != endInternalTrans(); iit++ ) {   
				out.addInternalTrans( partitionMap.representative( iit->first ), iit->second, partitionMap.representative( iit->third) );   
			}

			//Add call transitions
			for(  CallIterator cit = beginCallTrans(); cit != endCallTrans(); cit++ ) {   
				out.addCallTrans( partitionMap.representative( cit->first ), cit->second, partitionMap.representative( cit->third) );   
			}

			//Add return transitions
			for(  ReturnIterator rit = beginReturnTrans(); rit != endCReturnTrans(); rit++ ) {   
				out.addReturnTrans( partitionMap.representative( rit->first ), partitionMap.representative(rit->second), rit->third, partitionMap.representative( rit->third) );   
			}

#endif
			return;
		}


		NwaRefPtr quotient( Nwa const & nwa, wali::util::DisjointSets<State> const & partitionMap )
		{
			NwaRefPtr out(new Nwa());
			quotient(*out, nwa, partitionMap);
			return out;
		}

		void printPartitionMap(wali::util::DisjointSets<State> const & partitionMap) {

			partitionMap.output(std::cout << "Printing partitionMap : " <<std::endl);
						
			}

	} // end 'namespace construct'!!!

} // end 'namespace opennwa'!!!


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

