#ifndef wali_VISITOR_GUARD
#define wali_VISITOR_GUARD 1

/*!
 * @author Nicholas Kidd
 */
#include "wali/Common.hpp"

namespace wali
{
  namespace witness
  {
    /*!
     * @class Visitor
     * Declares the methods a Visitor must implement to traverse a Witness
     * DAG. Class Visitor provides a default method for each visit_* method
     * that simply aborts the computation.
     *
     * @see Visitable
     * @see Witness
     * @see WitnessExtend
     * @see WitnessCombine
     * @see WitnessRule
     * @see WitnessTrans
     */
    class Witness;
    class WitnessExtend;
    class WitnessCombine;
    class WitnessRule;
    class WitnessTrans; 
    class WitnessMerge;

    class Visitor
    {
      public:
        virtual ~Visitor() {}

        /*!
         * @return true to continue visiting children, false to stop.
         */
        virtual bool visit( Witness * w );

        /*!
         * @return true to continue visiting children, false to stop.
         */
        virtual bool visitExtend( WitnessExtend * w );

        /*!
         * @return true to continue visiting children, false to stop.
         */
        virtual bool visitCombine( WitnessCombine * w );

        /*!
         * @return true to continue visiting children, false to stop.
         */
        virtual bool visitRule( WitnessRule * w );

        /*!
         * @return true to continue visiting children, false to stop.
         */
        virtual bool visitTrans( WitnessTrans * w );

        //!
        //! @return true to continue visiting children, false to stop
        //!
        virtual bool visitMerge( WitnessMerge * w );


        //! Called in postorder. NOT called if visitExtend() returns false.
        virtual void postvisitExtend( WitnessExtend * w)   { (void) w; }

        //! Called in postorder. NOT called if visitCombine() returns false.
        virtual void postvisitCombine( WitnessCombine * w) { (void) w; }

        //! Called in postorder. NOT called if visitRule() returns false.
        virtual void postvisitRule( WitnessRule * w)       { (void) w; }

        //! Called in postorder. NOT called if visitTrans() returns false.
        virtual void postvisitTrans( WitnessTrans * w)     { (void) w; }

        //! Called in postorder. NOT called if visitMerge() returns false.
        virtual void postvisitMerge( WitnessMerge * w)     { (void) w; }

    }; // class Visitor

  } // namespace witness

} // namespace wali

#endif  // wali_VISITOR_GUARD

