#ifndef wali_wpds_RULE_FUNCTOR_GUARD
#define wali_wpds_RULE_FUNCTOR_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include <iostream>

namespace wali
{
    namespace wpds
    {
        class Rule;
        class rule_t;

        /*! @class RuleFunctor
         *
         * This functor is used in WPDS::for_each.
         * This is a pure virtual class.
         *
         * @see WPDS
         */
        class RuleFunctor
        {
            public:
                virtual ~RuleFunctor() {}
                virtual void operator()( rule_t & r ) = 0;
        };

        /*! @class ConstRuleFunctor
         *
         * This functor is used in WPDS::for_each.
         * This is a pure virtual class.
         *
         * @see WPDS
         */
        class ConstRuleFunctor
        {
            public:
                virtual ~ConstRuleFunctor() {}
                virtual void operator()( const rule_t & r ) = 0;
        };

        /*!
         * @class RulePrinter
         * Prints rules to constructor param std::ostream o
         */
        class RulePrinter : public ConstRuleFunctor
        {
            std::ostream & os;

            public:
                RulePrinter( std::ostream & o );
                virtual ~RulePrinter();

                virtual void operator()( const rule_t & r );

        };

        /*!
         * @class RulePrinter
         * Marshalls rules to constructor param std::ostream o
         */
        class RuleMarshaller : public ConstRuleFunctor
        {
            std::ostream & os;

            public:
                RuleMarshaller( std::ostream & o );
                virtual ~RuleMarshaller();

                virtual void operator()( const rule_t & r );

        };


    } // end namespace wpds

} // end namespace wali

#endif	// wali_wpds_RULE_FUNCTOR_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
