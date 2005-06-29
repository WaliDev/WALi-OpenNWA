#ifndef wali_wfa_TRANS_FUNCTOR_GUARD
#define wali_wfa_TRANS_FUNCTOR_GUARD 1
#pragma once

/*!
 * $Id: TransFunctor.hpp,v 1.3 2005/06/23 17:07:10 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.3 $
 */

#include <iostream>

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

                virtual void operator()( Trans * t ) = 0;
        };

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

                virtual void operator()( const Trans * t ) = 0;
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

                virtual void operator()( const Trans * t );
        };

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

                virtual void operator()( const Trans * t );
        };

        /*!
         * @class TransDeleter
         *
         * Calls delete on each Trans* object passed to it.
         *
         * @see TransFunctor
         */
        class TransDeleter : public TransFunctor
        {
            public:
                virtual ~TransDeleter() {}

                virtual void operator()( Trans * t );
        };

        /*!
         * @class TransDotty
         * Prints each Trans to std::ostream in dotty format.
         */
        class TransDotty : public ConstTransFunctor
        {
            public:
                std::ostream& o;
                bool print_weights;

                TransDotty( std::ostream& o, bool print_weights );

                virtual ~TransDotty() {}

                virtual void operator()( const Trans * t );
        };

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

                virtual void operator()( const Trans * t );
        };

    } // namespace wfa

} // namespace wali

#endif  // wali_wfa_TRANS_FUNCTOR_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
