#ifndef wali_nwa_WeightGen_GUARD
#define wali_nwa_WeightGen_GUARD 1

/**
 * @author Amanda Burton
 */
 
 namespace wali
 {
 
  namespace nwa
  {
    template<typename St,typename Sym>
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
        
        virtual sem_elem_t getOne();
        
        virtual sem_elem_t getWeight( St from, Sym inst, Kind kind, St to );
    };
      
  }
}
#endif