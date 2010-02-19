#ifndef wali_nwa_WeightGen_GUARD
#define wali_nwa_WeightGen_GUARD 1

/**
 * @author Amanda Burton
 */
 
namespace wali
{
  namespace nwa
  {
    /**
     *
     * This class is used in NWA::NWAtoPDS and NWA::NWAtoBackwardsPDS.  It is used to
     * compute the weights for the rules added to the WPDS and must be extended to 
     * incorporate the desired semiring in order to use either of these methods.
     *  
     */
    //TODO: do these methods need NWARefPtr to make sense?

    class WeightGen
    {
      public:
        //The kinds of edges that need to be considered.
        //  INTRA:  corresponds to an Internal transition in the NWA
        //  CALL_TO_ENTRY:  corresponds to a Call transition in the NWA
        //  EXIT_TO_RET:  corresponds to the exit-to-return part of a Return transition in the NWA
        //  CALL_TO_RET:  corresponds to the call-to-return part of a Return transition in the NWA
        enum Kind {INTRA, CALL_TO_ENTRY, EXIT_TO_RET, CALL_TO_RET}; 
      
      //
      // Methods
      //
      
      public:
      
      //Constructors and Destructor
      WeightGen( ) { }
        
      /**
       *  
       * @brief access the One of the desired semiring
       *
       * This method returns the One of the desired semiring.  
       *
       * @return the One of the desired semiring
       *
       */
      virtual sem_elem_t getOne( ) 
      {
        // TODO: fix this somehow!
        assert(0); //grrr ugliness 
        return NULL;
      }

      /**
       *
       * @brief computes the weight(in the desired semiring) for a rule of the WPDS from 
       *        the given edge
       *
       * This method computes the weight(in the desired semiring) for a WPDS rule from 
       * the (kind) edge between 'src' and 'tgt' labeled with symbol 'sym'.
       * Note: The kind will never be CALL_TO_RET, this case is never needed.
       *
       * @param - src: the source of the edge
       * @param - sym: the symbol labeling the edge
       * @param - kind: the variety of edge
       * @param - tgt: the target of the edge
       * @return the weight to put on the rule corresponding to the given edge
       *         
       */
      virtual sem_elem_t getWeight( Key src, Key sym, Kind kind, Key tgt ) 
      {
        return getOne();
      }
        
      /** 
       * 
       * @brief computes the weight(in the desired semiring) for the return rule of the 
       *        WPDS associated with the given exit
       *
       * This method computes the weight(in the desired semiring) for the return rule of
       * the WPDS corresponding to the exit 'src'.
       * Note: This value is generally the same as getOne().
       * 
       * @param - src: the source of the exit edge
       * @return the weight to put on the return rule corresponding to the given exit 
       *
       */
      virtual sem_elem_t getExitWeight( Key src) 
      {
        return getOne();
      }

      /**
       *
       * @brief computes the weight(in the desired semiring) for a rule when the symbol
       *        labeling the corresponding edge in the NWA is the wild symbol
       *
       * This method computes the weight(in the desired semiring) for a WPDS rule from 
       * the edge between 'src' and 'tgt' labeled with the wild symbol.
       *
       * @param - src: the source of the edge
       * @param - tgt: the target of the edge
       * @return the weight to put on the rule corresponding to the given edge
       *
       */
      virtual sem_elem_t getWildWeight( Key src, Key tgt )
      { 
        return getOne();
      }
    };      
  }
}
#endif