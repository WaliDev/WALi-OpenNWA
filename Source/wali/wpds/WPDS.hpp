#ifndef wali_wpds_WPDS_GUARD
#define wali_wpds_WPDS_GUARD 1

/**
 * @author Nicholas Kidd
 */

// ::wali
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/HashMap.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/SemElem.hpp"
#include "wali/Worklist.hpp"

// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/TransFunctor.hpp"

// ::wali::wpds
#include "wali/wpds/Wrapper.hpp"

// std c++
#include <iostream>
#include <set>

namespace wali
{
  template< typename T > class Worklist;

  namespace wfa
  {
    class ITrans;
  }

  namespace wpds
  {

    class Config;
    class rule_t;
    class RuleFunctor;
    class ConstRuleFunctor;

    /**
     * @class WPDS
     */
    class WPDS : public Printable, public wfa::ConstTransFunctor
    {

      public:
        static const std::string XMLTag;

      protected:
        typedef HashMap< KeyPair,Config * > chash_t;
        typedef chash_t::iterator iterator;
        typedef chash_t::const_iterator const_iterator;

        /**
         * r2hash_t is a map from key to list of rules. The key
         * is the second RHS stack symbol of a rule. For example, 
         * the rule R1 = < s1,a > -> < s2, b c > will add R1 to 
         * the list that is mapped to by the stack symbol c.
         */
        typedef HashMap< Key, std::list< rule_t > > r2hash_t;

      private:

      public:

        WPDS();
        WPDS( ref_ptr<Wrapper> wrapper );
        WPDS( const WPDS& w );
        virtual ~WPDS();

        /**
         * Clears all rules from the WPDS
         */
        virtual void clear();

        /**
         * Set the worklist used for pre and poststar queries.
         */
        void setWorklist( ref_ptr< Worklist<wfa::ITrans> > wl );


        /** 
         * @brief create rule with no r.h.s. stack symbols
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool add_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            sem_elem_t se );

        /** @brief create rule with one r.h.s. stack symbol
         *
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool add_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            Key to_stack1,
            sem_elem_t se );

        /** @brief create rule with two r.h.s. stack symbols
         *
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool add_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            Key to_stack1,
            Key to_stack2,
            sem_elem_t se );

        /** 
         * @brief create rule with no r.h.s. stack symbols
	 * @brief Replace the weight if the rule already existed
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool replace_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            sem_elem_t se );

        /** @brief create rule with one r.h.s. stack symbol
	 * @brief Replace the weight if the rule already existed
         *
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool replace_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            Key to_stack1,
            sem_elem_t se );

        /** @brief create rule with two r.h.s. stack symbols
	 * @brief Replace the weight if the rule already existed
         *
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool replace_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            Key to_stack1,
            Key to_stack2,
            sem_elem_t se );

        /**
         * @brief Perform prestar reachability query
         *
         * @return wfa::WFA
         *
         * @see wfa::WFA
         */
        virtual wfa::WFA prestar( wfa::WFA const & input );

        /**
         * @brief Perform prestar reachability query
         * The result of the query is stored in
         * the parameter WFA& output. Any transitions
         * in output before the query will be there
         * after the query but will have no effect
         * on the reachability query.
         *
         * @return void
         *
         * @see wfa::WFA
         */
        virtual void prestar( wfa::WFA const & input, wfa::WFA & output );

        /**
         * @brief Perform poststar reachability query
         *
         * @return WFA
         *
         * @see wfa::WFA
         */
        virtual wfa::WFA poststar( wfa::WFA const & input );

        /**
         * @brief Perform poststar reachability query.
         * The result of the query is stored in
         * the parameter WFA& output. Any transitions
         * in output before the query will be there
         * after the query but will have no effect
         * on the reachability query.
         *
         * @return wfa::WFA
         *
         * @see wfa::WFA
         */
        virtual void poststar( wfa::WFA const & input, wfa::WFA & output );

        /**
         * This method writes the WPDS to the passed in 
         * std::ostream parameter. Implements Printable::print.
         *
         * @param o the std::ostream this is written to
         * @return std::ostream this was written to
         *
         * @see Printable
         */
        virtual std::ostream & print( std::ostream & o ) const;

        /**
         * This method marshalls the WPDS into the passed
         * in std::ostream parameter.  Marshalling simply
         * writes the WPDS in XML form.
         *
         * @return std::ostream the WPDS was marshalled into
         */
        virtual std::ostream & marshall( std::ostream & o ) const;

        /**
         * Print the WPDS in dot format. The format is a little strange:
         * by default it does not print state information (since there is
         * typically only one state in a WPDS), however, it can be included
         * optionally.
         *
         * Additionally, delta-0 edges are drawn as edges to nowhere
         * (indicating "I don't know where this goes, since it is dynamic"),
         * and delta-2 edges use the stack symbol which is pushed as the edge
         * label, and call the top of stack symbol the state.
         *
         * Printing weights is not currently supported, though it would not be
         * difficult to add.
         */
        virtual std::ostream & print_dot( 
            std::ostream & o,
            bool print_state=false) const;

        /**
         * @brief Return the number of rules in the WPDS.
         * Note that this function takes time O(num rules), not
         * constant time.
         *
         */
        virtual int count_rules( ) const;

        /**
         * @brief apply ConstRuleFunctor to each rule in WPDS
         *
         * @see ConstRuleFunctor
         * @see Rule
         * @see rule_t
         */
        virtual void for_each( ConstRuleFunctor &func ) const;

        /**
         * @brief apply RuleFunctor to each rule in WPDS
         *
         * @see RuleFunctor
         * @see Rule
         * @see rule_t
         */
        virtual void for_each( RuleFunctor &func );

        /**
         * Implementation of TransFunctor
         */
        virtual void operator()( wfa::ITrans const * t );

        bool is_pds_state(wali::Key k) const;
        int num_pds_states() const { return (int) pds_states.size(); }
        const std::set<wali::Key>& get_states() const { return pds_states; }
 
        /** @brief Runs a poststar query on the following automaton
         * to get one that represents the program CFG.
         *
         * Ainit = { (p, e, <p,e>) | e \in Procedure Entry Point }
         *
         * returns the PDS state p; a map [e -> <p,e>]; and poststar(Ainit)
         */
        Key constructCFG(std::set<Key> &entries, std::map<Key, Key> &entryState, wfa::WFA &cfg);

        sem_elem_t get_theZero() {return theZero; }

        void printStatistics(std::ostream & os) const;
        
      protected:

        /** @brief Actually creates the rule, hanldes the mappings,
         * etc.
	 * @brief Replace the weight if the rule already existed and replace_weight is set,
	 * otherwise take a combine of the weights
         *
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool add_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            Key to_stack1,
            Key to_stack2,
            sem_elem_t se,
	    bool replace_weight,
            rule_t& r );

        /** @brief Actually creates the rule, hanldes the mappings,
         * etc.
         *
         * @return true if rule existed
         *
         * @see sem_elem_t
         * @see Key
         */
        virtual bool add_rule(
            Key from_state,
            Key from_stack,
            Key to_state,
            Key to_stack1,
            Key to_stack2,
            sem_elem_t se,
            rule_t& r );

        /**
         * @brief copy relevant material from input WFA to output WFA
         */
        virtual void setupOutput( wfa::WFA const & input, wfa::WFA& fa );

        /**
         * @brief For each t \in fa, t->setConfig(0)
         */
        virtual void unlinkOutput( wfa::WFA& fa );

        /**
         * @brief Gets WPDS ready for fixpoint
         */
        virtual void prestarSetupFixpoint( wfa::WFA const & input, wfa::WFA& fa );

        /**
         * @brief Performs the fixpoint computation
         */
        virtual void prestarComputeFixpoint( wfa::WFA& fa );

        /**
         * @brief Performs pre for 1 ITrans
         */
        virtual void pre( wfa::ITrans * t, wfa::WFA& fa );

        /**
         * @brief helper method for prestar
         */
        virtual void prestar_handle_call(
            wfa::ITrans * t1 ,
            wfa::ITrans * t2,
            rule_t & r,
            sem_elem_t delta
            );

        /**
         * @brief helper method for prestar
         */
        virtual void prestar_handle_trans(
            wfa::ITrans * t,
            wfa::WFA & ca  ,
            rule_t & r,
            sem_elem_t delta );

        /**
         * @brief Gets WPDS ready for fixpoint
         */
        virtual void poststarSetupFixpoint( wfa::WFA const & input, wfa::WFA& fa );

        /**
         * @brief Performs the fixpoint computation
         */
        virtual void poststarComputeFixpoint( wfa::WFA& fa );

        /**
         * @brief Performs post for 1 ITrans
         */
        virtual void post( wfa::ITrans * t, wfa::WFA& fa );

        /**
         * @brief helper method for poststar
         */
        virtual void poststar_handle_eps_trans(
            wfa::ITrans *teps, 
            wfa::ITrans *tprime, 
            sem_elem_t delta
            );

        /**
         * @brief helper method for poststar
         */
        virtual void poststar_handle_trans(
            wfa::ITrans * t ,
            wfa::WFA & ca   ,
            rule_t & r,
            sem_elem_t delta
            );

        /**
         * @brief create a new temp state from two existing states
         *
         * gen_state is only used by poststar
         *
         * @return Key for new state
         */
        virtual Key gen_state( Key state, Key stack );

        /**
         * @brief link input WFA transitions to Configs
         *
         * @see Config
         * @see wfa::ITrans
         * 
         */
        //virtual void copy_and_link( wfa::WFA & in, wfa::WFA & out );

        /**
         * Create the Config for the state and stack KeyPair.
         * If the Config already exists just return that.
         *
         * @return Config pointer
         */
        virtual Config * make_config( Key state, Key stack );

        /**
         * Creates a rule that links two configurations.
         * If rule exists then (combines the weight if replace_weight is false) or
	 * (replace the weight if replace_weight is true)
         *
         * @return true if Rule already existed
         *
         * @see Config
         * @see sem_elem_t
         * @see rule_t
         */
        virtual bool make_rule(
            Config *f,
            Config *t,
            Key stk2,
	    bool replace_weight,
            rule_t& r );

        /**
         * Creates a rule that links two configurations.
         * If rule exists then combines the weight
         *
         * @return true if Rule already existed
         *
         * @see Config
         * @see sem_elem_t
         * @see rule_t
         */
        virtual bool make_rule(
            Config *f,
            Config *t,
            Key stk2,

            rule_t& r );

        /**
         * Find config with KeyPair(state,stack)
         *
         * @return the Config * or 0 if it doesn't exist
         *
         * @see Config
         * @see KeyPair
         */
        virtual Config * find_config( Key state, Key stack );

        /** @brief helper method for fixpoint loop
         *
         * return true if wfa::ITrans was retrieved from
         * worklist, false if worklist is empty
         */
        virtual bool get_from_worklist( wfa::ITrans * & );

        /**
         * @brief helper function to create and link a transition
         *
         */
        virtual void update(
            Key from, Key stack, Key to, 
            sem_elem_t se, Config * cfg );

        /**
         * update_prime does not need to take a Config b/c no Config
         * will match a transition that is created here. The from state
         * is not \in WFA.P. Therefore we do not need to add it to the
         * worklist.
         *
         * @return generated transition
         */
        virtual wfa::ITrans* update_prime(
            Key from, //<! Guaranteed to be a generated state
            wfa::ITrans* call, //<! The call transition
            rule_t r, //<! The push rule
            sem_elem_t delta, //<! Delta change on the call transition
            sem_elem_t wWithRule //<! delta \extends r->weight()
            );

        /**
         * @return const chash_t reference
         */
        const chash_t & config_map() const {
          return configs;
        }

        /**
         * @return chash_t reference
         */
        chash_t & config_map() {
          return configs;
        }
      private: // methods

      protected: // data members
        ref_ptr<Wrapper> wrapper;
        ref_ptr< Worklist<wfa::ITrans> > worklist;
        chash_t configs;
        std::set< Config * > rule_zeroes;
        r2hash_t r2hash;

        /**
         * Points to the output automaton during a pre or poststar
         * query. Is NULL all other times.
         */
        wfa::WFA* currentOutputWFA; //!< Point

        /**
         * theZero holds onto the semiring zero weight
         * for this WPDS. It is set the very first time a
         * rule is added to the WPDS.
         * 
         * WARNING: FWPDS relies on theZero being valid.
         *          If you go changing theZero, fix FWPDS.
         */
        sem_elem_t theZero; 
        std::set<wali::Key> pds_states; // set of PDS states

      private:

    };

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_WPDS_GUARD

