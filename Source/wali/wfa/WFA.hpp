#ifndef wali_wfa_WFA_GUARD
#define wali_wfa_WFA_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

// ::wali
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/SemElem.hpp"
#include "wali/HashMap.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/Progress.hpp"
#include "wali/domains/SemElemSet.hpp"

// ::wali::wfa
#include "wali/wfa/WeightMaker.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/DeterminizeWeightGen.hpp"

// std::c++
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>

namespace wali
{
  template< typename T > class Worklist;

  namespace wpds
  {
    class WPDS;
    class DebugWPDS;
    class TransCopyLinker;
    namespace ewpds
    {
      class EWPDS;
    }
    namespace fwpds
    {
      class FWPDS;
      class SWPDS;
    }
  }

  namespace regex {
    class Regex;
    typedef wali::ref_ptr<Regex> regex_t;
  }

  namespace wfa
  {
    class State;
    class TransFunctor;
    class ConstTransFunctor;
    class DeterminizeWeightGen;

    /**
     * "Callbacks" for outputting attributes in the Dot.
     */
    struct DotAttributePrinter {
      virtual void print_extra_attributes(State const * state, std::ostream& o) = 0;
      virtual void print_extra_attributes(ITrans const * trans, std::ostream& o) = 0;
      virtual ~DotAttributePrinter() {}
    };

      bool is_epsilon_transition(ITrans const * trans);
    
    bool is_any_transition(ITrans const * trans);
      

    /** @class WFA
     *
     * TODO:
     *  - Should state_map_t have type HashMap< Key , ref_ptr<State> >
     *      This allows for automatic collection of States via HashMap
     *      State objects can escape the WFA (@see WPDS).
     */

    class WFA : public Printable
    {
      //
      // Types
      //
      public:
        /**
         * @enum query_t
         * query_t determines how the weights are extended
         * during path_summary. INORDER is the normal each state's
         * weight is the combine of all outgoing transitions's weights
         * extended by the "to" state. REVERSE reverses this. INORDER
         * corresponds to a prestar query while REVERSE corresponds to
         * a poststar query when used with WPDS
         */
        enum query_t { INORDER,REVERSE,MAX };

        enum PathSummaryImplementation {
            IterativeOriginal,
            IterativeWpds,
            TarjanFwpds,
            CrossCheckAll
        };

        static PathSummaryImplementation globalDefaultPathSummaryImplementation;

        typedef wali::HashMap< KeyPair, TransSet > kp_map_t;
        typedef wali::HashMap< Key , State * > state_map_t;
        typedef wali::HashMap< Key , TransSet > eps_map_t;
        typedef std::set< State*,State > StateSet_t;
        typedef wali::HashMap< Key,StateSet_t > PredHash_t;
        typedef wali::HashMap< Key, std::vector<ITrans*> > IncomingTransMap_t;

        friend class ::wali::wpds::WPDS;
        friend class ::wali::wpds::DebugWPDS;
        friend class ::wali::wpds::ewpds::EWPDS;
        friend class ::wali::wpds::fwpds::FWPDS;
        friend class ::wali::wpds::fwpds::SWPDS;

        static const std::string XMLTag;
        static const std::string XMLQueryTag;
        static const std::string XMLInorderTag;
        static const std::string XMLReverseTag;

        
        typedef std::map<Key, sem_elem_t> AccessibleStateMap;
        typedef std::map<Key, AccessibleStateMap> EpsilonCloseCache;
        typedef std::map<Key, domains::SemElemSet::ElementSet > AccessibleStateSetMap;
        typedef std::vector<Key> Word;
        
      protected:
      private:

        //
        // Methods
        //
      public:
        WFA( query_t q = INORDER, progress_t prog = NULL );
        WFA( const WFA & rhs );
        WFA & operator=( const WFA & rhs );

        virtual ~WFA();

        /**
         * Remove all transitions from the WFA
         */
        virtual void clear();

        /**
         * @brief set initial state
         *
         * A WFA has 1 initial state. This method will
         * set it to the key parameter
         *
         * @param key the key for the desired initial state
         * @return Key that is the old initial stat
         */
        Key set_initial_state( Key key );

        /**
         * @brief set initial state
         *
         * A WFA has 1 initial state. This method will
         * set it to the key parameter
         *
         * @param key the key for the desired initial state
         * @return Key that is the old initial stat
         */
        Key setInitialState( Key key );

        /**
         * @brief Return the initial state
         *
         * @return Key for the initial state
         */
        Key initial_state() const;

        /**
         * @brief Return the initial state
         *
         * @return Key for the initial state
         */
        Key getInitialState() const;

        /**
         * Test if param key is the initial state.
         *
         * @return true if key == WFA::initial_state()
         */
        bool isInitialState( Key key ) const;

        /**
         * Add parameter key to the set of final states
         * The state must already have been added to the FA
         */
        void add_final_state( Key key );

        /**
         * Add parameter key to the set of final states
         * The state must already have been added to the FA
         */
        void addFinalState( Key key );

        /**
         * Add parameter key to the set of final states, accepting with the
         * given weight.
         */
        void addFinalState(Key key, sem_elem_t accept_weight);

        /**
         * Return true if parameter key is a final state
         */
        bool isFinalState( Key key ) const;

        /**
         * Set the WFA query mode.
         * @return the old query mode
         */
        query_t setQuery( query_t newQuery );

        /**
         * @return WFA query mode
         *
         * The query mode effects how weights are extended during
         * path summary. 
         *
         * @see WFA::query_t
         */
        query_t getQuery() const;

        /**
         * @return The current generation
         *
         * Each WFA keeps a generation, which is
         * the number of times a reachability query has
         * been invoked on the WFA.
         */
        size_t getGeneration() const;

        /**
         * @brief Set the generation to have the value of param g
         * @return void
         */
        void setGeneration(size_t g);

        /** @brief Get a weight from the WFA. This is to get hold
         * of the weight domain class
         *
         * @see sem_elem_t
         * @return a weight on some transition in the WFA
         */
        virtual sem_elem_t getSomeWeight() const;

        /** @brief Add transition (p,g,q) to the WFA
         *
         * @see Key
         * @see sem_elem_t
         */
        virtual void addTrans(
            Key p,
            Key g,
            Key q,
            sem_elem_t se );

        /**
         * @brief Add Trans t to the WFA
         *
         * @see wali::wfa::Trans
         */
        virtual void addTrans( ITrans * t );

        /**
         * @brief erase Trans
         *
         * Removes Trans (from,stack,to) from the WFA if it exists.
         */
        virtual void erase(
            Key from,
            Key stack,
            Key to );

        /**
         * @brief erase State q and all of its outgoing transitions
         * It does not remove incoming transitions -- 
         * <b><i>that has to be done by the client</i></b>. 
         * 
         * This does not delete the wfa::State object. The destructor
         * will take care of reclaiming memory.
         *
         * @return true if it was sucessful
         */
        virtual bool eraseState( Key q );

        /**
         * @brief return true if transition matching (p,g,q) exists
         * and copy that transition into the ref parameter t
         *
         * If a Trans matching (p,g,q) exists then it is copied into the
         * passed in reference "Trans & t". This way the user is not given
         * a handle to the WFA's internal Trans * pointer and deleting
         * all transitions associated with this WFA when it is deleted is
         * safe. However, because the weight of this transition is a
         * sem_elem_t (ref_ptr< SemElem >) modifying the weight from
         * the Trans & t will result in modification of the weight on
         * the interal Trans * object.
         *
         * @return bool true if transition matching (p,g,q) exists
         * @see Trans
         * @see SemElem
         * @see sem_elem_t
         */
        virtual bool find( 
            Key p,
            Key g,
            Key q,
            Trans & t ) const;

        /**
         * @brief invoke TransFunctor tf on each Trans
         */
        virtual void for_each( TransFunctor& tf );
        virtual void for_each( boost::function<void(ITrans * t)> &);

        /**
         * @brief invoke ConstTransFunctor on each Trans
         */
        virtual void for_each( ConstTransFunctor& tf ) const;
        virtual void for_each( boost::function<void(ITrans const * t)> &) const;

#ifdef WALI_WFA_FUNCTOR_FOR_EACH
        template<typename Functor>
        typename boost::disable_if<boost::is_base_of<TransFunctor, Functor> >::type
        for_each(Functor & func) {
            boost::function<void(ITrans * t)> wrapper(boost::ref(func));
            for_each(wrapper);
        }

        template<typename Functor>
        typename boost::disable_if<boost::is_base_of<TransFunctor, Functor> >::type
        for_each(Functor & func) const {
            boost::function<void(ITrans const * t)> wrapper(boost::ref(func));
            for_each(wrapper);
        }
#endif

        /**
         * Intersect this with parameter fa. This is a wrapper
         * for intersect( WeightMaker&,WFA& ) that passes
         * the WeightMaker KeepBoth.
         *
         * @see wali::wfa::WeightMaker
         * @see wali::wfa::KeepBoth
         */
        virtual WFA intersect( WFA const & fa ) const;

        /**
         * Intersect this with parameter fa. This is a wrapper
         * for intersect( WeightMaker&,WFA&,WFA& ) that passes
         * the WeightMaker KeepBoth. Result is stored in dest.
         *
         * @see wali::wfa::WeightMaker
         * @see wali::wfa::KeepBoth
         */
        virtual void intersect( WFA const & fa, WFA& dest ) const;

        /**
         * Intersect this with parameter fa and return the result
         * by value. The parameter WeightMaker determines how
         * intersection should join the weights on matching
         * transitions.
         *
         * @see wali::wfa::WeightMaker
         */
        WFA intersect( WeightMaker& wmaker , WFA const & fa ) const;

        /**
         * Intersect this with parameter fa. The result is stored in
         * parameter dest. Parameter dest is cleared before any
         * results are stored. The parameter WeightMaker determines
         * how intersection should join the weights on matching
         * transitions.
         *
         * NOTE: For now this means (dest != this) && (dest != fa).
         */
        virtual void intersect(WeightMaker& wmaker, WFA const & fa, WFA& dest) const;

        virtual void intersect_worklist(WeightMaker& wmaker, WFA const & fa, WFA& dest ) const;
        virtual void intersect_cross(WeightMaker& wmaker, WFA const & fa, WFA& dest ) const;

        /**
         * Performs path summary. Simply calls the path_summary with
         * the default Worklist provided by WALi.
         */
        virtual void path_summary();

        /**
         * Computes a regular expression for the automaton.
         * The regex, when evaluated, produces a weight
         * that is equal to calling path_summary and then
         * getting the weight on the initial state. I.e.,
         *
         * <code>
         *      fa.toRegex().solve(); 
         *    <==>
         *      fa.path_summary();
         *      fa.getState( fa.getInitialState() )->weight();
         *
         * </code>
         */
        virtual regex::regex_t toRegex();

        /**
         * Performs path summary with the specified Worklist
         */
        virtual void path_summary( Worklist<State>& wl );

        /**
         * Performs path summary. Simply calls the path_summary with
         * the default Worklist provided by WALi.
         * Initializes the weight on the final state to wt (can be
         * useful for efficiency in some cases)
         */
        virtual void path_summary(sem_elem_t wt);

        /**
         * Performs path summary using Tarjan's algorithm. This results
         * in the dual benefits of lazy witness evaluation and
         * transparent witness propagation.
         */
        // NOTE: This currently does not use FWPDS, and therefore does
        // not actually use Tarjan's algorithm. The framework is there,
        // such that a switch from WPDS to FWPDS is all that's needed.
        virtual void path_summary_tarjan();

        /**
         * Prunes the WFA. This removes any transitions that are
         * not in the (getInitialState(),F) chop.
         */
        virtual void prune();

        /**
         * Intersects the WFA with <init_state, (stk \Gamma^*)>
         * that essentially removes transitions and calls prune
         */
        virtual void filter(Key stk);

        /**
         * Intersects the WFA with { <init_state, (stk \Gamma^*)> | stk
         * \in stkset }
         * This essentially removes transitions and calls prune
         */
        virtual void filter(std::set<Key> &stkset);

        /**
         * For every state s \in st, rename it to s'. Then add
         * epsilon transition from s to s' and perform the
         * epsilon closure. This uses WFA::getGeneration() to
         * produce the renamed states.
         * Returns the result in "output"
         */
        virtual void duplicateStates(std::set<Key> &st, WFA &output) const;

        /**
         * Write the WFA to the std::ostream parameter, implements
         * Printable::print
         *
         * @param o the std::ostream this is written to
         * @return std::ostream WFA was written to
         *
         * @see Printable
         */
        virtual std::ostream & print( std::ostream & ) const;

        /**
         * @brief Print WFA in dot format
         */
        virtual std::ostream& print_dot(
            std::ostream& o,
            bool print_weights=false,
            DotAttributePrinter * attribute_printer=NULL) const;

        /**
         * @brief marshall WFA in XML 
         */
        virtual std::ostream& marshall( std::ostream& o ) const;

        virtual std::ostream& marshallState( std::ostream& o, Key key ) const;

        /**
         * @brief inserts tnew into the WFA
         *
         * This method inserts Trans * tnew into the WFA.  If a transition
         * told matching (p,g,q) of tnew already exists, then tnew is
         * combined into told.
         *
         * @warning: Note that insert assumes ownership of the passed
         *      in parameter tnew
         * @warning: insert does assumes the states of tnew
         *      are already in the WFA. If they are not path_summary
         *      and other methods might fail
         *
         * @return pointer to real transition
         */
        ITrans * insert( ITrans * tnew );

        /**
         * @brief Returns a TransSet containing all 
         * transitions matching (p,y,?) in the WFA
         *
         * @see TransSet
         */
        TransSet match( Key p, Key y) const;

        /** 
         * Create a State * for the key Key
         */
        void addState( Key key , sem_elem_t zero );

        /**
         * @return const pointer to underlying State object or NULL if
         * no such state exists.
         *
         * @note This may throw an exception in the future
         */
        const State* getState( Key name ) const;

        /**
         * @return pointer to underlying State object or NULL if
         * no such state exists.
         *
         * @param name the key for this State
         * @note This may throw an exception in the future
         *
         */
        State * getState( Key name );

        const std::set< Key >& getStates() const;

        const std::set< Key >& getFinalStates() const;

        /* TODO
           size_t size() const;
           */

        size_t numStates() const
        {
          return Q.size();
        }

        size_t numTransitions() const;

      protected:

        /**
         * setupFixpoint clears each states markable flag and sets
         * the states weight to zero
         */
        void setupFixpoint( Worklist<State>& wl, IncomingTransMap_t* trans, PredHash_t* preds );

        /**
         * setupFixpoint clears each states markable flag and sets
         * the states weight to zero, and weight of final states to
         * wtFinal (or ONE if NULL)
         */
        void setupFixpoint( Worklist<State>& wl, IncomingTransMap_t* trans, PredHash_t* preds, sem_elem_t wtFinal );

        virtual ITrans * find( 
            Key p,
            Key g,
            Key q);

        virtual ITrans const * find( 
            Key p,
            Key g,
            Key q) const;
        
        /**
         * Erases the specified Trans(from,stack,to) from the
         * kpmap and epsmap. A null return value means no transition existed.
         *
         * @return the Trans* that was removed from the maps
         */
        ITrans * eraseTransFromMaps(
            Key from,
            Key stack,
            Key to );
        /**
         * Erases the specified Trans(from,stack,to) from the
         * kpmap. A null return value means no transition existed.
         *
         * @return the Trans* that was removed from the KpMap
         */
        ITrans* eraseTransFromKpMap(
            Key from,
            Key stack,
            Key to );

        ITrans* eraseTransFromKpMap( ITrans* terase );

        /**
         * Erases the specified Trans(from,stack,to) from the
         * epsmap. A false return value means no transition existed.
         *
         * @return true if a transition was erased from EpsMap
         */
        ITrans* eraseTransFromEpsMap( ITrans* terase );

        /**
         * Erases the State 'state' from the WFA and all transitions
         * that go to or from the State.
         *
         * @return true
         */
        bool eraseState( State* state );

        /**
         * Performs path summary with the specified Worklist
         * Initializes the weight on the final state to wt (can be
         * useful for efficiency in some cases)
         */
        virtual void path_summary( Worklist<State>& wl, sem_elem_t wt );

        /**
         * Uses Tarjan's algorithm to build a regular expression
         * for this WFA. IIRC, it is the cubic dynamic programming
         * algorithm.
         */
        regex::regex_t TarjanBasicRegex();

      private:
        // Poststar builds backwards WFAs so 
        // extend needs to take this into account.
        regex::regex_t EXTEND(regex::regex_t a,regex::regex_t b);

        //
        // Variables
        //
      public:
      protected:
        kp_map_t kpmap;          //! < map from KeyPair to list of trans
        state_map_t state_map;   //! < map from key to State
        eps_map_t eps_map;       //! < map from "to state" to list of eps trans ending in "to state"
        Key init_state;          //! < initial state of WFA
        std::set< Key > F;       //! < set of final states
        std::set< Key > Q;       //! < set of all states
        query_t query;           //! < determine the extend order for path_summary
        size_t generation;       //! < Each WPDS query increments the generation count.
        progress_t progress;     //! < Provides indication of progress to client.

        std::set<State*> deleted_states;

        PathSummaryImplementation defaultPathSummaryImplementation;

      private:


      public:

        void setDefaultPathSummaryImplementation(PathSummaryImplementation i) {
            defaultPathSummaryImplementation = i;
        }
        PathSummaryImplementation getDefaultPathSummaryImplementation() const {
            return defaultPathSummaryImplementation;
        }

        /// Return the set of states reachable from 'start', along with the
        /// weights gathered by following those paths. Includes the start
        /// state, with weight (at least) one. (If there is an epsilon loop
        /// that involves the start state, then the net effect of that loop
        /// will be included as well.)
        AccessibleStateMap epsilonClose(Key start) const;

        /// Returns the same thing as epsilonClose(start). If 'start' is
        /// already present in 'cache',
        AccessibleStateMap epsilonCloseCached(Key start, EpsilonCloseCache & cache) const;

        // The following are specific variants. (epsilonClose() and epsilonCloseCached() each calls one of
        // these.)
        AccessibleStateMap epsilonClose_Mohri(Key start) const;
        AccessibleStateMap epsilonClose_Fwpds(Key start) const;

        AccessibleStateMap epsilonCloseCached_MohriDemand     (Key start, EpsilonCloseCache & cache) const;
        AccessibleStateMap epsilonCloseCached_FwpdsDemand     (Key start, EpsilonCloseCache & cache) const;
        AccessibleStateMap epsilonCloseCached_MohriAll        (Key start, EpsilonCloseCache & cache) const;
        AccessibleStateMap epsilonCloseCached_FwpdsAllSingles (Key start, EpsilonCloseCache & cache) const;
        AccessibleStateMap epsilonCloseCached_FwpdsAllMulti   (Key start, EpsilonCloseCache & cache) const;

        // This is a helper function used for both epsilonClose_Fwpds and
        // epsilonCloseCached_FwpdsAllMulti.
        EpsilonCloseCache genericFwpdsPoststar(std::set<Key> const & sources,
                                               boost::function<bool (ITrans const *)> trans_accept) const;

        EpsilonCloseCache
        genericWpdsPoststar(std::set<Key> const & sources,
                            boost::function<bool (ITrans const *)> trans_accept,
                            wpds::WPDS & wpds,
                            sem_elem_t query_weight,
                            sem_elem_t state_weight) const;
        

        /// For each state q, compute the set of all possible weights w such
        /// that there is a path from the initial state to q with weight w.
        //
        // This function is in WFA-eclose.cpp due to its similarity with the
        // Tarjar/FWPDS epsilon closure algorithm.
        AccessibleStateSetMap computeAllReachingWeights() const;
        AccessibleStateSetMap computeAllReachingWeights(domains::SemElemSet::SemElemSubsumptionComputer,
                                                        bool include_zeroes) const;

        /// Creates (and returns) a new WFA which is the same as *this,
        /// except that it has no epsilon transitions.
        ///
        /// The combine-over-all-paths solution for every pair of states has
        /// the same value in this and the original.
        WFA removeEpsilons() const;

        /// Starting from the states in 'start' (with the given weight),
        /// simulate running the word 'word'. Return the list of accessible
        /// states, and the weights with which they can be accessed.
        ///
        /// This may assume there are no epsilon loops along the path
        /// encountered by 'word' -- but that may be fixed now. Check the
        /// tests if you need this feature.
        AccessibleStateMap simulate(AccessibleStateMap const & start,
                                    Word const & word) const;

        /// Returns whether the given string is accepted with a non-zero
        /// weight
        bool isAcceptedWithNonzeroWeight(Word const & word) const;


        /// Performs the subset construction and returns the resulting
        /// WFA. (Does not mutate.) Completes the automaton when done.
        WFA determinize() const;
        WFA determinize(DeterminizeWeightGen const & weight_gen) const;

        /// Performs the subset construction and returns the resulting
        /// WFA. (Does not mutate.) Is likely to return a WFA with a
        /// non-total transition function.
        WFA semideterminize() const;
        WFA semideterminize(DeterminizeWeightGen const & weight_gen) const;
        
        /// Returns whether this WFA is isomorphic to the given WFA; that is,
        /// the two automata are equal up to a relabeling of the states. (Or,
        /// expressed another way, the transition graphs are isomorphic with
        /// the extra conditions that (1) start and final states must
        /// correspond, (2) state weights must be the same, (3) edge labels
        /// (symbols) must correspond, and (4) edge weights must be the
        /// same.) (TODO: accepting weights are not checked.)
        bool isIsomorphicTo(WFA const & other) const;

        /// Same as isIsomorphicTo(WFA const & other) const, except that
        /// conditions (2) and (4) (those pertaining to the weights on states
        /// and transitions) are lifted if 'check_weights' is false. (TODO:
        /// accepting weights are not checked.)
        bool isIsomorphicTo(WFA const & other, bool check_weights) const;

        /// Returns whether the two automata are the same. That is they have
        /// the same states, transitions, state weights, transition weights,
        /// starting states, and accepting states. (TODO: accepting state
        /// weights are not checked.) IGNORES the query direction and
        /// generation count.
        bool equal(WFA const & other) const;

        /// Adds transitions so that every state has an outgoing for every
        /// symbol. The overloads allow specification of:
        ///
        /// - What you mean by "every symbol". By default, this is every
        ///   symbol that appears in any transition in the WFA. If you
        ///   specify your own set, no attempt is made to ensure that your
        ///   set is a superset of the default, so there may be symbols for
        ///   which there are transitions out of some states but not others.
        ///
        /// - What the destination state should be. By default, this is
        ///   getKey({}), where {} is an empty std::set<Key>, whether or not
        ///   that state is already in the WFA. (In other words, if the WFA
        ///   already has a sink state, but it's different, then you have to
        ///   find and specify that yourself.) If that state is already there
        ///   but is not a sink state, then the code asserts.
        void complete(std::set<Key> const & symbols, Key sink_state);
        void complete(std::set<Key> const & symbols);
        void complete(Key sink_state);
        void complete();

        /// Sets F = Q - F.
        ///
        /// THIS IS NOT THE SAME AS COMPLEMENTING THE LANGUAGE: if the
        /// machine is either nondeterministic or incomplete (has a non-total
        /// transition function), then things can be different.
        ///
        /// This is intended to be used after calling either determinize() or
        /// both semideterminize() then complete() [both have multiple
        /// overloads]; the reason there is no 'complementLanguage()'
        /// function is that the present design encourages you to explicitly
        /// think about what you are complementing with respect to. This is
        /// made especially acute because of the fact that WFA does not
        /// explictly store its alphabet, so it is not possible to have a
        /// symbol that's part of a WFA's alphabet and does not appear on a
        /// transition (even if you want to think of the WFA as
        /// incomplete/nondeterministic).
        void complementStates();
        

        static
        bool
        is_isomorphism(WFA const & left, std::vector<Key> const & left_states,
                       WFA const & right, std::vector<Key> const & right_states,
                       bool check_weights);

        
        static
        std::map<Key, std::map<Key, std::set<Key> > >
        next_states_no_eclose(WFA const & wfa, std::set<Key> const & froms);

        //// Prints to 'os' statistics about this WFA. 
        void printStatistics(std::ostream & os) const;


        /// "Converts" the automaton to a WPDS.
        ///
        /// The WPDS has a single state (given as a function parameter). WFA
        /// states become WPDS stack symbols. Each transition in the WFA
        /// becomes a rule in the WPDS.
        ///
        /// If delta(q, a) = q', then <p, q> -> <p, q'> becomes a rule
        ///
        /// The WPDS must be passed in so that we don't have to guess what
        /// kind of WPDS to make (WPDS, EWPDS, FWPDS, etc.) The WPDS will
        /// *not* be cleared before anything is added.
        ///
        /// The given callback function is called for each transition in the
        /// NWA; a rule for that transition is added iff the callback returns
        /// true. This callback may be set to empty.
        // %%% Review above paragraph.
        void toWpds(Key p_state,
                    wpds::WPDS * wpds,
                    boost::function<bool (ITrans const *)> trans_accept,
                    bool reverse=false) const;

        /// Returns the set of symbols that appear on a transition somewhere
        /// in this WFA. Never includes epsilon.
        ///
        /// (Note that it is impossible to have an element of the alphabet
        /// that does not appear as a transition due to this fact -- the
        /// alphabet is never stored explicitly, and this function will
        /// recompute it each time.)
        std::set<Key> alphabet() const;


        /// Returns the outgoing TransSet for the given state and symbol. If
        /// there aren't any transitions, returns NULL.
        ///
        /// THIS RETURNS NULL EVEN IF THERE IS AN EMPTY TRANSSET. (This
        /// allows the user to know if there are transitions just by checking
        /// the size.)
        TransSet const *
        outgoingTransSet(Key state, Key symbol) const;
        TransSet *
        outgoingTransSet(Key state, Key symbol);

        /// Returns the state and weight to that state which is at the end of
        /// a chain of single epsilon transitions.
        ///
        /// That is, if there are eps transitions from A to B to C to D and
        /// you call endOfEpsilonChain(A), it will return D and the weight
        /// from A to D. However, if B has another outgoing transition to E,
        /// it will only give you B and the weight to B.
        ///
        /// If there are no outgoing epsilon transitions or there are
        /// multiple transitions outgoing from starting_state, returns
        /// <starting_state, 1>.
        std::pair<Key, sem_elem_t>
        endOfEpsilonChain(Key starting_state) const;

        void
        collapseTransitionsForwardFrom(Key state);

        void
        removeStatesWithInDegree0();

        void
        collapseEpsilonChains();
    };

  } // namespace wfa

} // namespace wali

#endif  // wali_wfa_WFA_GUARD

