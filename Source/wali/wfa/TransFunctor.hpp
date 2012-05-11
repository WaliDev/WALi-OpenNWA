#ifndef wali_wfa_TRANS_FUNCTOR_GUARD
#define wali_wfa_TRANS_FUNCTOR_GUARD 1
#pragma once

/*!
 * @author Nicholas Kidd
 */

#include <iostream>
#include <map>
#include "wali/HashMap.hpp"
#include "wali/wfa/TransSet.hpp"

namespace wali
{
  namespace wfa
  {
    class Trans;    //! < Needed by all (Const)TransFunctors
    class WFA;      //! < Needed by TransCopier

    /*!
     * @class TransFunctor
     *
     * Pure virtual class exporting the interface used
     * by wali::wfa::WFA::for_each. This class allows
     * for modification of the WFA's Trans objects in place.
     *
     * @see Trans
     * @see WFA
     */
    class TransFunctor
    {
      public:
        virtual ~TransFunctor() {}

        virtual void operator()( ITrans* t ) = 0;
    }; // class TransFunctor

    /*!
     * @class ConstTransFunctor
     *
     * Pure virtual class exporting the interface used by
     * wali::wfa::WFA::for_each. This class DOES NOT ALLOW for
     * modification of the WFA's Trans objects.
     *
     * @see Trans
     * @see WFA
     */
    class ConstTransFunctor
    {
      public:
        virtual ~ConstTransFunctor() {}

        virtual void operator()( const ITrans* t ) = 0;
    }; // class ConstTransFunctor

    /** 
     *
     */
    class TransCounter : public wali::wfa::ConstTransFunctor
    {
      int numTrans;
    public:
      TransCounter( ) : numTrans(0) {}
      virtual ~TransCounter() {}

      virtual void operator()( const ITrans* t )
      {
        (void) t;
        numTrans++;
      }

      int getNumTrans( )
      {
        return numTrans;
      }
    };

    /*!
     * @class TransPrinter
     *
     * Writes Transes to its std::ostream member var. Used
     * by wali::wfa::WFA::print. Calls Trans::print( std::ostream& ).
     *
     * @see ConstTransFunctor
     * @see Trans
     * @see WFA
     */
    class TransPrinter : public ConstTransFunctor
    {
      std::ostream& o;
      public:
      TransPrinter( std::ostream & o_ ) : o(o_) {}

      virtual ~TransPrinter() {}

      virtual void operator()( const ITrans* t );
    }; // class TransPrinter

    /*!
     * @class TransCopier
     *
     * Inserts a copy of the passed in Trans into its member
     * var WFA. Used by WFA copy constructor and operator=.
     *
     * @see ConstTransFunctor
     * @see Trans
     * @see WFA
     */
    class TransCopier : public ConstTransFunctor
    {
      WFA & fa;
      public:
      TransCopier( WFA & fa_ ) : fa(fa_) {}

      virtual ~TransCopier() {}

      virtual void operator()( const ITrans* t );
    }; // class TransCopier

    /*!
     * @class TransDuplicator
     *
     * Used by WFA::duplicateStates
     * Changes transition states
     *
     * @see ConstTransFunctor
     * @see Trans
     * @see WFA
     */
    class TransDuplicator : public ConstTransFunctor
    {
      WFA & fa;
      std::map< Key, Key > &dup;
      public:
      TransDuplicator( WFA & fa_, std::map< Key, Key > &dup_ ) : fa(fa_), dup(dup_) {}

      virtual ~TransDuplicator() {}

      virtual void operator()( const ITrans* t );
    }; // class TransDuplicator


    /*!
     * @class TransDeleter
     *
     * Calls delete on each ITrans* object passed to it.
     *
     * @see TransFunctor
     */
    class TransDeleter : public TransFunctor
    {
      public:
        virtual ~TransDeleter() {}

        virtual void operator()( ITrans* t );
    }; // class TransDeleter


    /*!
     * @class TransZeroWeight
     *
     * Accumulates the set of transitions with zero weight
     *
     * @see TransFunctor
     */
    class TransZeroWeight : public TransFunctor
    {
      public:
        TransSet zeroWeightTrans;
        virtual ~TransZeroWeight() {}

        virtual void operator()( ITrans* t );
    }; // class TransZeroWeight


    class DotAttributePrinter;

    /*!
     * @class TransDotty
     * Prints each Trans to std::ostream in dotty format.
     */
    class TransDotty : public ConstTransFunctor
    {
      public:
        std::ostream& o;
        bool print_weights;
        DotAttributePrinter * printer;

        TransDotty( std::ostream& o, bool print_weights, DotAttributePrinter * attribute_printer );

        virtual ~TransDotty() {}

        virtual void operator()( const ITrans* t );
    }; // class TransDotty

    /*!
     * @class TransMarshaller
     * Writes a Trans in xml format
     */
    class TransMarshaller : public ConstTransFunctor
    {
      public:
        std::ostream& o;

        TransMarshaller( std::ostream& o );

        virtual ~TransMarshaller() {}

        virtual void operator()( const ITrans* t );
    }; // class TransMarshaller

    /*!
     * @class StackHasher
     * Hashes the transitions on their stack symbol.
     * Used by WFA::intersect
     */

    class StackHasher : public TransFunctor
    {
      public:
        typedef wali::HashMap< Key , TransSet > stackmap_t;
        typedef stackmap_t::iterator iterator;
        stackmap_t stackmap;

        virtual ~StackHasher() {}

        virtual void operator()( ITrans* t );

        iterator begin();
        iterator end();
        iterator find( Key k );

    }; // class StackHasher

  } // namespace wfa

} // namespace wali

#endif  // wali_wfa_TRANS_FUNCTOR_GUARD

