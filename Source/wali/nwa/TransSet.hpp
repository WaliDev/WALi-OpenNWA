#ifndef wali_nwa_TransSet_GUARD
#define wali_nwa_TransSet_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/nwa/StateSet.hpp"
#include "wali/nwa/TransInfo.hpp"

// std::c++
#include <iostream>
#include <set>
#include <assert.h>

namespace wali
{
  namespace nwa
  {
    /**
     *
     * This class is used to keep track of the transitions of an NWA.
     *
     */
#ifndef KEY
    template <typename St, typename Sym, typename Call, typename Internal, typename Return>
    class TransSet : public Printable
    {
      public:        
        typedef std::set< Call > Calls;
        typedef std::set< Internal > Internals;
        typedef std::set< Return > Returns;
        
        typedef typename Calls::const_iterator callIterator;
        typedef typename Internals::const_iterator internalIterator;
        typedef typename Returns::const_iterator returnIterator;
        
        typedef TransInfo<St,Call,Internal,Return> Info;
      
      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      TransSet( );
      TransSet( const TransSet & other );
      TransSet & operator=( const TransSet & other );

      ~TransSet( );

      //Transition Accessors

      /**
       *  TODO: use refptr<St>  
       * @brief finds the symbol on any transition whose source is 'from' and
       *        whose target is 'to'
       *
       * This method return the symbol on any transition whose source is 'from'
       * and whose target is 'to'.
       *
       * @param - from: the source of the transition
       * @param - to: the target of the transition
       * @return true if there exists some transition with the given source and 
       *         target, false otherwise
       *
       */
      bool getSymbol( const St * from, const St * to, Sym & sym );

      /**
       *  TODO: use refptr<St> 
       * @brief tests whether there exists a transition whose source is 'from',
       *        whose symbol is 'sym', and whose target is 'to'
       *
       * This method determines whether there exists a transition (internal, call,
       * or return) whose source is 'from', whose symbol is 'sym', and whose target
       * is 'to'.
       *
       * @param - from: the source of the transition
       * @param - sym: the symbol that labels the transition
       * @param - to: the target of the transition
       * @return true if such a transition exists, false otherwise
       * 
       */
      bool findTrans( const St * from, const Sym & sym, const St * to ) const;

      /**
       *  TODO: use refptr<St> 
       * @brief returns all return sites that correspond with the given call site
       *
       * This method returns the set of all return sites that correspond with the
       * given call site.
       *
       * @param - callSite: the call site whose return sites to look for
       * @return the set of all return sites that correspond with the given call site
       *
       */
      std::set<St> getReturnSites( const St * callSite ) const;

      /**
       *  TODO: use refptr<St> 
       * @brief returns all return sites that correspond with the given exit and call site
       *
       * This method returns the set of all return sites that correspond with the
       * given exit and call site.
       *
       * @param - exit: the exit whose return sites to look for
       * @param - callSite: the call site whose return sites to look for
       * @return the set of all return sites that correspond with the given exit and call site
       *
       */
      const std::set<Return> getReturns( const St * exit, const St * callSite ) const;

      /**
       *  TODO: use refptr<St> 
       * @brief returns all call sites that correspond with the given exit - return site pair
       *
       * This method returns the set of all call sites that correspond with the given
       * exit - return site pair.
       *
       * @param - exitSite: the exit of the pair whose call sites to look for
       * @param = returnSite: the return site of the pair whose call sites to look for
       * @return the set of all call sites that correspond with the exit - return site pair
       *
       */
      std::set<St> getCallSites( const St * exitSite, const St * returnSite ) const;

      /**
       *  TODO: use refptr<St> 
       * @brief returns all entry sites that correspond with the given call site
       *
       * This method returns the set of all entry sites that correspond with the
       * given call site.
       *
       * @param - callSite: the call site whose entry sites to look for
       * @return the set of all entry sites that correspond with the given call site
       *
       */
      const std::set<Call> getEntries( const St * callSite ) const;

      /**
       *  TODO: use refptr<St> 
       * @brief returns all targets that correspond with the given source 
       *
       * This method returns the set of all targets that correspond with the 
       * given source .
       *
       * @param - source: the source whose targets to look for
       * @return the set of all targets that correspond with the given source 
       *
       */
      const std::set<Internal> getTargets( const St * source ) const;

      /**
       *  TODO: use refptr<St>  
       * @brief creates transitions for 'dup' mirroring 'orig' outgoing transitions
       *
       * This method creates a transition using 'dup' in the place of 'orig' for
       * each outgoing transition involving 'orig'.  Do not duplicate selfloops.
       *
       * @param - orig: the state that is being duplicated
       * @param - dup: the state that is duplicating 'orig'
       *  
       */
      void dupTransOutgoing( const St * orig, const St * dup );

      /**
       *  TODO: use refptr<St>  
       * @brief creates transitions for 'dup' mirroring 'orig' transitions
       *
       * This method creates a transition using 'dup' in the place of 'orig' for
       * each transition involving 'orig'.  In addition, for any transition involving a 
       * self-loop on 'orig', two(three in the case of a return transition with exit, 
       * pred, and ret all 'orig') transitions are added (using 'dup' for 'orig' in each 
       * position while retaining 'orig' in all other positions).
       *
       * @param - orig: the state that is being duplicated
       * @param - dup: the state that is duplicating 'orig'
       *  
       */
      void dupTrans( const St * orig, const St * dup );

      /**
       *
       * @brief removes all transitions from this collection of transitions
       *
       * This method removes all transitions from this collection of
       * transitions. 
       *
       */
      void clear( );
      
      /**
       *
       * @brief get all call transitions in this collection of transitions 
       *
       * This method provides access to all call transitions in this collection of
       * transitions.  
       *
       * @return all call transitions in this collection of transitions
       *
       */
      const Calls & getCalls() const;
        
      /**
       *
       * @brief get all internal transitions in this collection of transitions 
       *
       * This method provides access to all internal transitions in this collection 
       * of transitions.  
       *
       * @return all internal transitions in the collection of transitions 
       *
       */
      const Internals & getInternals() const;
       
      /**
       *
       * @brief get all return transitions in the collection of transitions 
       *
       * This method provides access to all return transitions in the collection 
       * of transitions associated with the NWA.  
       *
       * @return all return transitions in the collection of transitions 
       *
       */
      const Returns & getReturns() const;
      
      /**
       *
       * @brief add the given call transition to the collection of transitions
       *
       * This method adds the given call transition to this collection of 
       * transitions.  If this exact call transition already exists, false is 
       * returned. Otherwise, true is returned.
       *
       * @param - addTrans: the call transition to add to the collection of transitions
       * @return false if the call transition already exists in the collection
       *
       */
      bool addCall( const Call & addTrans );
        
      /**
       *
       * @brief add the given internal transition to the collection of transitions
       *
       * This method adds the given internal transition to the collection of 
       * transitions.  If this exact internal transition already exists, false is 
       * returned. Otherwise, true is returned.
       *
       * @param - addTrans: the internal transition to add to the collection of transitions
       * @return false if the internal transition already exists in the collection
       *
       */
      bool addInternal( const Internal & addTrans );
        
      /**
       *
       * @brief add the given return transition to the collection of transitions
       *
       * This method adds the given return transition to the collection of 
       * transitions.  If this exact return transition already exists, false is 
       * returned. Otherwise, true is returned.
       *
       * @param - addTrans: the return transition to add to the collection of transitions
       * @return false if the return transition already exists in the collection
       *
       */
      bool addReturn( const Return & addTrans );
      
      /**
       *
       * @brief add all transitions in the given collection of transitions to this
       *        collection of transitions
       *
       * This method adds all of the transitions in the given collection of
       * transitions to this collection of transitions.
       *
       * @param - addTransSet: the collection of transitions to add to this 
       *          collection of transitions
       *
       */
      void addAllTrans( TransSet addTransSet );
      
      /**
       *
       * @brief remove the given call transition from this collection of 
       *        transitions
       *
       * This method removes the given call transition from this collection 
       * of transitions.  If the given call transition does not exust in
       * this collection of transitions false is returned.  Otherwise,
       * true is returned.
       *
       * @param - removeTrans: the call transition to remove from the collection
       * @return false if the given call transition does not exist in the 
       *          collection, true otherwise.
       *
       */
      bool removeCall( const Call & removeTrans );
        
      /**
       *
       * @brief remove the given internal transition from the collection of 
       *        transitions
       *
       * This method removes the given internal transition from the collection 
       * of transitions.  If the given internal transition does not exist in
       * this collection of transitions false is returned.  Otherwise,
       * true is returned.
       *
       * @param - removeTrans: the internal transition to remove from the collection
       * @return false if the given internal transition does not exist in the 
       *          collection, true otherwise.
       *
       */
      bool removeInternal( const Internal & removeTrans );
        
      /**
       *
       * @brief remove the given return transition from the collection of 
       *        transitions
       *
       * This method removes the given return transition from the collection 
       * of transitions.  If the given return transition does not exist in 
       * this collection of transitions false is returned.  Otherwise, true 
       * is returned.
       *
       * @param - removeTrans: the return transition to remove from the collection
       * @return false if the given return transition does not exist in the 
       *          collection, true otherwise.
       *
       */
      bool removeReturn( const Return & removeTrans );
                      
      /**
       *
       * @brief test whether the given call transition is in this collection of 
       *        transitions 
       *
       * This method tests whether the given call transition is in this collection
       * of transitions.
       *
       * @param - trans: the call transition to check
       * @return true if the given call transition is in this collection of 
       *          transitions
       *
       */
      bool isCall( const Call & trans ) const;
        
      /**
       *
       * @brief test whether the given internal transition is in this collection of 
       *        transitions 
       *
       * This method tests whether the given internal transition is in this
       * collection of transitions.
       *
       * @param - trans: the internal transition to check
       * @return true if the given internal transition is in this collection of 
       *          transitions associated with the NWA
       *
       */
      bool isInternal( const Internal & trans ) const;
       
      /**
       *
       * @brief test whether the given return transition is in this collection of 
       *        transitions 
       *
       * This method tests whether the given return transition is in this
       * collection of transitions.
       *
       * @param - trans: the return transition to check
       * @return true if the given return transition is in this collection of 
       *          transitions
       *
       */
      bool isReturn( const Return & trans ) const;                   
                    
                      
      //Utilities	

      /**
       *
       * @brief print the collection of transitions
       *
       * This method prints out the transition set to the output stream 
       * provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print( std::ostream & o ) const;

      /*
       * 
       * @brief print the collection of transitions in dot format
       *
       * This method prints out the transition set in dot format to the
       * output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print_dot( std::ostream & o ) const;

      /**
       *
       * @brief tests whether this collection of transitions is equivalent 
       *        to the collection of transitions 'other'
       *
       * This method tests the equivalence of this set of transitions and 
       * the set of transitions 'other'.
       *
       * @param - other: the TransSet to compare this TransSet to
       * @return true if this TransSet is equivalent to the TransSet 'other'
       *
       */
      bool operator==( const TransSet & other ) const;

      /**
       *
       * @brief provides access to the call transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the call transitions in the collection
       * of transitions through an iterator.
       *
       * @return the starting point of an iterator through the call transitions
       *          in the collection of transitions
       *
       */
      callIterator beginCall( ) const;
        
      /**
       *
       * @brief provides access to the internal transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the internal transitions in the collection
       * of transitions through an iterator.
       *
       * @return the starting point of an iterator through the internal transitions
       *          in the collection of transitions
       *
       */
      internalIterator beginInternal( ) const;
       
      /**
       *
       * @brief provides access to the return transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the return transitions in the collection
       * of transitions through an iterator.
       *
       * @return the starting point of an iterator through the return transitions
       *          in the collection of transitions
       *
       */
      returnIterator beginReturn( ) const;
      
      /**
       *
       * @brief provides access to the call transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the call transitions in the collection
       * of transitions through an iterator.
       *
       * @return the exit point of an iterator through the call transitions in
       *          the collection of transitions
       *
       */
      callIterator endCall( ) const;
         
      /**
       *
       * @brief provides access to the internal transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the internal transitions in the collection
       * of transitions through an iterator.
       *
       * @return the exit point of an iterator through the internal transitions in
       *          the collection of transitions
       *
       */
      internalIterator endInternal( ) const;
         
      /**
       *
       * @brief provides access to the return transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the return transitions in the collection
       * of transitions through an iterator.
       *
       * @return the exit point of an iterator through the return transitions in
       *          the collection of transitions
       *
       */
      returnIterator endReturn( ) const;

      /**
       *
       * @brief returns the number of call transitions in this collection of
       *        transitions
       *
       * This method returns the number of call transitions in the collection
       * of transitions associated with the NWA.  
       *
       * @return the number of call transitions in this collection of transitions
       *
       */
      size_t sizeCall( ) const;
        
      /**
       *
       * @brief returns the number of internal transitions in this collection of
       *        transitions 
       *
       * This method returns the number of internal transitions in this collection
       * of transitions.  
       *
       * @return the number of internal transitions in this collection of transitions
       *
       */
      size_t sizeInternal( ) const;
        
      /**
       *
       * @brief returns the number of return transitions in this collection of
       *        transitions
       *
       * This method returns the number of return transitions in this collection
       * of transitions associated with the NWA.  
       *
       * @return the number of return transitions in this collection of transitions
       *
       */
      size_t sizeReturn( ) const;
        
      /**
       *
       * @brief returns the total number of transitions (call, internal, and return) in 
       * this collection of transitions
       *
       * This method returns the total number of transitions (call, internal, and return) 
       * in this collection of transitions.  
       *
       * @return the total number of transitions in this collection of transitions
       *
       */
      size_t size( ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief returns all outgoing internal transitions for the given state
       *
       * This method returns all internal transitions with the given state as
       * the source.
       *
       * @param - state: the source state
       * @return the set of all outgoing internal transitions for the given state
       *
       */
      std::set<Internal> getTransFrom( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief retuans all incoming internal transitions for the given state
       *
       * This method returns all internal transitions with the given state as
       * the target.
       *
       * @param - state: the target state
       * @return the set of all incoming internal transitions for the given state
       *
       */
      std::set<Internal> getTransTo( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief returns all call transitions for the given call site
       *
       * This method returns all call transitions with the given state as
       * the call site.
       *
       * @param - state: the call site
       * @return the set of all call transitions for the given call site
       *
       */
      std::set<Call> getTransCall( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief returns all call transitions for the given entry point
       *
       * This method returns all call transitions with the given state as
       * the entry point.
       *
       * @param - state: the entry point
       * @return the set of all call transitions for the given entry point
       *
       */
      std::set<Call> getTransEntry( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief returns all return transitions for the given exit point
       *
       * This method returns all return transitions with the given state as
       * the exit point.
       * 
       * @param - state: the exit point
       * @return the set of all return transitions for the given exit point
       *
       */
      std::set<Return> getTransExit( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief returns all return transitions for the given call predecessor
       *
       * This method returns all return transitions with the given state as
       * the call predecessor.
       * 
       * @param - state: the call predecessor
       * @return the set of all return transitions for the given call predecessor
       *
       */
      std::set<Return> getTransPred( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief returns all return transitions for the given return site
       *
       * This method returns all return transitions with the given state as
       * the return site.
       *
       * @param - state: the return site
       * @return the set of all return transitions for the given return site
       *
       */
      std::set<Return> getTransRet( const St * state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the source of any internal 
       *        transition
       *
       * This method determines whether the given state is the source of any 
       * internal transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the source of some internal 
       *          transition, false otherwise
       *
       */
      bool isFrom( const St & state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the target of any internal
       *        transition
       *
       * This method determines whether the given state is the target of any
       * internal transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the target of some internal
       *          transition, false otherwise
       *
       */
      bool isTo( const St & state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the call site of any call 
       *        transition
       *
       * This method determines whether the given state is the call site of 
       * any call transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the call site of some call
       *          transition, false otherwise
       *
       */
      bool isCall( const St & state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the entry point of any call
       *        transition
       *
       * This method determines whether the given state is the entry point
       * of any call transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the entry point of some call 
       *          transition, false otherwise
       *
       */
      bool isEntry( const St & state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the exit point of any return
       *        transition
       *
       * This method determines whether the given state is the exit point of 
       * any return transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the exit point of some return
       *          transition, false otherwise
       *
       */
      bool isExit( const St & state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the call predecessor for any
       *        return transition
       *
       * This methos determines whether the given state is the call predecessor
       * for any return transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the call predecessor of some return
       *          transition, false otherwise
       *
       */
      bool isPred( const St & state ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief tests whether the given state is the return site of any return
       *        transition
       *
       * This method determines whether the given state is the return site of any
       * return transition.
       * 
       * @param - state: the state to test
       * @return true if the given state is the return site of some return 
       *          transition, false otherwise
       *
       */
      bool isRet( const St & state ) const;

      /** 
       *  TODO: use refptr<St> 
       * @brief removes all transitions involving the given state  
       *
       * This method removes all transitions involving the given state 
       * If no transitions exist involving this state false is returned.  
       * Otherwise, true is returned.
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeTransWith( const St * state );   
      
      /** 
       *
       * @brief removes all call transitions with the given symbol 
       *
       * This method removes all call transitions with the given symbol. 
       * If no call transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeCallTransSym( const Sym & sym );
        
      /** 
       *
       * @brief removes all internal transitions with the given symbol 
       *
       * This method removes all internal transitions with the given symbol. 
       * If no internal transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeInternalTransSym( const Sym & sym );
        
      /** 
       *
       * @brief removes all return transitions with the given symbol 
       *
       * This method removes all return transitions with the given symbol. 
       * If no return transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeReturnTransSym( const Sym & sym );
      
      /**
       *  TODO: use refptr<St> 
       * @brief test if there exists a call transition with the given from state 
       *        and symbol in this collection of transitions 
       *
       * This method tests whether there exists a call transition with the given 
       * from state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the call transition
       * @param - sym: the desired symbol for the call transition
       * @return true if there exists a call transition with the given from state and
       *          symbol in this collection of transitions 
       *
       */
      bool callExists( St * from, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief provides access to all call transitions with the given from state
       *        and symbol in this collection of transitions
       *
       * This method provides access to all call transitions with the given from
       * state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the call transitions
       * @param - sym: the desired symbol for the call transitions
       * @return the set of call transitions with the given from state and symbol
       *
       */
      const Calls getCalls( St * from, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St> 
       * @brief test if there exists an internal transition with the given from state 
       *        and symbol in this collection of transitions 
       *
       * This method tests whether there exists an internal transition with the given 
       * from state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the internal transition
       * @param - sym: the desired symbol for the internal transition
       * @return true if there exists an internal transition with the given from state and
       *          symbol in this collection of transitions
       *
       */
      bool internalExists( St * from, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St> 
       * @brief provides access to all internal transitions with the given from 
       *        state and symbol in this collection of transitions
       *
       * This method provides access to all internal transitions with the given
       * from state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the internal transitions
       * @param - sym: the desired symbol for the internal transitions
       * @return the set of internal transitions with the given from state and symbol
       *
       */
      const Internals getInternals( St * from, Sym * sym ) const;

      /**
       *  TODO this is redundant
       * @brief provides access to all internal transitions with the given 
       *        from state in this collection of transitions
       *
       * This method provides access to all internal transitions with the given
       * from state in this collection of transitions.
       *
       * @param - from: the desired from state for the internal transitions
       * @return the set of internal transitions with the given from state
       *
       */
      const Internals getInternalsFrom( St * from ) const;
        
      /**
       *  TODO: use refptr<St> 
       * @brief test if there exists a return transition with the given from state, 
       *        predecessor state, and symbol in this collection of transitions 
       *
       * This method tests whether there exists a return transition with the given 
       * from state, predecessor state, and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the return transition
       * @param - pred: the desired predecessor state for the return transition
       * @param - sym: the desired symbol for the return transition
       * @return true if there exists a return transition with the given from state and
       *          symbol in this collection of transitions
       *
       */
      bool returnExists( St * from, St * pred, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief provides access to all return transitions with the given from
       *        state and symbol in this collection of transitions
       *
       * This method provides access to all return transitions with the given from
       * state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the return transitions
       * @param - sym: the desired symbol for the return transitions
       * @return the set of return transitions with the given from state and symbol
       *
       */
      const Returns getReturns( St * from, Sym * sym ) const;

      protected:
        /** 
       *  TODO: use refptr<St> 
       * @brief removes all call transitions to or from the given state  
       *
       * This method removes all call transitions to or from the given state 
       * If no call transitions exist to or from this state false is returned.  
       * Otherwise, true is returned.
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeCallTransWith( const St * state );        
      
      /** 
       *  TODO: use refptr<St> 
       * @brief removes all internal transitions to or from the given state
       *
       * This method removes all internal transitions to or from the given
       * state.  If no internal transitions exist to or from this state, 
       * false is returned.  Otherwise, true is returned. 
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeInternalTransWith( const St * state );
      
      /** 
       *  TODO: use refptr<St> 
       * @brief removes all return transitions to or from the given state
       *        as well as return transitions corresponding to calls from
       *        that state
       *
       * This method removes all return transitions to or from the given 
       * state as well as return transitions corresponding to calls from
       * that state.  If no return transitions exist to or from this state,
       * or having this state as a call predecessor, false is returned.  
       * Otherwise, true is returned.
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeReturnTransWith( const St * state );    

      //
      // Variables
      //
      
      protected: 
       
      Calls callTrans;
      Internals internalTrans;
      Returns returnTrans;
        
      Info T_info;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return>::TransSet( )
    {
      callTrans = Calls();
      internalTrans = Internals();
      returnTrans = Returns();  
      
      T_info = Info();
    }
    
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return>::TransSet( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
    }
    
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return> & TransSet<St,Sym,Call,Internal,Return>::operator=( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      if (this == &other)     
        return *this;

      clear();
      T_info.clearMaps();
        
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
      return *this;
    }
   
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return>::~TransSet( ) 
    {
      clear();
      T_info.clearMaps(); 
    }

    //Transition Accessors

    /**
     *  TODO: use refptr<St>  
     * @brief finds the symbol on any transition whose source is 'from' and
     *        whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - to: the target of the transition
     * @return true if there exists some transition with the given source and 
     *         target, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::getSymbol( const St * fromSt, const St * toSt, Sym & sym )
    {
      //Check internal transitions.
      const Info::Internals from = T_info.fromTrans(fromSt);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt->getName() == it->third )
        {
          sym = it->second;
          return true;
        }
      }
      //Check call transitions.
      const Info::Calls call = T_info.callTrans(fromSt);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt->getName() == it->third )
        {
          sym = it->second;
          return true;
        }
      }
      //Check return transitions.      
      const Info::Returns exit = T_info.exitTrans(fromSt);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt->getName() == it->fourth )
        {
          sym = it->third;
          return true;
        }
      }
      //TODO: Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt->getName() == it->fourth )
        {
          sym = it->third;
          return true;
        }
      }*/
      
      return false;
    }

    /**
     *  TODO: use refptr<St> 
     * @brief tests whether there exists a transition whose source is 'from',
     *        whose symbol is 'sym', and whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - sym: the symbol that labels the transition
     * @param - to: the target of the transition
     * @return true if such a transition exists, false otherwise
     * 
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::findTrans( const St * fromSt, const Sym & sym, const St * toSt ) const
    {
      //Check internal transitions.
      const Info::Internals from = T_info.fromTrans(fromSt);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt->getName() == it->third && sym==it->second)
          return true;
      }
      //Check call transitions.
      const Info::Calls call = T_info.callTrans(fromSt);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt->getName() == it->third && sym == it->second)
        {
          return true;
        }
      }
      //Check return transitions.      
      const Info::Returns exit = T_info.exitTrans(fromSt);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt->getName() == it->fourth && sym == it->third)
        {
          return true;
        }
      }
      //TODO: Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt->getName() == it->fourth )
        {
          sym = it->third;
          return true;
        }
      }*/
      
      return false;
    }

    /**
     *  TODO: use refptr<St> 
     * @brief returns all return sites that correspond with the given call site
     *
     * @param - callSite: the call site whose return sites to look for
     * @return the set of all return sites that correspond with the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<St> TransSet<St,Sym,Call,Internal,Return>::getReturnSites( const St * callSite ) const
    {
      std::set<St> returns;
      const Info::Returns pred = T_info.predTrans(callSite);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        returns.insert(it->fourth);
      }
      return returns;
    }

    /**
     *  TODO: use refptr<St> 
     * @brief returns all return sites that correspond with the given exit and call site
     *
     * @param - exit: the exit whose return sites to look for
     * @param - callSite: the call site whose return sites to look for
     * @return the set of all return sites that correspond with the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const std::set<typename Return> TransSet<St,Sym,Call,Internal,Return>::getReturns( const St * exit, const St * callSite ) const
    {
      std::set<Return> returns;
      const Info::Returns pred = T_info.predTrans(callSite);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( it->first == exit->getName() )
          returns.insert(*it);
      }
      return returns;
    }

    /**
     *  TODO: use refptr<St> 
     * @brief returns all call sites that correspond with the given exit - return site pair
     *
     * @param - exitSite: the exit of the pair whose call sites to look for
     * @param = returnSite: the return site of the pair whose call sites to look for
     * @return the set of all call sites that correspond with the exit - return site pair
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<St> TransSet<St,Sym,Call,Internal,Return>::getCallSites( const St * exitSite, const St * returnSite ) const
    {
      std::set<St> calls;
      const Info::Returns exit = T_info.exitTrans(exitSite);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( it->fourth == returnSite->getName() )
          calls.insert(it->second);
      }
      return calls;
    }

    /**
     *  TODO: use refptr<St> 
     * @brief returns all entry sites that correspond with the given call site
     *
     * @param - callSite: the call site whose entry sites to look for
     * @return the set of all entry sites that correspond with the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const std::set<typename Call> TransSet<St,Sym,Call,Internal,Return>::getEntries( const St * callSite ) const
    {
      std::set<Call> entries;
      const Info::Calls cll = T_info.callTrans(callSite);
      for( Info::Calls::const_iterator it = cll.begin(); it != cll.end(); it++ )
      {
        entries.insert(*it);
      }
      return entries;
    }

    /**
     *  TODO: use refptr<St> 
     * @brief returns all targets that correspond with the given source 
     *
     * @param - source: the source whose targets to look for
     * @return the set of all targets that correspond with the given source 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const std::set<typename Internal> TransSet<St,Sym,Call,Internal,Return>::getTargets( const St * source ) const
    {
      std::set<Internal> targets;
      const Info::Internals src = T_info.fromTrans(source);
      for( Info::Internals::const_iterator it = src.begin(); it != src.end(); it++ )
      {
        targets.insert(*it);
      }
      return targets;
    }

    /**
     *  TODO: use refptr<St>  
     * @brief creates transitions for 'dup' mirroring 'orig' outgoing transitions
     *
     * @param - orig: the state that is being duplicated
     * @param - dup: the state that is duplicating 'orig'
     *  
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::dupTransOutgoing( const St * orig, const St * dup )
    { 
      //Duplicate outgoing internal transitions.
      const Info::Internals from = T_info.fromTrans(orig);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans = Internal(dup->getName(),it->second,it->third);
        addInternal(iTrans);

      }
      //Duplicate call site call transitions.
      const Info::Calls call = T_info.callTrans(orig);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans = Call(dup->getName(),it->second,it->third);
        addCall(cTrans);

      }
      //Duplicate exit point return transitions.
      const Info::Returns exit = T_info.exitTrans(orig);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans = Return(dup->getName(),it->second,it->third,it->fourth);
        addReturn(rTrans);
      }
      //Duplicate call predecessor return transitions.
      const Info::Returns pred = T_info.predTrans(orig);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans = Return(it->first,dup->getName(),it->third,it->fourth);
        addReturn(rTrans);
      }
    }

    /**
     *  TODO: use refptr<St>  
     * @brief creates transitions for 'dup' mirroring 'orig' transitions
     *
     * @param - orig: the state that is being duplicated
     * @param - dup: the state that is duplicating 'orig'
     *  
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::dupTrans( const St * orig, const St * dup )
    { 
      //Duplicate outgoing internal transitions.
      const Info::Internals from = T_info.fromTrans(orig);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans = Internal(dup->getName(),it->second,it->third);
        addInternal(iTrans);
        if( orig->getName() == it->third )   //Handle self-loops.
        {
          iTrans = Internal(dup->getName(),it->second,dup->getName());
          addInternal(iTrans);
        }
      }
      //Duplicate incoming internal transitions.
      const Info::Internals to = T_info.toTrans(orig);
      for( Info::Internals::const_iterator it = to.begin(); it != to.end(); it++ )
      {
        Internal iTrans = Internal(it->first,it->second,dup->getName());
        addInternal(iTrans);
      }

      //Duplicate call site call transitions.
      const Info::Calls call = T_info.callTrans(orig);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans = Call(dup->getName(),it->second,it->third);
        addCall(cTrans);
        if( orig->getName() == it->third )   //Handle self-loops.
        {
          cTrans = Call(dup->getName(),it->second,dup->getName());
          addCall(cTrans);
        }
      }
      //Duplicate entry point call transitions.
      const Info::Calls entry = T_info.entryTrans(orig);
      for( Info::Calls::const_iterator it = entry.begin(); it != entry.end(); it++ )
      {
        Call cTrans = Call(it->first,it->second,dup->getName());
        addCall(cTrans);
      }
      //Duplicate exit point return transitions.
      const Info::Returns exit = T_info.exitTrans(orig);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans = Return(dup->getName(),it->second,it->third,it->fourth);
        addReturn(rTrans);
        if( orig->getName() == it->second )   //Handle self-loops.
        {
          rTrans = Return(dup->getName(),dup->getName(),it->third,it->fourth);
          addReturn(rTrans);
        }
        if( orig->getName() == it->fourth )   //Handle self-loops.
        {
          rTrans = Return(dup->getName(),it->second,it->third,dup->getName());
          addReturn(rTrans);
        }
        if( orig->getName() == it->second && orig->getName() == it->fourth )   //Handle self-loops.
        {
          rTrans = Return(dup->getName(),dup->getName(),it->third,dup->getName());
          addReturn(rTrans);
        }
      }
      //Duplicate call predecessor return transitions.
      const Info::Returns pred = T_info.predTrans(orig);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans = Return(it->first,dup->getName(),it->third,it->fourth);
        addReturn(rTrans);
        if( orig->getName() == it->fourth )   //Handle self-loops.
        {
          rTrans = Return(it->first,dup->getName(),it->third,dup->getName());
          addReturn(rTrans);
        }
      }
      //Duplicate return site return transitions.
      const Info::Returns ret = T_info.retTrans(orig);
      for( Info::Returns::const_iterator it = ret.begin(); it != ret.end(); it++ )
      {
        Return rTrans = Return(it->first,it->second,it->third,dup->getName());
        addReturn(rTrans);
      }
    }

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     * This method removes all transitions from this collection of
     * transitions. 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::clear( )
    {
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      T_info.clearMaps();
    }
    
    /**
     *
     * @brief get all call transitions in this collection of transitions 
     *
     * @return all call transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls & TransSet<St,Sym,Call,Internal,Return>::getCalls() const
    {
      return callTrans;
    }
    
    /**
     *
     * @brief get all internal transitions in this collection of transitions 
     *
     * @return all internal transitions in the collection of transitions 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals & TransSet<St,Sym,Call,Internal,Return>::getInternals() const
    {
      return internalTrans;
    }
    
    /**
     *
     * @brief get all return transitions in the collection of transitions 
     *
     * @return all return transitions in the collection of transitions 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns & TransSet<St,Sym,Call,Internal,Return>::getReturns() const
    {
      return returnTrans;
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @param - addTrans: the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */ 
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::addCall( const Call & addTrans )
    {     
      if( callTrans.count(addTrans) > 0 )
        return false;      
      
      callTrans.insert(addTrans);
      T_info.addCall(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @param - addTrans: the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::addInternal( const Internal & addTrans )
    {
      if( internalTrans.count(addTrans) > 0 )
        return false;
      
      internalTrans.insert(addTrans);
      T_info.addIntra(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @param - addTrans: the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::addReturn( const Return & addTrans )
    {
      if( returnTrans.count(addTrans) > 0 )
        return false;
        
      returnTrans.insert(addTrans);
      T_info.addRet(addTrans);

      return true;
    }
      
    /**
     *
     * @brief add all transitions in the given collection of transitions to this
     *        collection of transitions
     *
     * @param - addTransSet: the collection of transitions to add to this 
     *          collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::addAllTrans( TransSet<St,Sym,Call,Internal,Return> addTransSet )
    {   
      //Add call transitions.
      for( TransSet::callIterator it = addTransSet.beginCall(); 
            it != addTransSet.endCall(); it ++ )
      {
        addCall(it);
      }
      //Add internal transitions.
      for( TransSet::internalIterator it = addTransSet.beginInternal(); 
            it != addTransSet.endInternal(); it ++ )
      {
        addInternal(it);
      }
      //Add return transitions.
      for( TransSet::returnIterator it = addTransSet.beginReturn(); 
            it != addTransSet.endReturn(); it ++ )
      {
        addReturn(it);
      }
    }
      
    /**
     *
     * @brief remove the given call transition from this collection of 
     *        transitions
     *
     * @param - removeTrans: the call transition to remove from the collection
     * @return false if the given call transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeCall( const Call & removeTrans )
    {
      if( callTrans.count(removeTrans) == 0 )
        return false;
        
      callTrans.erase(removeTrans);
      T_info.removeCall(removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given internal transition from the collection of 
     *        transitions
     *
     * @param - removeTrans: the internal transition to remove from the collection
     * @return false if the given internal transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternal( const Internal & removeTrans )
    {
      if( internalTrans.count(removeTrans) == 0 )
        return false;
        
      internalTrans.erase(removeTrans);
      T_info.removeIntra(removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given return transition from the collection of 
     *        transitions
     *
     * @param - removeTrans: the return transition to remove from the collection
     * @return false if the given return transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturn( const Return & removeTrans )
    {
      if( returnTrans.count(removeTrans) == 0 )
        return false;
        
      returnTrans.erase(removeTrans);
      T_info.removeRet(removeTrans);

      return true;
    }
     
    /**
     *
     * @brief test whether the given call transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the call transition to check
     * @return true if the given call transition is in this collection of 
     *          transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isCall( const Call & trans ) const
    {
      return (callTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test whether the given internal transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the internal transition to check
     * @return true if the given internal transition is in this collection of 
     *          transitions associated with the NWA
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isInternal( const Internal & trans ) const
    {
      return (internalTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test whether the given return transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the return transition to check
     * @return true if the given return transition is in this collection of 
     *          transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isReturn( const Return & trans ) const
    {
      return (returnTrans.count(trans) > 0);
    } 
         
    //Utilities	

    /**
     *
     * @brief print the collection of transitions
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::ostream & TransSet<St,Sym,Call,Internal,Return>::print( std::ostream & o ) const
    {    
      //Print call transitions.
      //TODO: this is commented out because StName does not have a print method.
      /*o << "Delta_c: \n" << "{ \n";
      callIterator cit = callTrans.begin();
      callIterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        if( !first )
          o << ", \n";
        o << "(";
        (*cit).first.print(o);
        o << ", ";
        (*cit).second.print(o);
        o << ", "; 
        (*cit).third.print(o);
        o << ")";
        first=false;
      }
      o << " \n}\n";
      //Print internal transitions.
      o << "Delta_i:\n" << "{\n";
      internalIterator iit = internalTrans.begin();
      internalIterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        //assert(*iit);
        if( !first )
          o << ", \n";
        o << "(";
        (*iit).first.print(o);
        o << ", ";
        (*iit).second.print(o);
        o << ", ";
        (*iit).third.print(o);
        o << ")";
        first = false;
      }
      o << " \n}\n";
      //Print return transitions.
      o << "Delta_r:\n" << "{\n";
      returnIterator rit = returnTrans.begin();
      returnIterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if( !first )
          o << ",\n";
        o << "(";
        (*rit).first.print(o);
        o << ", ";
        (*rit).second.print(o);
        o << ", "; 
        (*rit).third.print(o);
        o << ", ";
        (*rit).fourth.print(o);
        o << ")";
        first = false;
      }
      o << "\n}\n";
      */
      return o;
    }

    /*
     * 
     * @brief print the collection of transitions in dot format
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    std::ostream & TransSet<St,Sym,Call,Internal,Return>::print_dot( std::ostream & o) const
    {    
      //Print call transitions.
      //TODO: this is commented out because StName does not have a print method.
     /* o << "//Delta_c: \n";
      callIterator cit = callTrans.begin();
      callIterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        (*cit).first.print(o << "\"") << "\"";
        o << "->";
        (*cit).third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        (*cit).second.print(o);
        o << "\"";
        o << " color=green";
        o << "];\n";
        
      }
      o << " \n";
      //Print internal transitions.
      o << "// Delta_i:\n" ;
      internalIterator iit = internalTrans.begin();
      internalIterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        (*iit).first.print(o << "\"") << "\"";
        o << "->";
        (*iit).third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        (*iit).second.print(o);
        o << "\"";
        o << "];\n";
      }
      o << " \n";
      //Print return transitions.
      o << "// Delta_r:\n";
      returnIterator rit = returnTrans.begin();
      returnIterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        //dummy
        o << "\"" << std::dec << (*rit).second.getName() << std::dec << ", " << (*rit).first.getName() << "\"";
        o <<"[ shape=box ];\n";

        // exit to dummy
        (*rit).first.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << (*rit).second.getName() <<", " << (*rit).first.getName() << "\"";
        o << "[";
        o << " label=\"";
        (*rit).third.print(o);
        o << "\"";
        o << " color=red";
        o << "];\n";

        // call to dummy
        (*rit).second.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << (*rit).second.getName() <<", " << (*rit).first.getName() << "\"";
        o << "[";
        o << " color=blue";
        o << "];\n";

        //dummy to ret
        o << "\"" << (*rit).second.getName() <<", " << (*rit).first.getName() << "\"";
        o << "->";
        (*rit).fourth.print(o << "\"") << "\"";
        o << "[ style=dotted ];\n";

      }
      o << "\n";
      */
      return o;
    }

    /**
     *
     * @brief tests whether this collection of transitions is equivalent 
     *        to the collection of transitions 'other'
     *
     * @param - other: the TransSet to compare this TransSet to
     * @return true if this TransSet is equivalent to the TransSet 'other'
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::operator==( const TransSet<St,Sym,Call,Internal,Return> & other ) const
    { //TODO: Q: Do I want a deeper check here?
      return (  (callTrans == other.callTrans) &&
                (internalTrans == other.internalTrans) &&
                (returnTrans == other.returnTrans) );
    }

    /**
     *
     * @brief provides access to the call transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the call transitions
     *          in the collection of transitions
     *
     */  
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::beginCall() const
    {
      return callTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the internal transitions
     *          in the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::beginInternal() const
    {
      return internalTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the return transitions
     *          in the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::beginReturn() const
    {
      return returnTrans.begin();
    }
      
    /**
     *
     * @brief provides access to the call transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the call transitions in
     *          the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::endCall() const
    {
      return callTrans.end();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the internal transitions in
     *          the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::endInternal() const
    {
      return internalTrans.end();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the return transitions in
     *          the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::endReturn() const
    {
      return returnTrans.end();
    }
    
    /**
     *
     * @brief returns the number of call transitions in this collection of
     *        transitions
     *
     * @return the number of call transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeCall( ) const
    {
      return callTrans.size();
    }
        
    /**
     *
     * @brief returns the number of internal transitions in this collection of
     *        transitions 
     *
     * @return the number of internal transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeInternal( ) const
    {
      return internalTrans.size();
    }
        
    /**
     *
     * @brief returns the number of return transitions in this collection of
     *        transitions
     *
     * @return the number of return transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeReturn( )  const
    {
      return returnTrans.size();
    }
        
    /**
     *
     * @brief returns the total number of transitions (call, internal, and return) in 
     * this collection of transitions
     *
     * @return the total number of transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::size( ) const
    {
      return (sizeCall() + sizeInternal() + sizeReturn());
    }
    
    /**
     *  TODO: use refptr<St> 
     * @brief returns all outgoing internal transitions for the given state
     *
     * @param - state: the source state
     * @return the set of all outgoing internal transitions for the given state
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Internal> TransSet<St,Sym,Call,Internal,Return>::getTransFrom( const St * state ) const
    {
      //TODO getTransFrom( StName )
      return T_info.fromTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief retuans all incoming internal transitions for the given state
     *
     * @param - state: the target state
     * @return the set of all incoming internal transitions for the given state
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Internal> TransSet<St,Sym,Call,Internal,Return>::getTransTo( const St * state ) const
    {
      //TODO getTransTo( StName )
      return T_info.toTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief returns all call transitions for the given call site
     *
     * @param - state: the call site
     * @return the set of all call transitions for the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Call> TransSet<St,Sym,Call,Internal,Return>::getTransCall( const St * state ) const
    {
      //TODO getTransCall( StName )
      return T_info.callTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief returns all call transitions for the given entry point
     *
     * @param - state: the entry point
     * @return the set of all call transitions for the given entry point
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Call> TransSet<St,Sym,Call,Internal,Return>::getTransEntry( const St * state ) const
    {
      //TODO getTransEntry( StName )
      return T_info.entryTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief returns all return transitions for the given exit point
     *
     * @param - state: the exit point
     * @return the set of all return transitions for the given exit point
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Return> TransSet<St,Sym,Call,Internal,Return>::getTransExit( const St * state ) const
    {
      //TODO getTransExit( StName )
      return T_info.exitTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief returns all return transitions for the given call predecessor
     *
     * @param - state: the call predecessor
     * @return the set of all return transitions for the given call predecessor
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Return> TransSet<St,Sym,Call,Internal,Return>::getTransPred( const St * state ) const
    {
      //TODO getTransPred( StName )
      return T_info.predTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief returns all return transitions for the given return site
     *
     * @param - state: the return site
     * @return the set of all return transitions for the given return site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Return> TransSet<St,Sym,Call,Internal,Return>::getTransRet( const St * state ) const
    {
      //TODO getTransRet( StName )
      return T_info.retTrans( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the source of any internal 
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the source of some internal 
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isFrom( const St & state ) const
    {
      return T_info.isFrom( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the target of any internal
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the target of some internal
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isTo( const St & state ) const
    { 
      return T_info.isTo( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the call site of any call 
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the call site of some call
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isCall( const St & state ) const
    {
      return T_info.isCall( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the entry point of any call
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the entry point of some call 
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isEntry( const St & state ) const
    {
      return T_info.isEntry( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the exit point of any return
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the exit point of some return
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isExit( const St & state ) const
    {
      return T_info.isExit( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the call predecessor for any
     *        return transition
     *
     * @param - state: the state to test
     * @return true if the given state is the call predecessor of some return
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isPred( const St & state ) const
    {
      return T_info.isPred( state );
    }
    
    /**
     *  TODO: use refptr<St>  
     * @brief tests whether the given state is the return site of any return
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the return site of some return 
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isRet( const St & state ) const
    {
      return T_info.isRet( state );
    }
      
    /** 
     *  TODO: use refptr<St> 
     * @brief removes all transitions involving the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeTransWith( const St * state )
    {
      bool found = false;
      found = found || removeCallTransWith(state);
      found = found || removeInternalTransWith(state);
      found = found || removeReturnTransWith(state);
      return found;
    }

    /** 
     *  TODO: use refptr<St> 
     * @brief removes all call transitions to or from the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransWith( const St * state )
    {  
      Calls removeTrans = Calls();
      //Find all transitions to remove.
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == state->getName()) ||
            (cit->third == state->getName()) )
            removeTrans.insert(*cit);
      }     
      //Remove the transitions.
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *  TODO: use refptr<St> 
     * @brief removes all internal transitions to or from the given state
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransWith( const St * state )
    {
      Internals removeTrans = Internals();
      //Find transitions to remove.
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == state->getName()) ||
            (iit->third == state->getName()) )
            removeTrans.insert(*iit);
      }     
      //Remove transitions.
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *  TODO: use refptr<St> 
     * @brief removes all return transitions to or from the given state
     *        as well as return transitions corresponding to calls from
     *        that state
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransWith( const St * state )
    {
      Returns removeTrans = Returns();
      //Find transitions to remove.
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == state->getName()) ||
            (rit->second == state->getName()) ||
            (rit->fourth == state->getName()) )
            removeTrans.insert(*rit);
      }     
      //Remove transitions.
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0;
    }
    
    /** 
     *
     * @brief removes all call transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransSym( const Sym & sym )
    {
      Calls removeTrans = Calls();
      //Find transitions to remove.
      for( callIterator cit = callTrans.begin();
            cit != callTrans.end(); cit++ )
      {
        if( cit->second == sym )
          removeTrans.insert(*cit);
      }     
      //Remove transitions.
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;  
    }
    
    /** 
     *
     * @brief removes all internal transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransSym( const Sym & sym )
    {
      Internals removeTrans = Internals();
      //Find transitions to remove.
      for( internalIterator iit = internalTrans.begin();
            iit != internalTrans.end(); iit++ )
      {
        if( iit->second == sym )
          removeTrans.insert(*iit);
      }     
      //Remove transitions.
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /** 
     *
     * @brief removes all return transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransSym( const Sym & sym )
    {
      Returns removeTrans = Returns();
      //Find transitions to remove.
      for( returnIterator rit = returnTrans.begin();
            rit != returnTrans.end(); rit++ )
      {
        if( rit->third == sym )
          removeTrans.insert(*rit);
      }     
      //Remove transitions.
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     *  TODO: use refptr<St> 
     *  TODO optimize
     * @brief test if there exists a call transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the call transition
     * @param - sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     *          symbol in this collection of transitions 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::callExists( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == from->getName()) && (cit->second == *sym) )
          return true;
      }  
      return false;    
    }
    
    /**
     *  TODO: use refptr<St> 
     *  TODO optimize
     * @brief provides access to all call transitions with the given from state
     *        and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the call transitions
     * @param - sym: the desired symbol for the call transitions
     * @return the set of call transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls TransSet<St,Sym,Call,Internal,Return>::getCalls( St * from, Sym * sym ) const 
    {
      assert(from);
      assert(sym);
      Calls result;
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == from->getName()) && (cit->second == *sym) )
          result.insert(*cit);
      } 
      return result;
    }
    
    /**
     *  TODO: use refptr<St> 
     *  TODO optimize
     * @brief test if there exists an internal transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the internal transition
     * @param - sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::internalExists( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      for( internalIterator iit = beginInternal(); iit != endInternal(); iit++ )
      {
        if( (iit->first == from->getName()) && (iit->second == *sym) )
          return true;    
      }     
      return false;
    }
    
    /**
     *  TODO: use refptr<St> 
     *  TODO optimize
     * @brief provides access to all internal transitions with the given from 
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @param - sym: the desired symbol for the internal transitions
     * @return the set of internal transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals TransSet<St,Sym,Call,Internal,Return>::getInternals( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      Internals result;
      for( internalIterator iit = beginInternal(); iit != endInternal(); iit++ )
      {
        if( (iit->first == from->getName()) && (iit->second == *sym) )
          result.insert(*iit);
      } 
      return result;
    }
    
    /**
     *  TODO this is redundant
     * @brief provides access to all internal transitions with the given 
     *        from state in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @return the set of internal transitions with the given from state
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals TransSet<St,Sym,Call,Internal,Return>::getInternalsFrom( St * from ) const
    {
      assert(from);
      return fromTrans(from);
    }


    /**
     *  TODO: use refptr<St> 
     *  TODO optimize
     * @brief test if there exists a return transition with the given from state, 
     *        predecessor state, and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the return transition
     * @param - pred: the desired predecessor state for the return transition
     * @param - sym: the desired symbol for the return transition
     * @return true if there exists a return transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::returnExists( St * from, St * pred, Sym * sym ) const
    {
      assert(from);
      assert(pred);
      assert(sym);
      for( returnIterator rit = beginReturn(); rit != endReturn(); rit++ )
      {
        if( (rit->first == from->getName()) && (rit->second == pred->getName()) && (rit->third == *sym) )
            return true;      
      }     
      return false;
    }   
    
    /**
     *  TODO: use refptr<St> 
     *  TODO optimize
     * @brief provides access to all return transitions with the given from
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the return transitions
     * @param - sym: the desired symbol for the return transitions
     * @return the set of return transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns TransSet<St,Sym,Call,Internal,Return>::getReturns( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      Returns result;
      for( returnIterator rit = beginReturn(); rit != endReturn(); rit++ )
      {
        if( (rit->first == from->getName()) && (rit->third == *sym) )
          result.insert(*rit);
      } 
      return result;
    } 
#else //ifdef KEY
    template <typename St, typename Sym, typename Call, typename Internal, typename Return>
    class TransSet : public Printable
    {
      public:        
        typedef std::set< Call > Calls;
        typedef std::set< Internal > Internals;
        typedef std::set< Return > Returns;
        
        typedef typename Calls::const_iterator callIterator;
        typedef typename Internals::const_iterator internalIterator;
        typedef typename Returns::const_iterator returnIterator;
        
        typedef TransInfo<St,Call,Internal,Return> Info;
      
      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      TransSet( );
      TransSet( const TransSet & other );
      TransSet & operator=( const TransSet & other );

      ~TransSet( );

      //Transition Accessors

      /**
       *  
       * @brief finds the symbol on any transition whose source is 'from' and
       *        whose target is 'to'
       *
       * This method return the symbol on any transition whose source is 'from'
       * and whose target is 'to'.
       *
       * @param - from: the source of the transition
       * @param - to: the target of the transition
       * @return true if there exists some transition with the given source and 
       *         target, false otherwise
       *
       */
      bool getSymbol( const St & from, const St & to, Sym & sym );

      /**
       * 
       * @brief tests whether there exists a transition whose source is 'from',
       *        whose symbol is 'sym', and whose target is 'to'
       *
       * This method determines whether there exists a transition (internal, call,
       * or return) whose source is 'from', whose symbol is 'sym', and whose target
       * is 'to'.
       *
       * @param - from: the source of the transition
       * @param - sym: the symbol that labels the transition
       * @param - to: the target of the transition
       * @return true if such a transition exists, false otherwise
       * 
       */
      bool findTrans( const St & from, const Sym & sym, const St & to ) const;

      /**
       * 
       * @brief returns all return sites that correspond with the given call site
       *
       * This method returns the set of all return sites that correspond with the
       * given call site.
       *
       * @param - callSite: the call site whose return sites to look for
       * @return the set of all return sites that correspond with the given call site
       *
       */
      std::set<St> getReturnSites( const St & callSite ) const;

      /**
       *  
       * @brief returns all return sites that correspond with the given exit and call site
       *
       * This method returns the set of all return sites that correspond with the
       * given exit and call site.
       *
       * @param - exit: the exit whose return sites to look for
       * @param - callSite: the call site whose return sites to look for
       * @return the set of all return sites that correspond with the given exit and call site
       *
       */
      const std::set<Return> getReturns( const St & exit, const St & callSite ) const;

      /**
       * 
       * @brief returns all call sites that correspond with the given exit - return site pair
       *
       * This method returns the set of all call sites that correspond with the given
       * exit - return site pair.
       *
       * @param - exitSite: the exit of the pair whose call sites to look for
       * @param = returnSite: the return site of the pair whose call sites to look for
       * @return the set of all call sites that correspond with the exit - return site pair
       *
       */
      std::set<St> getCallSites( const St & exitSite, const St & returnSite ) const;

      /**
       * 
       * @brief returns all entry sites that correspond with the given call site
       *
       * This method returns the set of all entry sites that correspond with the
       * given call site.
       *
       * @param - callSite: the call site whose entry sites to look for
       * @return the set of all entry sites that correspond with the given call site
       *
       */
      const std::set<Call> getEntries( const St & callSite ) const;

      /**
       * 
       * @brief returns all targets that correspond with the given source 
       *
       * This method returns the set of all targets that correspond with the 
       * given source .
       *
       * @param - source: the source whose targets to look for
       * @return the set of all targets that correspond with the given source 
       *
       */
      const std::set<Internal> getTargets( const St & source ) const;

      /**
       *  
       * @brief creates transitions for 'dup' mirroring 'orig' outgoing transitions
       *
       * This method creates a transition using 'dup' in the place of 'orig' for
       * each outgoing transition involving 'orig'.  Do not duplicate selfloops.
       *
       * @param - orig: the state that is being duplicated
       * @param - dup: the state that is duplicating 'orig'
       *  
       */
      void dupTransOutgoing( const St & orig, const St & dup );

      /**
       *  
       * @brief creates transitions for 'dup' mirroring 'orig' transitions
       *
       * This method creates a transition using 'dup' in the place of 'orig' for
       * each transition involving 'orig'.  In addition, for any transition involving a 
       * self-loop on 'orig', two(three in the case of a return transition with exit, 
       * pred, and ret all 'orig') transitions are added (using 'dup' for 'orig' in each 
       * position while retaining 'orig' in all other positions).
       *
       * @param - orig: the state that is being duplicated
       * @param - dup: the state that is duplicating 'orig'
       *  
       */
      void dupTrans( const St & orig, const St & dup );

      /**
       *
       * @brief removes all transitions from this collection of transitions
       *
       * This method removes all transitions from this collection of
       * transitions. 
       *
       */
      void clear( );
      
      /**
       *
       * @brief get all call transitions in this collection of transitions 
       *
       * This method provides access to all call transitions in this collection of
       * transitions.  
       *
       * @return all call transitions in this collection of transitions
       *
       */
      const Calls & getCalls() const;
        
      /**
       *
       * @brief get all internal transitions in this collection of transitions 
       *
       * This method provides access to all internal transitions in this collection 
       * of transitions.  
       *
       * @return all internal transitions in the collection of transitions 
       *
       */
      const Internals & getInternals() const;
       
      /**
       *
       * @brief get all return transitions in the collection of transitions 
       *
       * This method provides access to all return transitions in the collection 
       * of transitions associated with the NWA.  
       *
       * @return all return transitions in the collection of transitions 
       *
       */
      const Returns & getReturns() const;
      
      /**
       *
       * @brief add the given call transition to the collection of transitions
       *
       * This method adds the given call transition to this collection of 
       * transitions.  If this exact call transition already exists, false is 
       * returned. Otherwise, true is returned.
       *
       * @param - addTrans: the call transition to add to the collection of transitions
       * @return false if the call transition already exists in the collection
       *
       */
      bool addCall( const Call & addTrans );
        
      /**
       *
       * @brief add the given internal transition to the collection of transitions
       *
       * This method adds the given internal transition to the collection of 
       * transitions.  If this exact internal transition already exists, false is 
       * returned. Otherwise, true is returned.
       *
       * @param - addTrans: the internal transition to add to the collection of transitions
       * @return false if the internal transition already exists in the collection
       *
       */
      bool addInternal( const Internal & addTrans );
        
      /**
       *
       * @brief add the given return transition to the collection of transitions
       *
       * This method adds the given return transition to the collection of 
       * transitions.  If this exact return transition already exists, false is 
       * returned. Otherwise, true is returned.
       *
       * @param - addTrans: the return transition to add to the collection of transitions
       * @return false if the return transition already exists in the collection
       *
       */
      bool addReturn( const Return & addTrans );
      
      /**
       *
       * @brief add all transitions in the given collection of transitions to this
       *        collection of transitions
       *
       * This method adds all of the transitions in the given collection of
       * transitions to this collection of transitions.
       *
       * @param - addTransSet: the collection of transitions to add to this 
       *          collection of transitions
       *
       */
      void addAllTrans( TransSet addTransSet );
      
      /**
       *
       * @brief remove the given call transition from this collection of 
       *        transitions
       *
       * This method removes the given call transition from this collection 
       * of transitions.  If the given call transition does not exust in
       * this collection of transitions false is returned.  Otherwise,
       * true is returned.
       *
       * @param - removeTrans: the call transition to remove from the collection
       * @return false if the given call transition does not exist in the 
       *          collection, true otherwise.
       *
       */
      bool removeCall( const Call & removeTrans );
        
      /**
       *
       * @brief remove the given internal transition from the collection of 
       *        transitions
       *
       * This method removes the given internal transition from the collection 
       * of transitions.  If the given internal transition does not exist in
       * this collection of transitions false is returned.  Otherwise,
       * true is returned.
       *
       * @param - removeTrans: the internal transition to remove from the collection
       * @return false if the given internal transition does not exist in the 
       *          collection, true otherwise.
       *
       */
      bool removeInternal( const Internal & removeTrans );
        
      /**
       *
       * @brief remove the given return transition from the collection of 
       *        transitions
       *
       * This method removes the given return transition from the collection 
       * of transitions.  If the given return transition does not exist in 
       * this collection of transitions false is returned.  Otherwise, true 
       * is returned.
       *
       * @param - removeTrans: the return transition to remove from the collection
       * @return false if the given return transition does not exist in the 
       *          collection, true otherwise.
       *
       */
      bool removeReturn( const Return & removeTrans );
                      
      /**
       *
       * @brief test whether the given call transition is in this collection of 
       *        transitions 
       *
       * This method tests whether the given call transition is in this collection
       * of transitions.
       *
       * @param - trans: the call transition to check
       * @return true if the given call transition is in this collection of 
       *          transitions
       *
       */
      bool isCall( const Call & trans ) const;
        
      /**
       *
       * @brief test whether the given internal transition is in this collection of 
       *        transitions 
       *
       * This method tests whether the given internal transition is in this
       * collection of transitions.
       *
       * @param - trans: the internal transition to check
       * @return true if the given internal transition is in this collection of 
       *          transitions associated with the NWA
       *
       */
      bool isInternal( const Internal & trans ) const;
       
      /**
       *
       * @brief test whether the given return transition is in this collection of 
       *        transitions 
       *
       * This method tests whether the given return transition is in this
       * collection of transitions.
       *
       * @param - trans: the return transition to check
       * @return true if the given return transition is in this collection of 
       *          transitions
       *
       */
      bool isReturn( const Return & trans ) const;                   
                    
                      
      //Utilities	

      /**
       *
       * @brief print the collection of transitions
       *
       * This method prints out the transition set to the output stream 
       * provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print( std::ostream & o ) const;

      /*
       * 
       * @brief print the collection of transitions in dot format
       *
       * This method prints out the transition set in dot format to the
       * output stream provided.
       *
       * @param - o: the output stream to print to
       * @return the output stream that was printed to
       *
       */
      std::ostream & print_dot( std::ostream & o ) const;

      /**
       *
       * @brief tests whether this collection of transitions is equivalent 
       *        to the collection of transitions 'other'
       *
       * This method tests the equivalence of this set of transitions and 
       * the set of transitions 'other'.
       *
       * @param - other: the TransSet to compare this TransSet to
       * @return true if this TransSet is equivalent to the TransSet 'other'
       *
       */
      bool operator==( const TransSet & other ) const;

      /**
       *
       * @brief provides access to the call transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the call transitions in the collection
       * of transitions through an iterator.
       *
       * @return the starting point of an iterator through the call transitions
       *          in the collection of transitions
       *
       */
      callIterator beginCall( ) const;
        
      /**
       *
       * @brief provides access to the internal transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the internal transitions in the collection
       * of transitions through an iterator.
       *
       * @return the starting point of an iterator through the internal transitions
       *          in the collection of transitions
       *
       */
      internalIterator beginInternal( ) const;
       
      /**
       *
       * @brief provides access to the return transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the return transitions in the collection
       * of transitions through an iterator.
       *
       * @return the starting point of an iterator through the return transitions
       *          in the collection of transitions
       *
       */
      returnIterator beginReturn( ) const;
      
      /**
       *
       * @brief provides access to the call transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the call transitions in the collection
       * of transitions through an iterator.
       *
       * @return the exit point of an iterator through the call transitions in
       *          the collection of transitions
       *
       */
      callIterator endCall( ) const;
         
      /**
       *
       * @brief provides access to the internal transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the internal transitions in the collection
       * of transitions through an iterator.
       *
       * @return the exit point of an iterator through the internal transitions in
       *          the collection of transitions
       *
       */
      internalIterator endInternal( ) const;
         
      /**
       *
       * @brief provides access to the return transitions in the collection 
       *        through an iterator
       *
       * This method provides access to the return transitions in the collection
       * of transitions through an iterator.
       *
       * @return the exit point of an iterator through the return transitions in
       *          the collection of transitions
       *
       */
      returnIterator endReturn( ) const;

      /**
       *
       * @brief returns the number of call transitions in this collection of
       *        transitions
       *
       * This method returns the number of call transitions in the collection
       * of transitions associated with the NWA.  
       *
       * @return the number of call transitions in this collection of transitions
       *
       */
      size_t sizeCall( ) const;
        
      /**
       *
       * @brief returns the number of internal transitions in this collection of
       *        transitions 
       *
       * This method returns the number of internal transitions in this collection
       * of transitions.  
       *
       * @return the number of internal transitions in this collection of transitions
       *
       */
      size_t sizeInternal( ) const;
        
      /**
       *
       * @brief returns the number of return transitions in this collection of
       *        transitions
       *
       * This method returns the number of return transitions in this collection
       * of transitions associated with the NWA.  
       *
       * @return the number of return transitions in this collection of transitions
       *
       */
      size_t sizeReturn( ) const;
        
      /**
       *
       * @brief returns the total number of transitions (call, internal, and return) in 
       * this collection of transitions
       *
       * This method returns the total number of transitions (call, internal, and return) 
       * in this collection of transitions.  
       *
       * @return the total number of transitions in this collection of transitions
       *
       */
      size_t size( ) const;
        
      /**
       * 
       * @brief returns all outgoing internal transitions for the given state
       *
       * This method returns all internal transitions with the given state as
       * the source.
       *
       * @param - state: the source state
       * @return the set of all outgoing internal transitions for the given state
       *
       */
      std::set<Internal> getTransFrom( const St & state ) const;
        
      /**
       * 
       * @brief retuans all incoming internal transitions for the given state
       *
       * This method returns all internal transitions with the given state as
       * the target.
       *
       * @param - state: the target state
       * @return the set of all incoming internal transitions for the given state
       *
       */
      std::set<Internal> getTransTo( const St & state ) const;
        
      /**
       * 
       * @brief returns all call transitions for the given call site
       *
       * This method returns all call transitions with the given state as
       * the call site.
       *
       * @param - state: the call site
       * @return the set of all call transitions for the given call site
       *
       */
      std::set<Call> getTransCall( const St & state ) const;
        
      /**
       * 
       * @brief returns all call transitions for the given entry point
       *
       * This method returns all call transitions with the given state as
       * the entry point.
       *
       * @param - state: the entry point
       * @return the set of all call transitions for the given entry point
       *
       */
      std::set<Call> getTransEntry( const St & state ) const;
        
      /**
       * 
       * @brief returns all return transitions for the given exit point
       *
       * This method returns all return transitions with the given state as
       * the exit point.
       * 
       * @param - state: the exit point
       * @return the set of all return transitions for the given exit point
       *
       */
      std::set<Return> getTransExit( const St & state ) const;
        
      /**
       * 
       * @brief returns all return transitions for the given call predecessor
       *
       * This method returns all return transitions with the given state as
       * the call predecessor.
       * 
       * @param - state: the call predecessor
       * @return the set of all return transitions for the given call predecessor
       *
       */
      std::set<Return> getTransPred( const St & state ) const;
        
      /**
       * 
       * @brief returns all return transitions for the given return site
       *
       * This method returns all return transitions with the given state as
       * the return site.
       *
       * @param - state: the return site
       * @return the set of all return transitions for the given return site
       *
       */
      std::set<Return> getTransRet( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the source of any internal 
       *        transition
       *
       * This method determines whether the given state is the source of any 
       * internal transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the source of some internal 
       *          transition, false otherwise
       *
       */
      bool isFrom( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the target of any internal
       *        transition
       *
       * This method determines whether the given state is the target of any
       * internal transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the target of some internal
       *          transition, false otherwise
       *
       */
      bool isTo( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the call site of any call 
       *        transition
       *
       * This method determines whether the given state is the call site of 
       * any call transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the call site of some call
       *          transition, false otherwise
       *
       */
      bool isCall( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the entry point of any call
       *        transition
       *
       * This method determines whether the given state is the entry point
       * of any call transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the entry point of some call 
       *          transition, false otherwise
       *
       */
      bool isEntry( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the exit point of any return
       *        transition
       *
       * This method determines whether the given state is the exit point of 
       * any return transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the exit point of some return
       *          transition, false otherwise
       *
       */
      bool isExit( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the call predecessor for any
       *        return transition
       *
       * This methos determines whether the given state is the call predecessor
       * for any return transition.
       *
       * @param - state: the state to test
       * @return true if the given state is the call predecessor of some return
       *          transition, false otherwise
       *
       */
      bool isPred( const St & state ) const;
        
      /**
       * 
       * @brief tests whether the given state is the return site of any return
       *        transition
       *
       * This method determines whether the given state is the return site of any
       * return transition.
       * 
       * @param - state: the state to test
       * @return true if the given state is the return site of some return 
       *          transition, false otherwise
       *
       */
      bool isRet( const St & state ) const;

      /** 
       *  
       * @brief removes all transitions involving the given state  
       *
       * This method removes all transitions involving the given state 
       * If no transitions exist involving this state false is returned.  
       * Otherwise, true is returned.
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeTransWith( const St * state );     
      
      /** 
       *
       * @brief removes all call transitions with the given symbol 
       *
       * This method removes all call transitions with the given symbol. 
       * If no call transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeCallTransSym( const Sym & sym );
        
      /** 
       *
       * @brief removes all internal transitions with the given symbol 
       *
       * This method removes all internal transitions with the given symbol. 
       * If no internal transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeInternalTransSym( const Sym & sym );
        
      /** 
       *
       * @brief removes all return transitions with the given symbol 
       *
       * This method removes all return transitions with the given symbol. 
       * If no return transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeReturnTransSym( const Sym & sym );
      
      /**
       *  TODO: use refptr<St>  
       * @brief test if there exists a call transition with the given from state 
       *        and symbol in this collection of transitions 
       *
       * This method tests whether there exists a call transition with the given 
       * from state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the call transition
       * @param - sym: the desired symbol for the call transition
       * @return true if there exists a call transition with the given from state and
       *          symbol in this collection of transitions 
       *
       */
      bool callExists( St * from, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St>   
       * @brief provides access to all call transitions with the given from state
       *        and symbol in this collection of transitions
       *
       * This method provides access to all call transitions with the given from
       * state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the call transitions
       * @param - sym: the desired symbol for the call transitions
       * @return the set of call transitions with the given from state and symbol
       *
       */
      const Calls getCalls( St * from, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief test if there exists an internal transition with the given from state 
       *        and symbol in this collection of transitions 
       *
       * This method tests whether there exists an internal transition with the given 
       * from state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the internal transition
       * @param - sym: the desired symbol for the internal transition
       * @return true if there exists an internal transition with the given from state and
       *          symbol in this collection of transitions
       *
       */
      bool internalExists( St * from, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief provides access to all internal transitions with the given from 
       *        state and symbol in this collection of transitions
       *
       * This method provides access to all internal transitions with the given
       * from state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the internal transitions
       * @param - sym: the desired symbol for the internal transitions
       * @return the set of internal transitions with the given from state and symbol
       *
       */
      const Internals getInternals( St * from, Sym * sym ) const;

      /**
       *  TODO: use refptr<St>  
       *  TODO this is redundant
       * @brief provides access to all internal transitions with the given 
       *        from state in this collection of transitions
       *
       * This method provides access to all internal transitions with the given
       * from state in this collection of transitions.
       *
       * @param - from: the desired from state for the internal transitions
       * @return the set of internal transitions with the given from state
       *
       */
      const Internals getInternalsFrom( St * from ) const;
        
      /**
       *  TODO: use refptr<St>  
       * @brief test if there exists a return transition with the given from state, 
       *        predecessor state, and symbol in this collection of transitions 
       *
       * This method tests whether there exists a return transition with the given 
       * from state, predecessor state, and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the return transition
       * @param - pred: the desired predecessor state for the return transition
       * @param - sym: the desired symbol for the return transition
       * @return true if there exists a return transition with the given from state and
       *          symbol in this collection of transitions
       *
       */
      bool returnExists( St * from, St * pred, Sym * sym ) const;
        
      /**
       *  TODO: use refptr<St>   
       * @brief provides access to all return transitions with the given from
       *        state and symbol in this collection of transitions
       *
       * This method provides access to all return transitions with the given from
       * state and symbol in this collection of transitions.
       *
       * @param - from: the desired from state for the return transitions
       * @param - sym: the desired symbol for the return transitions
       * @return the set of return transitions with the given from state and symbol
       *
       */
      const Returns getReturns( St * from, Sym * sym ) const;

      protected:
        /** 
       *
       * @brief removes all call transitions to or from the given state  
       *
       * This method removes all call transitions to or from the given state 
       * If no call transitions exist to or from this state false is returned.  
       * Otherwise, true is returned.
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeCallTransWith( const St & state );        
      
      /** 
       *
       * @brief removes all internal transitions to or from the given state
       *
       * This method removes all internal transitions to or from the given
       * state.  If no internal transitions exist to or from this state, 
       * false is returned.  Otherwise, true is returned. 
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeInternalTransWith( const St & state );
      
      /** 
       *
       * @brief removes all return transitions to or from the given state
       *        as well as return transitions corresponding to calls from
       *        that state
       *
       * This method removes all return transitions to or from the given 
       * state as well as return transitions corresponding to calls from
       * that state.  If no return transitions exist to or from this state,
       * or having this state as a call predecessor, false is returned.  
       * Otherwise, true is returned.
       *
       * @param - state: the state whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeReturnTransWith( const St & state );    

      //
      // Variables
      //
      
      protected: 
       
      Calls callTrans;
      Internals internalTrans;
      Returns returnTrans;
        
      Info T_info;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return>::TransSet( )
    {
      callTrans = Calls();
      internalTrans = Internals();
      returnTrans = Returns();  
      
      T_info = Info();
    }
    
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return>::TransSet( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
    }
    
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return> & TransSet<St,Sym,Call,Internal,Return>::operator=( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      if (this == &other)     
        return *this;

      clear();
      T_info.clearMaps();
        
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
      return *this;
    }
   
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    TransSet<St,Sym,Call,Internal,Return>::~TransSet( ) 
    {
      clear();
      T_info.clearMaps(); 
    }

    //Transition Accessors

    /**
     *  
     * @brief finds the symbol on any transition whose source is 'from' and
     *        whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - to: the target of the transition
     * @return true if there exists some transition with the given source and 
     *         target, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::getSymbol( const St & fromSt, const St & toSt, Sym & sym )
    {
      //Check internal transitions.
      const Info::Internals from = T_info.fromTrans(fromSt);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == (*it).third )
        {
          sym = (*it).second;
          return true;
        }
      }
      //Check call transitions.
      const Info::Calls call = T_info.callTrans(fromSt);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == (*it).third )
        {
          sym = (*it).second;
          return true;
        }
      }
      //Check return transitions.      
      const Info::Returns exit = T_info.exitTrans(fromSt);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == (*it).fourth )
        {
          sym = (*it).third;
          return true;
        }
      }
      //TODO: Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt == (*it)->fourth )
        {
          sym = (*it)->third;
          return true;
        }
      }*/
      
      return false;
    }

    /**
     * 
     * @brief tests whether there exists a transition whose source is 'from',
     *        whose symbol is 'sym', and whose target is 'to'
     *
     * @param - from: the source of the transition
     * @param - sym: the symbol that labels the transition
     * @param - to: the target of the transition
     * @return true if such a transition exists, false otherwise
     * 
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::findTrans( const St & fromSt, const Sym & sym, const St & toSt ) const
    {
      //Check internal transitions.
      const Info::Internals from = T_info.fromTrans(fromSt);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == (*it).third && sym==(*it).second)
          return true;
      }
      //Check call transitions.
      const Info::Calls call = T_info.callTrans(fromSt);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == (*it).third && sym == (*it).second)
        {
          return true;
        }
      }
      //Check return transitions.      
      const Info::Returns exit = T_info.exitTrans(fromSt);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == (*it).fourth && sym == (*it).third)
        {
          return true;
        }
      }
      //TODO: Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt == (*it)->fourth )
        {
          sym = (*it)->third;
          return true;
        }
      }*/
      
      return false;
    }

    /**
     * 
     * @brief returns all return sites that correspond with the given call site
     *
     * @param - callSite: the call site whose return sites to look for
     * @return the set of all return sites that correspond with the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<St> TransSet<St,Sym,Call,Internal,Return>::getReturnSites( const St & callSite ) const
    {
      std::set<St> returns;
      const Info::Returns pred = T_info.predTrans(callSite);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        returns.insert(it->fourth);
      }
      return returns;
    }

    /**
     * 
     * @brief returns all return sites that correspond with the given exit and call site
     *
     * @param - exit: the exit whose return sites to look for
     * @param - callSite: the call site whose return sites to look for
     * @return the set of all return sites that correspond with the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const std::set<typename Return> TransSet<St,Sym,Call,Internal,Return>::getReturns( const St & exit, const St & callSite ) const
    {
      std::set<Return> returns;
      const Info::Returns pred = T_info.predTrans(callSite);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( it->first == exit->getName() )
          returns.insert(*it);
      }
      return returns;
    }

    /**
     * 
     * @brief returns all call sites that correspond with the given exit - return site pair
     *
     * @param - exitSite: the exit of the pair whose call sites to look for
     * @param = returnSite: the return site of the pair whose call sites to look for
     * @return the set of all call sites that correspond with the exit - return site pair
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<St> TransSet<St,Sym,Call,Internal,Return>::getCallSites( const St & exitSite, const St & returnSite ) const
    {
      std::set<St> calls;
      const Info::Returns exit = T_info.exitTrans(exitSite);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( (*it).fourth == returnSite )
          calls.insert(it->first);
      }
      return calls;
    }

    /**
     * 
     * @brief returns all entry sites that correspond with the given call site
     *
     * @param - callSite: the call site whose entry sites to look for
     * @return the set of all entry sites that correspond with the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const std::set<typename Call> TransSet<St,Sym,Call,Internal,Return>::getEntries( const St & callSite ) const
    {
      std::set<Call> entries;
      const Info::Calls cll = T_info.callTrans(callSite);
      for( Info::Calls::const_iterator it = cll.begin(); it != cll.end(); it++ )
      {
        entries.insert(*it);
      }
      return entries;
    }

    /**
     * 
     * @brief returns all targets that correspond with the given source 
     *
     * @param - source: the source whose targets to look for
     * @return the set of all targets that correspond with the given source 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const std::set<typename Internal> TransSet<St,Sym,Call,Internal,Return>::getTargets( const St & source ) const
    {
      std::set<Internal> targets;
      const Info::Internals src = T_info.fromTrans(source);
      for( Info::Internals::const_iterator it = src.begin(); it != src.end(); it++ )
      {
        targets.insert(*it);
      }
      return targets;
    }

    /**
     *   
     * @brief creates transitions for 'dup' mirroring 'orig' outgoing transitions
     *
     * @param - orig: the state that is being duplicated
     * @param - dup: the state that is duplicating 'orig'
     *  
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::dupTransOutgoing( const St & orig, const St & dup )
    { 
      //Duplicate outgoing internal transitions.
      const Info::Internals from = T_info.fromTrans(orig);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans = Internal(dup->getName(),it->second,it->third);
        addInternal(iTrans);

      }
      //Duplicate call site call transitions.
      const Info::Calls call = T_info.callTrans(orig);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans = Call(dup->getName(),it->second,it->third);
        addCall(cTrans);

      }
      //Duplicate exit point return transitions.
      const Info::Returns exit = T_info.exitTrans(orig);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans = Return(dup->getName(),it->second,it->third,it->fourth);
        addReturn(rTrans);
      }
      //Duplicate call predecessor return transitions.
      const Info::Returns pred = T_info.predTrans(orig);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans = Return(it->first,dup->getName(),it->third,it->fourth);
        addReturn(rTrans);
      }
    }

    /**
     *  
     * @brief creates transitions for 'dup' mirroring 'orig' transitions
     *
     * @param - orig: the state that is being duplicated
     * @param - dup: the state that is duplicating 'orig'
     *  
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::dupTrans( const St & orig, const St & dup )
    { 
      //Duplicate outgoing internal transitions.
      const Info::Internals from = T_info.fromTrans(orig);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        Internal* iTrans = new Internal(dup,(*it).second,(*it).third);
        addInternal(*iTrans);
        if( orig == (*it).third )   //Handle self-loops.
        {
          iTrans = new Internal(dup,(*it).second,dup);
          addInternal(*iTrans);
        }
      }
      //Duplicate incoming internal transitions.
      const Info::Internals to = T_info.toTrans(orig);
      for( Info::Internals::const_iterator it = to.begin(); it != to.end(); it++ )
      {
        Internal* iTrans = new Internal((*it).first,(*it).second,dup);
        addInternal(*iTrans);
      }
      //Duplicate call site call transitions.
      const Info::Calls call = T_info.callTrans(orig);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        Call* cTrans = new Call(dup,(*it).second,(*it).third);
        addCall(*cTrans);
        if( orig == (*it).third )   //Handle self-loops.
        {
          cTrans = new Call(dup,(*it).second,dup);
          addCall(*cTrans);
        }
      }
      //Duplicate entry point call transitions.
      const Info::Calls entry = T_info.entryTrans(orig);
      for( Info::Calls::const_iterator it = entry.begin(); it != entry.end(); it++ )
      {
        Call* cTrans = new Call((*it).first,(*it).second,dup);
        addCall(*cTrans);
      }
      //Duplicate exit point return transitions.
      const Info::Returns exit = T_info.exitTrans(orig);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return* rTrans = new Return(dup,(*it).second,(*it).third,(*it).fourth);
        addReturn(*rTrans);
        if( orig == (*it).second )   //Handle self-loops.
        {
          rTrans = new Return(dup,dup,(*it).third,(*it).fourth);
          addReturn(*rTrans);
        }
        if( orig == (*it).fourth )   //Handle self-loops.
        {
          rTrans = new Return(dup,(*it).second,(*it).third,dup);
          addReturn(*rTrans);
        }
        if( orig == (*it).second && orig == (*it).fourth )   //Handle self-loops.
        {
          rTrans = new Return(dup,dup,(*it).third,dup);
          addReturn(*rTrans);
        }
      }
      //Duplicate call predecessor return transitions.
      const Info::Returns pred = T_info.predTrans(orig);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return* rTrans = new Return((*it).first,dup,(*it).third,(*it).fourth);
        addReturn(*rTrans);
        if( orig == (*it).fourth )   //Handle self-loops.
        {
          rTrans = new Return((*it).first,dup,(*it).third,dup);
          addReturn(*rTrans);
        }
      }
      //Duplicate return site return transitions.
      const Info::Returns ret = T_info.retTrans(orig);
      for( Info::Returns::const_iterator it = ret.begin(); it != ret.end(); it++ )
      {
        Return* rTrans = new Return((*it).first,(*it).second,(*it).third,dup);
        addReturn(*rTrans);
      }
    }

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     * This method removes all transitions from this collection of
     * transitions. 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::clear( )
    {
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      T_info.clearMaps();
    }
    
    /**
     *
     * @brief get all call transitions in this collection of transitions 
     *
     * @return all call transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls & TransSet<St,Sym,Call,Internal,Return>::getCalls() const
    {
      return callTrans;
    }
    
    /**
     *
     * @brief get all internal transitions in this collection of transitions 
     *
     * @return all internal transitions in the collection of transitions 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals & TransSet<St,Sym,Call,Internal,Return>::getInternals() const
    {
      return internalTrans;
    }
    
    /**
     *
     * @brief get all return transitions in the collection of transitions 
     *
     * @return all return transitions in the collection of transitions 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns & TransSet<St,Sym,Call,Internal,Return>::getReturns() const
    {
      return returnTrans;
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @param - addTrans: the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */ 
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::addCall( const Call & addTrans )
    {     
      if( callTrans.count(addTrans) > 0 )
        return false;      
      
      callTrans.insert(addTrans);
      T_info.addCall(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @param - addTrans: the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::addInternal( const Internal & addTrans )
    {
      if( internalTrans.count(addTrans) > 0 )
        return false;
      
      internalTrans.insert(addTrans);
      T_info.addIntra(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @param - addTrans: the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::addReturn( const Return & addTrans )
    {
      if( returnTrans.count(addTrans) > 0 )
        return false;
        
      returnTrans.insert(addTrans);
      T_info.addRet(addTrans);

      return true;
    }
      
    /**
     *
     * @brief add all transitions in the given collection of transitions to this
     *        collection of transitions
     *
     * @param - addTransSet: the collection of transitions to add to this 
     *          collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    void TransSet<St,Sym,Call,Internal,Return>::addAllTrans( TransSet<St,Sym,Call,Internal,Return> addTransSet )
    {   
      //Add call transitions.
      for( TransSet::callIterator it = addTransSet.beginCall(); 
            it != addTransSet.endCall(); it ++ )
      {
        addCall(*it);
      }
      //Add internal transitions.
      for( TransSet::internalIterator it = addTransSet.beginInternal(); 
            it != addTransSet.endInternal(); it ++ )
      {
        addInternal(*it);
      }
      //Add return transitions.
      for( TransSet::returnIterator it = addTransSet.beginReturn(); 
            it != addTransSet.endReturn(); it ++ )
      {
        addReturn(*it);
      }
    }
      
    /**
     *
     * @brief remove the given call transition from this collection of 
     *        transitions
     *
     * @param - removeTrans: the call transition to remove from the collection
     * @return false if the given call transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeCall( const Call & removeTrans )
    {
      if( callTrans.count(removeTrans) == 0 )
        return false;
        
      callTrans.erase(removeTrans);
      T_info.removeCall(removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given internal transition from the collection of 
     *        transitions
     *
     * @param - removeTrans: the internal transition to remove from the collection
     * @return false if the given internal transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternal( const Internal & removeTrans )
    {
      if( internalTrans.count(removeTrans) == 0 )
        return false;
        
      internalTrans.erase(removeTrans);
      T_info.removeIntra(removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given return transition from the collection of 
     *        transitions
     *
     * @param - removeTrans: the return transition to remove from the collection
     * @return false if the given return transition does not exist in the 
     *          collection, true otherwise.
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturn( const Return & removeTrans )
    {
      if( returnTrans.count(removeTrans) == 0 )
        return false;
        
      returnTrans.erase(removeTrans);
      T_info.removeRet(removeTrans);

      return true;
    }
     
    /**
     *
     * @brief test whether the given call transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the call transition to check
     * @return true if the given call transition is in this collection of 
     *          transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isCall( const Call & trans ) const
    {
      return (callTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test whether the given internal transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the internal transition to check
     * @return true if the given internal transition is in this collection of 
     *          transitions associated with the NWA
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isInternal( const Internal & trans ) const
    {
      return (internalTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test whether the given return transition is in this collection of 
     *        transitions 
     *
     * @param - trans: the return transition to check
     * @return true if the given return transition is in this collection of 
     *          transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isReturn( const Return & trans ) const
    {
      return (returnTrans.count(trans) > 0);
    } 
         
    //Utilities	

    /**
     *
     * @brief print the collection of transitions
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::ostream & TransSet<St,Sym,Call,Internal,Return>::print( std::ostream & o ) const
    {    
      //Print call transitions.
      o << "Delta_c: \n" << "{ \n";
      callIterator cit = callTrans.begin();
      callIterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        if( !first )
          o << ", \n";
        o << "(";
        (*cit).first.print(o);
        o << ", ";
        (*cit).second.print(o);
        o << ", "; 
        (*cit).third.print(o);
        o << ")";
        first=false;
      }
      o << " \n}\n";
      //Print internal transitions.
      o << "Delta_i:\n" << "{\n";
      internalIterator iit = internalTrans.begin();
      internalIterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        //assert(*iit);
        if( !first )
          o << ", \n";
        o << "(";
        (*iit).first.print(o);
        o << ", ";
        (*iit).second.print(o);
        o << ", ";
        (*iit).third.print(o);
        o << ")";
        first = false;
      }
      o << " \n}\n";
      //Print return transitions.
      o << "Delta_r:\n" << "{\n";
      returnIterator rit = returnTrans.begin();
      returnIterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if( !first )
          o << ",\n";
        o << "(";
        (*rit).first.print(o);
        o << ", ";
        (*rit).second.print(o);
        o << ", "; 
        (*rit).third.print(o);
        o << ", ";
        (*rit).fourth.print(o);
        o << ")";
        first = false;
      }
      o << "\n}\n";
      
      return o;
    }

    /*
     * 
     * @brief print the collection of transitions in dot format
     *
     * @param - o: the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    std::ostream & TransSet<St,Sym,Call,Internal,Return>::print_dot( std::ostream & o) const
    {    
      //Print call transitions.
      o << "//Delta_c: \n";
      callIterator cit = callTrans.begin();
      callIterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        (*cit).first.print(o << "\"") << "\"";
        o << "->";
        (*cit).third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        (*cit).second.print(o);
        o << "\"";
        o << " color=green";
        o << "];\n";
        
      }
      o << " \n";
      //Print internal transitions.
      o << "// Delta_i:\n" ;
      internalIterator iit = internalTrans.begin();
      internalIterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        (*iit).first.print(o << "\"") << "\"";
        o << "->";
        (*iit).third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        (*iit).second.print(o);
        o << "\"";
        o << "];\n";
      }
      o << " \n";
      //Print return transitions.
      o << "// Delta_r:\n";
      returnIterator rit = returnTrans.begin();
      returnIterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        //dummy
        o << "\"" << std::dec << (*rit).second.getName() << std::dec << ", " << (*rit).first.getName() << "\"";
        o <<"[ shape=box ];\n";

        // exit to dummy
        (*rit).first.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << (*rit).second.getName() <<", " << (*rit).first.getName() << "\"";
        o << "[";
        o << " label=\"";
        (*rit).third.print(o);
        o << "\"";
        o << " color=red";
        o << "];\n";

        // call to dummy
        (*rit).second.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << (*rit).second.getName() <<", " << (*rit).first.getName() << "\"";
        o << "[";
        o << " color=blue";
        o << "];\n";

        //dummy to ret
        o << "\"" << (*rit).second.getName() <<", " << (*rit).first.getName() << "\"";
        o << "->";
        (*rit).fourth.print(o << "\"") << "\"";
        o << "[ style=dotted ];\n";

      }
      o << "\n";
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of transitions is equivalent 
     *        to the collection of transitions 'other'
     *
     * @param - other: the TransSet to compare this TransSet to
     * @return true if this TransSet is equivalent to the TransSet 'other'
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::operator==( const TransSet<St,Sym,Call,Internal,Return> & other ) const
    { //TODO: Q: Do I want a deeper check here?
      return (  (callTrans == other.callTrans) &&
                (internalTrans == other.internalTrans) &&
                (returnTrans == other.returnTrans) );
    }

    /**
     *
     * @brief provides access to the call transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the call transitions
     *          in the collection of transitions
     *
     */  
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::beginCall() const
    {
      return callTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the internal transitions
     *          in the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::beginInternal() const
    {
      return internalTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     *        through an iterator
     *
     * @return the starting point of an iterator through the return transitions
     *          in the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::beginReturn() const
    {
      return returnTrans.begin();
    }
      
    /**
     *
     * @brief provides access to the call transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the call transitions in
     *          the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::endCall() const
    {
      return callTrans.end();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the internal transitions in
     *          the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::endInternal() const
    {
      return internalTrans.end();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     *        through an iterator
     *
     * @return the exit point of an iterator through the return transitions in
     *          the collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::endReturn() const
    {
      return returnTrans.end();
    }
    
    /**
     *
     * @brief returns the number of call transitions in this collection of
     *        transitions
     *
     * @return the number of call transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeCall( ) const
    {
      return callTrans.size();
    }
        
    /**
     *
     * @brief returns the number of internal transitions in this collection of
     *        transitions 
     *
     * @return the number of internal transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeInternal( ) const
    {
      return internalTrans.size();
    }
        
    /**
     *
     * @brief returns the number of return transitions in this collection of
     *        transitions
     *
     * @return the number of return transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeReturn( )  const
    {
      return returnTrans.size();
    }
        
    /**
     *
     * @brief returns the total number of transitions (call, internal, and return) in 
     * this collection of transitions
     *
     * @return the total number of transitions in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    size_t TransSet<St,Sym,Call,Internal,Return>::size( ) const
    {
      return (sizeCall() + sizeInternal() + sizeReturn());
    }
    
    /**
     * 
     * @brief returns all outgoing internal transitions for the given state
     *
     * @param - state: the source state
     * @return the set of all outgoing internal transitions for the given state
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Internal> TransSet<St,Sym,Call,Internal,Return>::getTransFrom( const St & state ) const
    {
      return T_info.fromTrans( state );
    }
    
    /**
     * 
     * @brief retuans all incoming internal transitions for the given state
     *
     * @param - state: the target state
     * @return the set of all incoming internal transitions for the given state
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Internal> TransSet<St,Sym,Call,Internal,Return>::getTransTo( const St & state ) const
    {
      return T_info.toTrans( state );
    }
    
    /**
     * 
     * @brief returns all call transitions for the given call site
     *
     * @param - state: the call site
     * @return the set of all call transitions for the given call site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Call> TransSet<St,Sym,Call,Internal,Return>::getTransCall( const St & state ) const
    {
      return T_info.callTrans( state );
    }
    
    /**
     * 
     * @brief returns all call transitions for the given entry point
     *
     * @param - state: the entry point
     * @return the set of all call transitions for the given entry point
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Call> TransSet<St,Sym,Call,Internal,Return>::getTransEntry( const St & state ) const
    {
      return T_info.entryTrans( state );
    }
    
    /**
     * 
     * @brief returns all return transitions for the given exit point
     *
     * @param - state: the exit point
     * @return the set of all return transitions for the given exit point
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Return> TransSet<St,Sym,Call,Internal,Return>::getTransExit( const St & state ) const
    {
      return T_info.exitTrans( state );
    }
    
    /**
     * 
     * @brief returns all return transitions for the given call predecessor
     *
     * @param - state: the call predecessor
     * @return the set of all return transitions for the given call predecessor
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Return> TransSet<St,Sym,Call,Internal,Return>::getTransPred( const St & state ) const
    {
      return T_info.predTrans( state );
    }
    
    /**
     * 
     * @brief returns all return transitions for the given return site
     *
     * @param - state: the return site
     * @return the set of all return transitions for the given return site
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    std::set<Return> TransSet<St,Sym,Call,Internal,Return>::getTransRet( const St & state ) const
    {
      return T_info.retTrans( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the source of any internal 
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the source of some internal 
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isFrom( const St & state ) const
    {
      return T_info.isFrom( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the target of any internal
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the target of some internal
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isTo( const St & state ) const
    { 
      return T_info.isTo( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the call site of any call 
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the call site of some call
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isCall( const St & state ) const
    {
      return T_info.isCall( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the entry point of any call
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the entry point of some call 
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isEntry( const St & state ) const
    {
      return T_info.isEntry( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the exit point of any return
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the exit point of some return
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isExit( const St & state ) const
    {
      return T_info.isExit( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the call predecessor for any
     *        return transition
     *
     * @param - state: the state to test
     * @return true if the given state is the call predecessor of some return
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isPred( const St & state ) const
    {
      return T_info.isPred( state );
    }
    
    /**
     * 
     * @brief tests whether the given state is the return site of any return
     *        transition
     *
     * @param - state: the state to test
     * @return true if the given state is the return site of some return 
     *          transition, false otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::isRet( const St & state ) const
    {
      return T_info.isRet( state );
    }

    /** 
     *  TODO: use refptr<St> 
     * @brief removes all transitions involving the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeTransWith( const St * state )
    {
      bool found = false;
      found = found || removeCallTransWith(state);
      found = found || removeInternalTransWith(state);
      found = found || removeReturnTransWith(state);
      return found;
    }
      
    /** 
     *
     * @brief removes all call transitions to or from the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransWith( const St & state )
    {  
      Calls removeTrans = Calls();
      //Find all transitions to remove.
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( ((*cit).first == state) ||
            ((*cit).third == state) )
            removeTrans.insert(*cit);
      }     
      //Remove the transitions.
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all internal transitions to or from the given state
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransWith( const St & state )
    {
      Internals removeTrans = Internals();
      //Find transitions to remove.
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( ((*iit).first == state) ||
            ((*iit).third == state) )
            removeTrans.insert(*iit);
      }     
      //Remove transitions.
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all return transitions to or from the given state
     *        as well as return transitions corresponding to calls from
     *        that state
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransWith( const St & state )
    {
      Returns removeTrans = Returns();
      //Find transitions to remove.
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( ((*rit).first == state) ||
            ((*rit).second == state) ||
            ((*rit).fourth == state) )
            removeTrans.insert(*rit);
      }     
      //Remove transitions.
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0;
    }
    
    /** 
     *
     * @brief removes all call transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransSym( const Sym & sym )
    {
      Calls removeTrans = Calls();
      //Find transitions to remove.
      for( callIterator cit = callTrans.begin();
            cit != callTrans.end(); cit++ )
      {
        if( (*cit).second == sym )
          removeTrans.insert(*cit);
      }     
      //Remove transitions.
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;  
    }
    
    /** 
     *
     * @brief removes all internal transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransSym( const Sym & sym )
    {
      Internals removeTrans = Internals();
      //Find transitions to remove.
      for( internalIterator iit = internalTrans.begin();
            iit != internalTrans.end(); iit++ )
      {
        if( (*iit).second == sym )
          removeTrans.insert(*iit);
      }     
      //Remove transitions.
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /** 
     *
     * @brief removes all return transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransSym( const Sym & sym )
    {
      Returns removeTrans = Returns();
      //Find transitions to remove.
      for( returnIterator rit = returnTrans.begin();
            rit != returnTrans.end(); rit++ )
      {
        if( (*rit).third == sym )
          removeTrans.insert(*rit);
      }     
      //Remove transitions.
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     * TODO optimize
     * @brief test if there exists a call transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the call transition
     * @param - sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     *          symbol in this collection of transitions 
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::callExists( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit.first == from) && (cit.second == sym) )
          return true;
      }  
      return false;    
    }
    
    /**
     * TODO optimize
     * @brief provides access to all call transitions with the given from state
     *        and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the call transitions
     * @param - sym: the desired symbol for the call transitions
     * @return the set of call transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls TransSet<St,Sym,Call,Internal,Return>::getCalls( St * from, Sym * sym ) const 
    {
      assert(from);
      assert(sym);
      Calls result;
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( ((*cit).first == from) && ((*cit).second == sym) )
          result.insert(*cit);
      } 
      return result;
    }
    
    /**
     * TODO optimize
     * @brief test if there exists an internal transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the internal transition
     * @param - sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::internalExists( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit.first == from) && (iit.second == sym) )
          return true;    
      }     
      return false;
    }
    
    /**
     * TODO optimize
     * @brief provides access to all internal transitions with the given from 
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @param - sym: the desired symbol for the internal transitions
     * @return the set of internal transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals TransSet<St,Sym,Call,Internal,Return>::getInternals( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      Internals result;
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( ((*iit).first == from) && ((*iit).second == sym) )
          result.insert(*iit);
      } 
      return result;
    }
    
    /**
     * TODO this is redundant
     * @brief provides access to all internal transitions with the given 
     *        from state in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @return the set of internal transitions with the given from state
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals TransSet<St,Sym,Call,Internal,Return>::getInternalsFrom( St * from ) const
    {
      assert(from);
      return fromTrans(*from);
    }


    /**
     * TODO optimize
     * @brief test if there exists a return transition with the given from state, 
     *        predecessor state, and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the return transition
     * @param - pred: the desired predecessor state for the return transition
     * @param - sym: the desired symbol for the return transition
     * @return true if there exists a return transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    bool TransSet<St,Sym,Call,Internal,Return>::returnExists( St * from, St * pred, Sym * sym ) const
    {
      assert(from);
      assert(pred);
      assert(sym);
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit.first == from) && (rit.second == pred) && (rit.third == sym) )
            return true;      
      }     
      return false;
    }   
    
    /**
     * TODO optimize
     * @brief provides access to all return transitions with the given from
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the return transitions
     * @param - sym: the desired symbol for the return transitions
     * @return the set of return transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym,typename Call,typename Internal,typename Return>
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns TransSet<St,Sym,Call,Internal,Return>::getReturns( St * from, Sym * sym ) const
    {
      assert(from);
      assert(sym);
      Returns result;
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( ((*rit).first == from) && ((*rit).third == sym) )
          result.insert(*rit);
      } 
      return result;
    } 
#endif
  }
}
#endif