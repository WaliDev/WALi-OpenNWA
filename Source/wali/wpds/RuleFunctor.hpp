#ifndef wali_wpds_RULE_FUNCTOR_GUARD
#define wali_wpds_RULE_FUNCTOR_GUARD 1

/**
 * @author Nicholas Kidd
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include <iostream>
#include <set>

namespace wali
{
  namespace wpds
  {
    class WPDS;
    class Wrapper;
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

    class RuleDotty : public ConstRuleFunctor
    {
      std::ostream & os;
      bool print_states;

      public:
      RuleDotty( std::ostream & o, bool _print_states = false );
      virtual ~RuleDotty();

      virtual void operator()( const rule_t & r );
    };

    /*!
     * @class WpdsStackSymbols
     * Gets all stack symbols from the WPDS and classfies them
     * as entry and return points, as appropriate
     */
    class WpdsStackSymbols : public ConstRuleFunctor
    {
      public:
        std::set< Key > gamma;
        std::set< Key > callPoints;
        std::set< Key > returnPoints;
        std::set< Key > entryPoints;
        std::set< Key > pdsStates;

        WpdsStackSymbols( );
        virtual ~WpdsStackSymbols();

        virtual void operator()( const rule_t & r );

      private:
        void addPoint(Key k, std::set< Key > &s);
    };

    // @author Amanda Burton
    /**
     * 
     * @class WpdsRules
     *
     * Gets all rules from the WPDS and classifies them as
     * push rules, pop rules, and step rules, as appropriate.
     *
     */
    class WpdsRules: public ConstRuleFunctor
    {
      public:
        std::set< Rule > pushRules;
        std::set< Rule > popRules;
        std::set< Rule > stepRules;
        
        WpdsRules();
        virtual ~WpdsRules();
        
        virtual void operator() (const rule_t & r);
        
      private:
        void addRule(Rule r, std::set< Rule > &s);
    };

    class RuleCopier : public ConstRuleFunctor
    {
      public:
        WPDS& w;
        ref_ptr<Wrapper> wrapper;
        RuleCopier(WPDS& w,ref_ptr<Wrapper>);
        virtual void operator()( const rule_t & r);
    };
  } // end namespace wpds

} // end namespace wali

#endif	// wali_wpds_RULE_FUNCTOR_GUARD

