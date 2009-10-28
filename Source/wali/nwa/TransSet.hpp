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

// std::c++
#include <iostream>
#include <set>

namespace wali
{
  namespace nwa
  {

    template <typename St, typename Sym, typename T, typename U, typename V>
    class TransSet : public Printable
    {
      public:
        typedef typename std::set<Sym>::iterator symbolIterator;
        typedef typename T::iterator callIterator;
        typedef typename U::iterator internalIterator;
        typedef typename V::iterator returnIterator;
      
      //
      // Methods
      //

      public:
        //Constructors and Destructor
        TransSet( );
        TransSet( const TransSet & other );
        TransSet & operator=( const TransSet & other );

        ~TransSet( );

        //Symbol Accessors

        /**
         *
         * @brief get all symbols in the symbol set associated with the NWA
         *
         * This method provides access to all symbols in the symbol set 
         * associated with the NWA.  Note: An NWA can have an unbounded number 
         * of symbols associated with it.
         *
         * @return set of all symbols associated with the NWA
         *
         */
        const std::set<Sym> & getSymbols( );

        /**
         *
         * @brief test if the given symbol is associated with the NWA
         *
         * This method tests whether the given symbol is in the symbol set
         * associated with the NWA.
         *
         * @param the symbol to check
         * @return true if the given symbol is associated with the NWA
         *
         */
        bool isSymbol( Sym sym );

        /**
         *
         * @brief add the given symbol to the NWA
         *
         * This method adds the given symbol to the symbol set associated with 
         * the NWA.  If the symbol is already associated with the NWA, false is 
         * returned. Otherwise, true is returned. Note: For bookkeeping purposes
         * a transition of each kind is added to the to the transition set for 
         * each state currently in the state set as the from state (for each pair
         * of states currently in the state set as the from state and pred state
         * in the case of return transitions) with the new symbol and the error
         * state as the to state. 
         *
         * @param the symbol to add
         * @return false if the symbol is already associated with the NWA, 
         * true otherwise
         *
         */
        bool addSymbol( Sym sym );

        /**
         *
         * @brief add the given symbols to the NWA
         *
         * This method adds all of the given symbols to the symbol set 
         * associated with the NWA.  Transitions of all kinds(call, internal, 
         * and return) with the given symbol for each pair(or triple) of 
         * states to the error state are added to the respective transition 
         * sets.  
         *
         * @param the symbols to add
         *
         */
        void addAllSymbols( TransSet addTransSet );

        /**
         *
         * @brief remove the given symbol from the NWA
         *
         * This method checks for the given symbol in the symbol set 
         * associated with the NWA.  It then removes the symbol from the
         * symbol set.  Any transitions associated with the symbol to be 
         * removed are also removed.
         *
         * @param the symbol to remove
         * @return false if the symbols is not associated with the NWA
         *
         */
        bool removeSymbol( Sym sym );

        /**
         *
         * @brief remove all symbols associated with the NWA
         *
         * This method removes all symbols associated with the NWA except 
         * the epsilon symbol.  It also removes all transitions associated 
         * with the removed symbols in the NWA.
         *
         */
        void clearSymbols( );

        /**
         *
         * @brief provide access to the beginning of the symbol set
         *
         * This method provides access to the beginning of the symbol set
         * associated with this transition set.
         *
         * @return an iterator pointing to the beginning of the symbol set
         *
         */
        symbolIterator beginSymbols();
        
        /**
         * 
         * @brief provide access to the end of the symbol set
         *
         * This method provides access to the position one past the end
         * of the symbol set associated with this transition set.
         *
         * @return an iterator pointing just past the end of the symbol set
         *
         */
        symbolIterator endSymbols();
        
        /**
         *
         * @brief returns the number of symbols associated with this NWA
         *
         * This method returns the number of symbols associated with this
         * NWA.  Note: The epsilon symbol is included in this count.
         *
         * @return the number of symbols associated with this NWA
         *
         */
        size_t sizeSymbols( );

        //Transition Accessors

        /**
         *
         * @brief removes all transitions from this collection of transitions
         *
         * This method removes all transitions from this collection of
         * transitions by redirecting all existing transitions to point to the 
         * error state.
         *
         */
        void clear( );
      
        /**
         *
         * @brief get all call transitions in the collection of transitions associated 
         * with the NWA
         *
         * This method provides access to all call transitions in the collection of
         * transitions associated with the NWA.  
         *
         * @return all call transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const T & getCalls();
        
        /**
         *
         * @brief get all internal transitions in the collection of transitions 
         * associated with the NWA
         *
         * This method provides access to all internal transitions in the collection 
         * of transitions associated with the NWA.  
         *
         * @return all internal transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const U & getInternals();
        
        /**
         *
         * @brief get all return transitions in the collection of transitions 
         * associated with the NWA
         *
         * This method provides access to all return transitions in the collection 
         * of transitions associated with the NWA.  
         *
         * @return all return transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const V & getReturns();
      
        /**
         *
         * @brief add the given call transition to the collection of transitions
         *
         * This method adds the given call transition to the collection of 
         * transitions associated with the NWA.  If the call transition with the
         * same from state and symbol but the error state as the to state exists 
         * in the collection of transitions, it is removed.  If this exact call 
         * transition already exists, false is returned. Otherwise, true is returned.
         *
         * @parm the call transition to add to the collection of transitions
         * @return false if the call transition already exists in the collection
         *
         */
        bool addCall(KeyTriple addTrans);
        
        /**
         *
         * @brief add the given internal transition to the collection of transitions
         *
         * This method adds the given internal transition to the collection of 
         * transitions associated with the NWA.  If the internal transition with the
         * same from state and symbol but the error state as the to state exists in 
         * the collection of transitions, it is removed.  If this exact internal 
         * transition already exists, false is returned. Otherwise, true is returned.
         *
         * @parm the internal transition to add to the collection of transitions
         * @return false if the internal transition already exists in the collection
         *
         */
        bool addInternal(KeyTriple addTrans);
        
        /**
         *
         * @brief add the given return transition to the collection of transitions
         *
         * This method adds the given return transition to the collection of 
         * transitions associated with the NWA.  If the return transition with the 
         * same from state, pred state, and symbol but the error state as the to 
         * state exists in the collection of transitions, it is removed.  If this 
         * exact return transition already exists, false is returned. Otherwise, 
         * true is returned.
         *
         * @parm the return transition to add to the collection of transitions
         * @return false if the return transition already exists in the collection
         *
         */
        bool addReturn(KeyQuad addTrans);
      
        /**
         *
         * @brief add all transitions in the given collection to this
         * collection of transitions
         *
         * This method adds all of the transitions in the given collection
         * of transitions to this collection of transitions.
         *
         * @parm the collection of transitions to add to this collection
         * of transitions
         *
         */
        void addAllTrans(TransSet addTransSet);
      
        /**
         *
         * @brief remove the given call transition from the collection of 
         * transitions
         *
         * This method removes the given call transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no
         * call transitions corresponding to the from state and symbol of the 
         * removed transition, a call transition with the same from state and 
         * symbol as the removed call transition and the error state as the to 
         * state is added to the NWA.  If the given call transition does not 
         * exist in the collection of transitions false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the call transition to remove from the collection
         * @return false if the given call transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeCall(KeyTriple removeTrans);
        
        /**
         *
         * @brief remove the given internal transition from the collection of 
         * transitions
         *
         * This method removes the given internal transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no 
         * internal transitions corresponding to the from state and symbol of the
         * removed transition, an internal transition with the same from state and
         * symbol as the removed internal transition and the error state as the to
         * state is added to the NWA.  If the given internal transition does not 
         * exist in the collection of transitions false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the internal transition to remove from the collection
         * @return false if the given internal transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeInternal(KeyTriple removeTrans);
        
        /**
         *
         * @brief remove the given return transition from the collection of 
         * transitions
         *
         * This method removes the given return transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no 
         * return transitions corresponding to the from state, pred state, and 
         * symbol of the removed transition, a return transition with the same 
         * from state, pred state, and symbol as the removed return transition 
         * and the error state as the to state is added to the NWA.  If the 
         * given return transition does not exist in the collection of transitions 
         * false is returned.  Otherwise, true is returned.
         *
         * @parm the return transition to remove from the collection
         * @return false if the given return transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeReturn(KeyQuad removeTrans);
                      
        /**
         *
         * @brief test if the given call transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given call transition is in the collection
         * of transitions associated with the NWA.
         *
         * @param the call transition to check
         * @return true if the given call transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isCall(KeyTriple trans);
        
        /**
         *
         * @brief test if the given internal transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given internal transition is in the 
         * collection of transitions associated with the NWA.
         *
         * @param the internal transition to check
         * @return true if the given internal transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isInternal(KeyTriple trans);
        
        /**
         *
         * @brief test if the given return transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given return transition is in the 
         * collection of transitions associated with the NWA.
         *
         * @param the return transition to check
         * @return true if the given return transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isReturn(KeyQuad trans);              
                      
        //Utilities	

        /**
         *
         * @brief print the collection of transitions
         *
         * This method prints out the transition set to the output stream 
         * provided.
         *
         * @param the output stream to print to
         * @return the output stream that was printed to
         *
         */
        std::ostream & print( std::ostream & o ) const;

        /**
         *
         * @brief tests whether this collection of transitions is equivalent 
         * to the collection of transitions 'other'
         *
         * This method tests the equivalence of this set of transitions and 
         * the set of transitions 'other'.
         *
         * @param the TransSet to compare this TransSet to
         * @return true if this TransSet is equivalent to the TransSet 'other'
         *
         */
        bool operator==( TransSet & other );

        /**
         *
         * @brief provides access to the call transitions in the collection 
         * through an iterator
         *
         * This method provides access to the call transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the call transitions
         * in the collection of transitions
         *
         */
        callIterator beginCall( );
        
        /**
         *
         * @brief provides access to the internal transitions in the collection 
         * through an iterator
         *
         * This method provides access to the internal transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the internal transitions
         * in the collection of transitions
         *
         */
        internalIterator beginInternal( );
        
        /**
         *
         * @brief provides access to the return transitions in the collection 
         * through an iterator
         *
         * This method provides access to the return transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the return transitions
         * in the collection of transitions
         *
         */
        returnIterator beginReturn( );
      
        /**
         *
         * @brief provides access to the call transitions in the collection 
         * through an iterator
         *
         * This method provides access to the call transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the call transitions in
         * the collection of transitions
         *
         */
         callIterator endCall( );
         
         /**
         *
         * @brief provides access to the internal transitions in the collection 
         * through an iterator
         *
         * This method provides access to the internal transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the internal transitions in
         * the collection of transitions
         *
         */
         internalIterator endInternal( );
         
         /**
         *
         * @brief provides access to the return transitions in the collection 
         * through an iterator
         *
         * This method provides access to the return transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the return transitions in
         * the collection of transitions
         *
         */
         returnIterator endReturn( );

        /**
         *
         * @brief returns the number of call transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of call transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not 
         * include any transition to or from the error state. 
         *
         * @return the number of call transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeCall( );
        
        /**
         *
         * @brief returns the number of internal transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of internal transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not include
         * any transition to or from the error state.
         *
         * @return the number of internal transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeInternal( );
        
        /**
         *
         * @brief returns the number of return transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of return transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not include
         * any transition to or from the error state or with the error state as the
         * pred state.
         *
         * @return the number of return transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeReturn( );
        
        /**
         *
         * @brief returns the total number of transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the total number of transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not 
         * include any transitions involving the error state.  This is equivalent 
         * to counting all transitions explicitly added to the NWA.
         *
         * @return the total number of transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t size( );

        /** 
         *
         * @brief removes all call transitions to or from the state with 
         * the given name  
         *
         * This method removes all call transitions to or from the state 
         * with the given name. If no call transitions exist to or from 
         * this state false is returned.  Otherwise, true is returned.
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeCallTransWith( St name );
      
        /** 
         *
         * @brief removes all internal transitions to or from the state
         * with the given name
         *
         * This method removes all internal transitions to or from the
         * state with the given name.  If no internal transitions exist
         * to or from this state false is returned.  Otherwise, true is 
         * returned. 
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeInternalTransWith( St name );
      
        /** 
         *
         * @brief removes all return transitions to or from the state with
         * the given name as well as return transitions corresponding to 
         * calls from the state with the given name
         *
         * This method removes all return transitions to or from the state
         * with the given name as well as return transitions corresponding
         * to calls from the state with the given name.  If no return 
         * transitions exist to or from this state false is returned.  
         * Otherwise, true is returned.
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeReturnTransWith( St name );
      
      private:
        /** 
         *
         * @brief removes all call transitions with the given symbol 
         *
         * This method removes all call transitions with the given symbol. 
         * If no call transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeCallTransSym(Sym sym);
        
        /** 
         *
         * @brief removes all internal transitions with the given symbol 
         *
         * This method removes all internal transitions with the given symbol. 
         * If no internal transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeInternalTransSym(Sym sym);
        
        /** 
         *
         * @brief removes all return transitions with the given symbol 
         *
         * This method removes all return transitions with the given symbol. 
         * If no return transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeReturnTransSym(Sym sym);
      
        /**
         *
         * @brief test if there exists a call transition with the given from state 
         * and symbol in the collection of transitions associated with the NWA
         *
         * This method tests whether there exists a call transition with the given 
         * from state and symbol but not the error state as the to state in the 
         * collection of transitions associated with the NWA.
         *
         * @param from: the desired from state for the call transition
         * @param sym: the desired symbol for the call transition
         * @return true if there exists a call transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool callExists(Key from,Key sym);
        
        /**
         *
         * @brief test if there exists an internal transition with the given from state 
         * and symbol in the collection of transitions associated with the NWA
         *
         * This method tests whether there exists an internal transition with the given 
         * from state and symbol but not the error state as the to state in the collection 
         * of transitions associated with the NWA.
         *
         * @param from: the desired from state for the internal transition
         * @param sym: the desired symbol for the internal transition
         * @return true if there exists an internal transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool internalExists(Key from,Key sym);
        
        /**
         *
         * @brief test if there exists a return transition with the given from state, 
         * predecessor state, and symbol in the collection of transitions associated 
         * with the NWA
         *
         * This method tests whether there exists a return transition with the given 
         * from state, predecessor state, and symbol but not the error state as the to
         * state in the collection of transitions associated with the NWA.
         *
         * @param from: the desired from state for the return transition
         * @param pred: the desired predecessor state for the return transition
         * @param sym: the desired symbol for the return transition
         * @return true if there exists a return transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool returnExists(Key from, Key pred, Key sym);

      //
      // Variables
      //
      
      protected: 
        std::set<Sym> symbols;
        T callTrans;
        U internalTrans;
        V returnTrans;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template < typename St,typename Sym,typename T,typename U, typename V >
    TransSet<St,Sym,T,U,V>::TransSet( )
    {
          
      symbols = std::set<Sym>();
      callTrans = T();
      internalTrans = U();
      returnTrans = V();
      
      addSymbol(Sym::getEpsilon());  
    }
    
    template < typename St,typename Sym,typename T,typename U, typename V >
    TransSet<St,Sym,T,U,V>::TransSet( const TransSet<St,Sym,T,U,V> & other )
    {
      symbols = other.symbols;
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
    }
    
    template < typename St,typename Sym,typename T,typename U, typename V >
    TransSet<St,Sym,T,U,V> & TransSet<St,Sym,T,U,V>::operator=( const TransSet<St,Sym,T,U,V> & other )
    {
      symbols = other.symbols;
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      return *this;
    }

   
    template < typename St,typename Sym,typename T,typename U, typename V >
    TransSet<St,Sym,T,U,V>::~TransSet( ) 
    {
      clear(); 
    }
    
    //Symbol Accessors

    /**
     *
     * @brief get all symbols in the symbol set of the NWA
     *
     * @return set of Key values for all symbols associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    const std::set<typename Sym> & TransSet<St,Sym,T,U,V>::getSymbols( )
    {
      return symbols;
    }
  
    /**
     *
     * @brief test if the given symbol is a symbol of the NWA
     *
     * @param the symbol to check
     * @return true if the given symbol is a symbol of the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::isSymbol( Sym sym )
    {
        return (symbols.count(sym) >  0);
    }
    
   
    /**
     *
     * @brief add the given symbol to the NWA
     *
     * @param symbol to add
     * @return false if the symbol is already associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::addSymbol( Sym sym )
    {
      if( symbols.count(sym) > 0 )
        return false;
      symbols.insert(sym);
      return true;
     
    }
    
    
    /**
     *
     * @brief add the given symbols to the NWA
     *
     * @param the symbols to add
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    void TransSet<St,Sym,T,U,V>::addAllSymbols( TransSet<St,Sym,T,U,V> addTransSet )
    {
      for( iterator it = addTransSet.symbols.begin();
            it != addTransSet.symbols.end(); it++ )
      {
        symbols.insert(*it);
      }
    }
    
   
    /**
     *
     * @brief remove the given symbol from the NWA
     *
     * @param the symbol to remove
     * @return false if the symbol is no associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeSymbol( Sym sym )
    {
      //Remove transitions associated with the state that was removed.
      if( symbols.count(sym) == 0 )
        return false;
      else if( symbols.erase(sym) )
      {
        removeCallTransSym(sym);
        removeInternalTransSym(sym);
        removeReturnTransSym(sym);
        
        return true;
      }      
      return false;
    }
    
   
    /**
     *
     * @brief remove all symbols associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    void TransSet<St,Sym,T,U,V>::clearSymbols( )
    {
      symbols.clear();

      //Since all symbols are being removed, all transitions are removed 
      //as well.
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      //The epsilon symbol should always remain in the symbol set.
      //This will also replace transitions with the epsilon symbol for each 
      //state that exists in the state set to the error state .
      addSymbol(Sym::getEpsilon());
    }
    
    /**
     *
     * @brief provide access to the beginning of the symbol set
     *
     * @return an iterator pointing to the beginning of the symbol set
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::symbolIterator TransSet<St,Sym,T,U,V>::beginSymbols()
    {
      return symbols.begin();
    }
    
    /**
     * 
     * @brief provide access to the end of the symbol set
     *
     * @return an iterator pointing just past the end of the symbol set
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::symbolIterator TransSet<St,Sym,T,U,V>::endSymbols()
    {
      return symbols.end();
    }
    
    /**
     *
     * @brief returns the number of symbols associated with this NWA
     *
     * @return the number of symbols associated with this NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    size_t TransSet<St,Sym,T,U,V>::sizeSymbols( )
    {
      return symbols.size();
    }

    //Transition Accessors

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    void TransSet<St,Sym,T,U,V>::clear( )
    {
      symbols.clear();
      
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
    }
    
    /**
     *
     * @brief get all call transitions in the collection of transitions associated 
     * with the NWA
     *
     * @return all call transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    const typename T & TransSet<St,Sym,T,U,V>::getCalls()
    {
      return callTrans;
    }
    
    /**
     *
     * @brief get all internal transitions in the collection of transitions 
     * associated with the NWA
     *
     * @return all internal transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    const typename U & TransSet<St,Sym,T,U,V>::getInternals()
    {
      return internalTrans;
    }
    
    /**
     *
     * @brief get all return transitions in the collection of transitions 
     * associated with the NWA
     *
     * @return all return transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    const typename V & TransSet<St,Sym,T,U,V>::getReturns()
    {
      return returnTrans;
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @parm the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */   
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::addCall(KeyTriple addTrans)
    {
      if(! isSymbol(addTrans.second) )
        addSymbol(addTrans.second);
      if( callTrans.count(addTrans) > 0 )
        return false;      
      
      callTrans.insert(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @parm the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::addInternal(KeyTriple addTrans)
    {
      if(! isSymbol(addTrans.second) )
        addSymbol(addTrans.second);  
      if( internalTrans.count(addTrans) > 0 )
        return false;
      internalTrans.insert(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @parm the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::addReturn(KeyQuad addTrans)
    {
      if(! isSymbol(addTrans.third) )
        addSymbol(addTrans.third);
      if( returnTrans.count(addTrans) > 0 )
        return false;
      returnTrans.insert(addTrans);

      return true;
    }
      
    /**
     *
     * @brief add all transitions in the given collection to this
     * collection of transitions
     *
     * @parm the collection of transitions to add to this collection
     * of transitions
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    void TransSet<St,Sym,T,U,V>::addAllTrans(TransSet<St,Sym,T,U,V> addTransSet)
    {    
      //Any symbols not in the original set of symbols must be added.
      addAllSymbols(addTransSet);
    
      for( TransSet::callIterator it = addTransSet.beginCall(); 
            it != addTransSet.endCall(); it ++ )
      {
        addCall(*it);
      }
      for( TransSet::internalIterator it = addTransSet.beginInternal(); 
            it != addTransSet.endInternal(); it ++ )
      {
        addInternal(*it);
      }
      for( TransSet::returnIterator it = addTransSet.beginReturn(); 
            it != addTransSet.endReturn(); it ++ )
      {
        addReturn(*it);
      }
    }
      
    /**
     *
     * @brief remove the given call transition from the collection of 
     * transitions
     *
     * @parm the call transition to remove from the collection
     * @return false if the given call transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeCall(KeyTriple removeTrans)
    {
      if( callTrans.count(removeTrans) == 0 )
        return false;
      callTrans.erase(removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given internal transition from the collection of 
     * transitions
     *
     * @parm the internal transition to remove from the collection
     * @return false if the given internal transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeInternal(KeyTriple removeTrans)
    {
      if( internalTrans.count(removeTrans) == 0 )
        return false;
      internalTrans.erase(removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given return transition from the collection of 
     * transitions
     *
     * @parm the return transition to remove from the collection
     * @return false if the given return transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeReturn(KeyQuad removeTrans)
    {
      if( returnTrans.count(removeTrans) == 0 )
        return false;
      returnTrans.erase(removeTrans);

      return true;
    }
     
    /**
     *
     * @brief test if the given call transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the call transition to check
     * @return true if the given call transition is in the collection of 
     * transitions associated with the NWA
     *
     */   
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::isCall(KeyTriple trans)
    {
      return (callTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test if the given internal transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the internal transition to check
     * @return true if the given internal transition is in the collection of 
     * transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::isInternal(KeyTriple trans)
    {
      return (internalTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test if the given return transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the return transition to check
     * @return true if the given return transition is in the collection of 
     * transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::isReturn(KeyQuad trans)
    {
      return (returnTrans.count(trans) > 0);
    } 
         
    //Utilities	

    /**
     *
     * @brief print the collection of transitions
     *
     * @param the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    std::ostream & TransSet<St,Sym,T,U,V>::print( std::ostream & o) const
    {    
      o << "Sigma: " << "{ ";
      symbols.print(o);
      o << " }\n";
      
      o << "Delta_c: " << "{ ";
      T::const_iterator cit = callTrans.begin();
      T::const_iterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        printKey(o,cit->first);
        o << ", ";
        printKey(o,cit->second);
        o << ", "; 
        printKey(o,cit->third);
        o << ")";
        first=false;
      }
      o << " }\n";
      
      o << "Delta_i: " << "{ ";
      U::const_iterator iit = internalTrans.begin();
      U::const_iterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        printKey(o,iit->first);
        o << ", ";
        printKey(o,iit->second);
        o << ", ";
        printKey(o,iit->third);
        o << ")";
        first = false;
      }
      o << " }\n";
      
      o << "Delta_r: " << "{ ";
      V::const_iterator rit = returnTrans.begin();
      V::const_iterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        printKey(o,rit->first);
        o << ", ";
        printKey(o,rit->second);
        o << ", "; 
        printKey(o,rit->third);
        o << ", ";
        printKey(o,rit->fourth);
        o << ")";
        first = false;
      }
      o << " }\n";
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of transitions is equivalent 
     * to the collection of transitions 'other'
     *
     * @param the TransSet to compare this TransSet to
     * @return true if this TransSet is equivalent to the TransSet 'other'
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::operator==( TransSet<St,Sym,T,U,V> & other )
    {
      return (  (symbols == other.symbols) &&
                (callTrans == other.callTrans) &&
                (internalTrans == other.internalTrans) &&
                (returnTrans == other.returnTrans) );
    }

    /**
     *
     * @brief provides access to the call transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the call transitions
     * in the collection of transitions
     *
     */    
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::callIterator TransSet<St,Sym,T,U,V>::beginCall()
    {
      return callTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the internal transitions
     * in the collection of transitions
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::internalIterator TransSet<St,Sym,T,U,V>::beginInternal()
    {
      return internalTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the return transitions
     * in the collection of transitions
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::returnIterator TransSet<St,Sym,T,U,V>::beginReturn()
    {
      return returnTrans.begin();
    }
      
    /**
     *
     * @brief provides access to the call transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the call transitions in
     * the collection of transitions
     *
     */    
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::callIterator TransSet<St,Sym,T,U,V>::endCall()
    {
      return callTrans.end();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the internal transitions in
     * the collection of transitions
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::internalIterator TransSet<St,Sym,T,U,V>::endInternal()
    {
      return internalTrans.end();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the return transitions in
     * the collection of transitions
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    typename TransSet<St,Sym,T,U,V>::returnIterator TransSet<St,Sym,T,U,V>::endReturn()
    {
      return returnTrans.end();
    }
    
    /**
     *
     * @brief returns the number of call transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of call transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    size_t TransSet<St,Sym,T,U,V>::sizeCall( )
    {
      return callTrans.size();
    }
        
    /**
     *
     * @brief returns the number of internal transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of internal transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V > 
    size_t TransSet<St,Sym,T,U,V>::sizeInternal( )
    {
      return internalTrans.size();
    }
        
    /**
     *
     * @brief returns the number of return transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of return transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    size_t TransSet<St,Sym,T,U,V>::sizeReturn( )
    {
      return returnTrans.size();
    }
        
    /**
     *
     * @brief returns the total number of transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the total number of transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    size_t TransSet<St,Sym,T,U,V>::size( )
    {
      return (sizeCall() + sizeInternal() + sizeReturn());
    }
      
    /** 
     *
     * @brief removes all call transitions to or from the state with 
     * the given name  
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeCallTransWith( St name )
    {
      T removeTrans = T();
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == name.getKey()) ||
            (cit->third == name.getKey()) )
            removeTrans.insert(*cit);
      }     
      
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all internal transitions to or from the state
     * with the given name
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeInternalTransWith( St name )
    {
      U removeTrans = U();
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == name.getKey()) ||
            (iit->third == name.getKey()) )
            removeTrans.insert(*iit);
      }     
      
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all return transitions to or from the state with
     * the given name as well as return transitions corresponding to 
     * calls from the state with the given name
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeReturnTransWith( St name )
    {
      V removeTrans = V();
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == name.getKey()) ||
            (rit->second == name.getKey()) ||
            (rit->fourth == name.getKey()) )
            removeTrans.insert(*rit);
      }     
      
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
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeCallTransSym(Sym sym)
    {
      std::set< KeyTriple > removeTrans = std::set< KeyTriple >();
      for( std::set< KeyTriple >::iterator cit = callTrans.begin();
            cit != callTrans.end(); cit++ )
      {
        if( cit->second == sym.getLabelKey() )
          removeTrans.insert(*cit);
      }     
      
      for( std::set< KeyTriple >::iterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        callTrans.erase(*rit);
      }
      
      return removeTrans.size() > 0;  
    }
    
    /** 
     *
     * @brief removes all internal transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeInternalTransSym(Sym sym)
    {
      std::set< KeyTriple > removeTrans = std::set< KeyTriple >();
      for( std::set< KeyTriple >::iterator iit = internalTrans.begin();
            iit != internalTrans.end(); iit++ )
      {
        if( iit->second == sym.getLabelKey() )
          removeTrans.insert(*iit);
      }     
      
      for( std::set< KeyTriple >::iterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        internalTrans.erase(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /** 
     *
     * @brief removes all return transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::removeReturnTransSym(Sym sym)
    {
      std::set< KeyQuad > removeTrans = std::set< KeyQuad >();
      for( std::set< KeyQuad >::iterator rit = returnTrans.begin();
            rit != returnTrans.end(); rit++ )
      {
        if( rit->third == sym.getLabelKey() )
          removeTrans.insert(*rit);
      }     
      
      for( std::set< KeyQuad >::iterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        returnTrans.erase(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     *
     * @brief test if there exists a call transition with the given from state 
     * and symbol in the collection of transitions associated with the NWA
     *
     * @param from: the desired from state for the call transition
     * @param sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::callExists(Key from,Key sym)
    {
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit->first == from) && (cit->second == sym) )
          return true;
      }  
      return false;    
    }
    
    /**
     *
     * @brief test if there exists an internal transition with the given from state 
     * and symbol in the collection of transitions associated with the NWA
     *
     * @param from: the desired from state for the internal transition
     * @param sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::internalExists(Key from,Key sym)
    {
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == from) && (iit->second == sym) )
          return true;    
      }     
      return false;
    }
    
    /**
     *
     * @brief test if there exists a return transition with the given from state, 
     * predecessor state, and symbol in the collection of transitions associated 
     * with the NWA
     *
     * @param from: the desired from state for the return transition
     * @param pred: the desired predecessor state for the return transition
     * @param sym: the desired symbol for the return transition
     * @return true if there exists a return transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename T,typename U, typename V >
    bool TransSet<St,Sym,T,U,V>::returnExists(Key from, Key pred, Key sym)
    {
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == from) && (rit->second == pred) && (rit->third == sym) )
            return true;      
      }     
      return false;
    }    
  }
}
#endif