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

    template <typename St, typename Sym, typename Call, typename Internal, typename Return>
    class TransSet : public Printable
    {
      //TODO: update comments
      public:        
        typedef std::set< Call > Calls;
        typedef std::set< Internal > Internals;
        typedef std::set< Return > Returns;
        typedef typename Calls::iterator callIterator;
        typedef typename Internals::iterator internalIterator;
        typedef typename Returns::iterator returnIterator;
      
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
        const Calls & getCalls();
        
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
        const Internals & getInternals();
        
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
        const Returns & getReturns();
      
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
        bool addCall(Call addTrans);
        
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
        bool addInternal(Internal addTrans);
        
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
        bool addReturn(Return addTrans);
      
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
        bool removeCall(Call removeTrans);
        
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
        bool removeInternal(Internal removeTrans);
        
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
        bool removeReturn(Return removeTrans);
                      
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
        bool isCall(Call trans);
        
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
        bool isInternal(Internal trans);
        
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
        bool isReturn(Return trans);              
                      
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
        bool callExists(St from,Sym sym);
        
        const Calls getCalls(St from,Sym sym);
        
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
        bool internalExists(St from,Sym sym);
        
        const Internals getInternals(St from,Sym sym);
        
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
        bool returnExists(St from, St pred, Sym sym);
        
        const Returns getReturns(St from, Sym sym);

      //
      // Variables
      //
      
      protected: 
        Calls callTrans;
        Internals internalTrans;
        Returns returnTrans;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return>::TransSet( )
    {
      callTrans = Calls();
      internalTrans = Internals();
      returnTrans = Returns();  
    }
    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return>::TransSet( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
    }
    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return> & TransSet<St,Sym,Call,Internal,Return>::operator=( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      return *this;
    }

   
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return>::~TransSet( ) 
    {
      clear(); 
    }

    //Transition Accessors

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    void TransSet<St,Sym,Call,Internal,Return>::clear( )
    {
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls & TransSet<St,Sym,Call,Internal,Return>::getCalls()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals & TransSet<St,Sym,Call,Internal,Return>::getInternals()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns & TransSet<St,Sym,Call,Internal,Return>::getReturns()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::addCall(Call addTrans)
    {
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::addInternal(Internal addTrans)
    {
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::addReturn(Return addTrans)
    {
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    void TransSet<St,Sym,Call,Internal,Return>::addAllTrans(TransSet<St,Sym,Call,Internal,Return> addTransSet)
    {   
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeCall(Call removeTrans)
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternal(Internal removeTrans)
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturn(Return removeTrans)
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isCall(Call trans)
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isInternal(Internal trans)
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isReturn(Return trans)
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    std::ostream & TransSet<St,Sym,Call,Internal,Return>::print( std::ostream & o) const
    {    
      o << "Delta_c: " << "{ ";
      Calls::const_iterator cit = callTrans.begin();
      Calls::const_iterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        cit->first.print(o);
        o << ", ";
        cit->second.print(o);
        o << ", "; 
        cit->third.print(o);
        o << ")";
        first=false;
      }
      o << " }\n";
      
      o << "Delta_i: " << "{ ";
      Internals::const_iterator iit = internalTrans.begin();
      Internals::const_iterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        iit->first.print(o);
        o << ", ";
        iit->second.print(o);
        o << ", ";
        iit->third.print(o);
        o << ")";
        first = false;
      }
      o << " }\n";
      
      o << "Delta_r: " << "{ ";
      Returns::const_iterator rit = returnTrans.begin();
      Returns::const_iterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        rit->first.print(o);
        o << ", ";
        rit->second.print(o);
        o << ", "; 
        rit->third.print(o);
        o << ", ";
        rit->fourth.print(o);
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::operator==( TransSet<St,Sym,Call,Internal,Return> & other )
    {
      return (  (callTrans == other.callTrans) &&
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::beginCall()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::beginInternal()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::beginReturn()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::endCall()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::endInternal()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::endReturn()
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeCall( )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return > 
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeInternal( )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeReturn( )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    size_t TransSet<St,Sym,Call,Internal,Return>::size( )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransWith( St name )
    {
      Calls removeTrans = Calls();
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == name) ||
            (cit->third == name) )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransWith( St name )
    {
      Internals removeTrans = Internals();
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == name) ||
            (iit->third == name) )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransWith( St name )
    {
      Returns removeTrans = Returns();
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == name) ||
            (rit->second == name) ||
            (rit->fourth == name) )
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransSym(Sym sym)
    {
      Calls removeTrans = Calls();
      for( callIterator cit = callTrans.begin();
            cit != callTrans.end(); cit++ )
      {
        if( cit->second == sym )
          removeTrans.insert(*cit);
      }     
      
      for( callIterator rit = removeTrans.begin();
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransSym(Sym sym)
    {
      Internals removeTrans = Internals();
      for( internalIterator iit = internalTrans.begin();
            iit != internalTrans.end(); iit++ )
      {
        if( iit->second == sym )
          removeTrans.insert(*iit);
      }     
      
      for( internalIterator rit = removeTrans.begin();
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransSym(Sym sym)
    {
      Returns removeTrans = Returns();
      for( returnIterator rit = returnTrans.begin();
            rit != returnTrans.end(); rit++ )
      {
        if( rit->third == sym )
          removeTrans.insert(*rit);
      }     
      
      for( returnIterator rit = removeTrans.begin();
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::callExists(St from,Sym sym)
    {
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit->first == from) && (cit->second == sym) )
          return true;
      }  
      return false;    
    }
    
    template < typename St,typename Sym,typename Call,typename Internal,typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls TransSet<St,Sym,Call,Internal,Return>::getCalls(St from,Sym sym)
    {
      Calls result;
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit->first == from) && (cit->second == sym) )
          result.insert(*cit);
      } 
      return result;
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::internalExists(St from,Sym sym)
    {
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == from) && (iit->second == sym) )
          return true;    
      }     
      return false;
    }
    
    template < typename St,typename Sym,typename Call,typename Internal,typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals TransSet<St,Sym,Call,Internal,Return>::getInternals(St from,Sym sym)
    {
      Internals result;
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == from) && (iit->second == sym) )
          result.insert(*iit);
      } 
      return result;
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
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::returnExists(St from, St pred, Sym sym)
    {
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == from) && (rit->second == pred) && (rit->third == sym) )
            return true;      
      }     
      return false;
    }   
    
    template < typename St,typename Sym,typename Call,typename Internal,typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns TransSet<St,Sym,Call,Internal,Return>::getReturns(St from,Sym sym)
    {
      Returns result;
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == from) && (rit->third == sym) )
          result.insert(*rit);
      } 
      return result;
    } 
  }
}
#endif