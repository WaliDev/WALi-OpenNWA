#ifndef wali_nwa_WeightGen_GUARD
#define wali_nwa_WeightGen_GUARD 1
#include "wali/Reach.hpp"
/**
 * @author Amanda Burton
 */
 
 namespace wali
 {
 
  namespace nwa
  {

    template<typename St,typename Sym, typename W = Reach>

    class WeightGen
    {
      public:
        enum Kind {INTRA, CALL_TO_ENTRY, EXIT_TO_RET, CALL_TO_RET};
      
      //
      // Methods
      //
      
      public:
        //Constructors and Destructor
        WeightGen( ) { }
        
        sem_elem_t getOne() {
          ref_ptr<W> w;
          return w->one();
        }

        sem_elem_t getWeight( St from, Sym inst, Kind kind, St to ) {
        //sem_elem_t getWeight( St from, Sym inst, St to ) {
          return getOne();
        }
        
        sem_elem_t getWildWeight( St from, St to )
        { //TODO: want the default here to be bottom
          return getOne();
        }

    };
      
  }
}
#endif