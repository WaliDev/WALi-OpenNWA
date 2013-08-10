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
			// for each equivalence class {S}
			//   out.quotientStates(nwa, {S}, resSt, resClientInfo)
			// out_map[representative of {S}] = resSt;
			//  out.setClientInfo(resSt, resClientInfo);


			// Add initial states
			for( StateIterator sit = nwa.beginInitialStates(); sit != nwa.endInitialStates(); sit++ ) {		 
				//out.addInitialState( out_map[partitionMap.representative(*sit)] );
				out.addInitialState( partitionMap.representative(*sit) );
			}

			// Add final states
			for( StateIterator sit = nwa.beginFinalStates(); sit != nwa.endFinalStates(); sit++ ) {		 
				out.addFinalState( partitionMap.representative(*sit) );
			}

			//Add internal transitions
			for(  InternalIterator iit = nwa.beginInternalTrans(); iit != nwa.endInternalTrans(); iit++ ) {   
				out.addInternalTrans( partitionMap.representative( iit->first ), iit->second, partitionMap.representative( iit->third) );   
			}

			//Add call transitions
			for(  CallIterator cit = nwa.beginCallTrans(); cit != nwa.endCallTrans(); cit++ ) {   
				out.addCallTrans( partitionMap.representative( cit->first ), cit->second, partitionMap.representative( cit->third) );   
			}

			//Add return transitions
			for(  ReturnIterator rit = nwa.beginReturnTrans(); rit != nwa.endReturnTrans(); rit++ ) {   
				out.addReturnTrans( partitionMap.representative( rit->first ), partitionMap.representative(rit->second), rit->third, partitionMap.representative( rit->fourth) );   
			}

			return;
		}


		NwaRefPtr quotient( Nwa const & nwa, wali::util::DisjointSets<State> partitionMap )
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

