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
    template <typename St, typename Sym>
    class TransSet : public Printable
    {
      public:    
        typedef std::set<St> States;
        typedef typename States::const_iterator stateIterator;

        typedef Triple<St,Sym,St> Call;
        typedef Triple<St,Sym,St> Internal;
        typedef Quad<St,St,Sym,St> Return;

        typedef std::set< Call > Calls;
        typedef std::set< Internal > Internals;
        typedef std::set< Return > Returns;
        
        typedef typename Calls::const_iterator callIterator;
        typedef typename Internals::const_iterator internalIterator;
        typedef typename Returns::const_iterator returnIterator;


        // The following macro fakes static data declarations with
        // initializers in a template class to work around C++ being
        // dumb. Static data in a template class is almost useless
        // because you have to explicitly define it for every
        // instantiation. Instead, make a static member function that
        // returns the value. (In this case, it's stored as a
        // function-static variable, but this is somewhat irrelevant.
#define DEFINE_FAKE_STATIC_DATA(name, value)    \
        static std::string const & name() {  \
          static std::string ret = value;    \
          return ret;                        \
        }

        DEFINE_FAKE_STATIC_DATA(XMLFromAttr, "from");
        DEFINE_FAKE_STATIC_DATA(XMLPredAttr, "call");
        DEFINE_FAKE_STATIC_DATA(XMLSymbolAttr, "symbol");
        DEFINE_FAKE_STATIC_DATA(XMLToAttr, "to");

        DEFINE_FAKE_STATIC_DATA(InternalXMLTag, "InternalTrans");
        DEFINE_FAKE_STATIC_DATA(CallXMLTag, "CallTrans");
        DEFINE_FAKE_STATIC_DATA(ReturnXMLTag, "ReturnTrans");

#undef DEFINE_FAKE_STATIC_DATA
        
        typedef TransInfo<St,Sym> Info;
      
      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      TransSet( );
      TransSet( const TransSet & other );
      TransSet & operator=( const TransSet & other );

      ~TransSet( );

      //Component Accessors

      /**
       *  
       * @brief access the call site of the given call transition
       *
       * This method provides access to the call site of the given call transition.
       *
       * @param - callTrans: the call transition whose call site to access
       * @return the call site of the given call transition
       *
       */
      inline 
      static St getCallSite( const Call & callTrans ) 
      {
        return callTrans.first;
      }
  
      /**
       *  
       * @brief access the symbol of the given call transition
       *
       * This method provides access to the symbol of the given call transition.
       *
       * @param - callTrans: the call transition whose symbol to access
       * @return the symbol of the given call transition
       *
       */
      inline
      static Sym getCallSym( const Call & callTrans )
      {
        return callTrans.second;
      }

      /**
       *  
       * @brief access the entry point of the given call transition
       *
       * This method provides access to the entry point of the given call transition.
       *
       * @param - callTrans: the call transition whose entry point to access
       * @return the entry point of the given call transition
       *
       */
      inline
      static St getEntry( const Call & callTrans ) 
      {
        return callTrans.third;
      }

      /**
       *  
       * @brief access the source of the given internal transition
       *
       * This method provides access to the source of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose source to access
       * @return the source of the given internal transition
       * 
       */
      inline
      static St getSource( const Internal & internalTrans ) 
      {
        return internalTrans.first;
      }

      /**
       *  
       * @brief access the symbol of the given internal transition
       *
       * This method provides access to the symbol of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose symbol to access
       * @return the symbol of the given internal transition
       *
       */
      inline
      static Sym getInternalSym( const Internal & internalTrans ) 
      {
        return internalTrans.second;
      }

      /**
       *  
       * @brief access the target of the given internal transition
       *
       * This method provides access to the target of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose target to access
       * @return the target of the given internal transition
       *
       */
      inline
      static St getTarget( const Internal & internalTrans ) 
      {
        return internalTrans.third;
      }

      /**
       *  
       * @brief access the exit point of the given return transition
       *
       * This method provides access to the exit point of the given return transition.
       *
       * @param - returnTrans: the return transition whose exit point to access
       * @return the exit point of the given return transition
       *
       */
      inline
      static St getExit( const Return & returnTrans ) 
      {
        return returnTrans.first;
      }

      /**
       *  
       * @brief access the call site of the given return transition
       * 
       * This method provides access to the call site of the given return transition.
       *
       * @param - returnTrans: the return transition whose call site to access
       * @return the call site of the given return transition
       *
       */
      inline
      static St getCallSite( const Return & returnTrans ) 
      {
        return returnTrans.second;
      }

      /**
       *  
       * @brief access the symbol of the given return transition
       *
       * This method provides access to the symbol of the given return transition.
       *
       * @param - returnTrans: the return transition whose symbol to access
       * @return the symbol of the given return transition
       *
       */
      inline
      static Sym getReturnSym( const Return & returnTrans ) 
      {
        return returnTrans.third;
      }

      /**
       *  
       * @brief access the return site of the given return transition
       *
       * This method provides access to the return site of the given return transition.
       *
       * @param - returnTrans: the return transition whose return site to access
       * @return the return site of the given return transition
       *
       */
      inline
      static St getReturnSite( const Return & returnTrans ) 
      {
        return returnTrans.fourth;
      }

#ifdef LABEL
      /**
       *  
       * @brief access the call site of the given call transition
       *
       * This method provides access to the call site of the given call transition.
       *
       * @param - callTrans: the call transition whose call site to access
       * @return the call site of the given call transition
       *
       */
      inline 
      static St getCallSite( const typename Info::Cal & callTrans ) 
      {
        return callTrans.first;
      }
      
      /**
       *  
       * @brief access the symbol of the given call transition
       *
       * This method provides access to the symbol of the given call transition.
       *
       * @param - callTrans: the call transition whose symbol to access
       * @return the symbol of the given call transition
       *
       */
      inline
      static Sym getCallSym( const typename Info::Cal & callTrans )
      {
        return (callTrans.second).getAnySymbol();
      }

      /**
       *  
       * @brief access the symbols of the given call transition
       *
       * This method provides access to the symbols of the given call transition.
       *
       * @param - callTrans: the call transition whose symbols to access
       * @return the symbols of the given call transition
       *
       */
      inline
        static Label getCallLabel( const typename Info::Cal & callTrans )
      {
        return callTrans.second;
      }
      
      /**
       *  
       * @brief access the entry point of the given call transition
       *
       * This method provides access to the entry point of the given call transition.
       *
       * @param - callTrans: the call transition whose entry point to access
       * @return the entry point of the given call transition
       *
       */
      inline
      static St getEntry( const typename Info::Cal & callTrans ) 
      {
        return callTrans.third;
      }

      /**
       *  
       * @brief access the source of the given internal transition
       *
       * This method provides access to the source of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose source to access
       * @return the source of the given internal transition
       * 
       */
      inline
      static St getSource( const typename Info::Int & internalTrans ) 
      {
        return internalTrans.first;
      }

      /**
       *  
       * @brief access the symbol of the given internal transition
       *
       * This method provides access to the symbol of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose symbol to access
       * @return the symbol of the given internal transition
       *
       */
      inline
      static Sym getInternalSym( const typename Info::Int & internalTrans ) 
      {
        return (internalTrans.second).getAnySymbol();
      }

      /**
       *  
       * @brief access the symbols of the given internal transition
       *
       * This method provides access to the symbols of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose symbols to access
       * @return the symbols of the given internal transition
       *
       */
      inline
      static Label getInternalLabel( const typename Info::Int & internalTrans ) 
      {
        return internalTrans.second;
      }
      
      /**
       *  
       * @brief access the target of the given internal transition
       *
       * This method provides access to the target of the given internal transition.
       *
       * @param - internalTrans: the internal transition whose target to access
       * @return the target of the given internal transition
       *
       */
      inline
      static St getTarget( const typename Info::Int & internalTrans ) 
      {
        return internalTrans.third;
      }
      
      /**
       *  
       * @brief access the exit point of the given return transition
       *
       * This method provides access to the exit point of the given return transition.
       *
       * @param - returnTrans: the return transition whose exit point to access
       * @return the exit point of the given return transition
       *
       */
      inline
      static St getExit( const typename Info::Ret & returnTrans ) 
      {
        return returnTrans.first;
      }      

      /**
       *  
       * @brief access the call site of the given return transition
       * 
       * This method provides access to the call site of the given return transition.
       *
       * @param - returnTrans: the return transition whose call site to access
       * @return the call site of the given return transition
       *
       */
      inline
      static St getCallSite( const typename Info::Ret & returnTrans ) 
      {
        return returnTrans.second;
      }
     
      /**
       *  
       * @brief access the symbol of the given return transition
       *
       * This method provides access to the symbol of the given return transition.
       *
       * @param - returnTrans: the return transition whose symbol to access
       * @return the symbol of the given return transition
       *
       */
      inline
      static Sym getReturnSym( const typename Info::Ret & returnTrans ) 
      {
        return (returnTrans.third).getAnySymbol();
      }

      /**
       *  
       * @brief access the symbol of the given return transition
       *
       * This method provides access to the symbol of the given return transition.
       *
       * @param - returnTrans: the return transition whose symbol to access
       * @return the symbol of the given return transition
       *
       */
      inline
      static Label getReturnLabel( const typename Info::Ret & returnTrans ) 
      {
        return returnTrans.third;
      }      

      /**
       *  
       * @brief access the return site of the given return transition
       *
       * This method provides access to the return site of the given return transition.
       *
       * @param - returnTrans: the return transition whose return site to access
       * @return the return site of the given return transition
       *
       */
      inline
      static St getReturnSite( const typename Info::Ret & returnTrans ) 
      {
        return returnTrans.fourth;
      }
#endif

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
      bool getSymbol( St from, St to, Sym & sym );

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
      bool findTrans( St from, Sym sym, St to ) const;

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
      const States getReturnSites( St callSite ) const;

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
      const States & getReturnSites( St exit, St callSite ) const;

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
      const States getCallSites( St exitSite, St returnSite ) const;

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
      const States & getEntries( St callSite ) const;

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
      const States & getTargets( St source ) const;

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
      void dupTransOutgoing( St orig, St dup );

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
      void dupTrans( St orig, St dup );

      /**
       *
       * @brief removes all transitions from this collection of transitions
       *
       * This method removes all transitions from this collection of transitions. 
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
       * @brief add a call transition to the NWA
       *
       * This method creates a call transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the call transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition already exists in the NWA
       *
       */
      bool addCall( St from, Sym sym, St to );

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
       * @brief add an internal transition to the NWA
       *
       * This method creates an internal transition with the given edge and label 
       * information and adds it to the transition set for the NWA.  If the internal 
       * transition already exists in the NWA, false is returned. Otherwise, true is 
       * returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition already exists in the NWA
       *
       */
      bool addInternal( St from, Sym sym, St to );

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
       * @brief add a return transition to the NWA
       *
       * This method creates a return transition with the given edge and label information 
       * and adds it to the transition set for the NWA.  If the return transition already 
       * exists in the NWA, false is returned. Otherwise, true is returned.
       *
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition already exists in the NWA
       *
       */
      bool addReturn( St from, St pred, Sym sym, St to );

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
       * @brief remove a call transition from the NWA
       *
       * This method checks for the call transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the call transition does not exist in the NWA
       *
       */
      bool removeCall( St from, Sym sym, St to );

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
       * @brief remove an internal transition from the NWA
       *
       * This method checks for the internal transition with the given edge and label
       * information in the transition set.  If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       *
       * @param - from: the state the edge departs from
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the internal transition does not exist in the NWA
       *
       */
      bool removeInternal( St from, Sym sym, St to );

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
       * @brief remove a return transition from the NWA
       *
       * This method checks for the return transition with the given edge and label 
       * information in the transition set. If the transition is found, it is removed 
       * from the transition set and true is returned.  Otherwise, false is returned.
       * 
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  	   
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return false if the return transition does not exist in the NWA
       *
       */
      bool removeReturn( St from, St pred, Sym sym, St to );

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
       * @param - from: the state the edge departs from  
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return true if the given call transition is in this collection of 
       *          transitions
       *
       */
      bool isCall( St from, Sym sym, St to ) const;

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
       * @param - from: the state the edge departs from  
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to 
       * @return true if the given internal transition is in this collection of 
       *          transitions associated with the NWA
       *
       */
      bool isInternal( St from, Sym sym, St to ) const;

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
       * @param - from: the state the edge departs from
       * @param - pred: the state from which the call was initiated  	   
       * @param - sym: the symbol labeling the edge
       * @param - to: the state the edge arrives to  
       * @return true if the given return transition is in this collection of 
       *          transitions
       *
       */
      bool isReturn( St from, St pred, Sym sym, St to ) const; 

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
      const Internals getTransFrom( St state ) const;
        
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
      const Internals getTransTo( St state ) const;
        
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
      const Calls getTransCall( St state ) const;
        
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
      const Calls getTransEntry( St state ) const;
        
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
      const Returns getTransExit( St state ) const;
        
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
      const Returns getTransPred( St state ) const;
        
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
      const Returns getTransRet( St state ) const;
        
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
      bool isFrom( St state ) const;
        
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
      bool isTo( St state ) const;
        
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
      bool isCall( St state ) const;
        
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
      bool isEntry( St state ) const;
        
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
      bool isExit( St state ) const;
        
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
      bool isPred( St state ) const;
        
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
      bool isRet( St state ) const;

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
      bool removeTransWith( St state );   

      /** 
       *
       * @brief removes all transitions with the given symbol 
       *
       * This method removes all transitions with the given symbol. 
       * If no transitions exist with the given symbol false is 
       * returned.  Otherwise, true is returned.
       *
       * @param - sym: the symbol whose transitions to remove
       * @return false if no transitions were removed, true otherwise
       *
       */
      bool removeTransSym( Sym sym );
      
      /**
       *   
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
      bool callExists( St from, Sym sym ) const;
        
      /**
       *     
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
      const Calls getCalls( St from, Sym sym ) const;
        
      /**
       *  
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
      bool internalExists( St from, Sym sym ) const;
        
      /**
       *  
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
      const Internals getInternals( St from, Sym sym ) const;

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
      const Internals getInternalsFrom( St from ) const;
        
      /**
       *   
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
      bool returnExists( St from, St pred, Sym sym ) const;
        
      /**
       *    
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
      const Returns getReturns( St from, Sym sym ) const;

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
      bool removeCallTransWith( St state );        
      
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
      bool removeInternalTransWith( St state );
      
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
      bool removeReturnTransWith( St state );  

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
      bool removeCallTransSym( Sym sym );
        
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
      bool removeInternalTransSym( Sym sym );
        
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
      bool removeReturnTransSym( Sym sym );

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
    template <typename St,typename Sym>
    TransSet<St,Sym>::TransSet( )
    {
      callTrans = Calls();
      internalTrans = Internals();
      returnTrans = Returns();  
      
      T_info = Info();
    }
    
    template <typename St,typename Sym>
    TransSet<St,Sym>::TransSet( const TransSet<St,Sym> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
    }
    
    template <typename St,typename Sym>
    TransSet<St,Sym> & TransSet<St,Sym>::operator=( const TransSet<St,Sym> & other )
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
   
    template <typename St,typename Sym>
    TransSet<St,Sym>::~TransSet( ) 
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::getSymbol( St fromSt, St toSt, Sym & sym )
    {
      //Check internal transitions.
      const typename Info::Internals from = T_info.fromTrans(fromSt);
      for(typename Info::internalIterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == getTarget(*it) )
        {
          sym = getInternalSym(*it);
          return true;
        }
      }
      //Check call transitions.
      const typename Info::Calls call = T_info.callTrans(fromSt);
      for(typename Info::callIterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == getEntry(*it) )
        {
          sym = getCallSym(*it);
          return true;
        }
      }
      //Check return transitions.      
      const typename Info::Returns exit = T_info.exitTrans(fromSt);
      for(typename Info::returnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == getReturnSite(*it) )
        {
          sym = getReturnSym(*it);
          return true;
        }
      }
      //Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
      for( Info::returnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt == getReturnSite(*it) )
        {
          sym = getReturnSym(*it);
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::findTrans( St fromSt, Sym sym, St toSt ) const
    {
      //Check internal transitions.
      const typename Info::Internals from = T_info.fromTrans(fromSt);
      for( typename Info::internalIterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == getTarget(*it) && sym == getInternalSym(*it) )
          return true;
      }
      //Check call transitions.
      const typename Info::Calls call = T_info.callTrans(fromSt);
      for( typename Info::callIterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == getEntry(*it) && sym == getCallSym(*it) )
        {
          return true;
        }
      }
      //Check return transitions.      
      const typename Info::Returns exit = T_info.exitTrans(fromSt);
      for( typename Info::returnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == getReturnSite(*it) && sym == getReturnSym(*it) )
        {
          return true;
        }
      }
      //Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.predTrans(fromSt);
      for( Info::returnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt == getReturnSite(*it) && sym == getReturnSym(*it) )
        {
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::States TransSet<St,Sym>::getReturnSites( St callSite ) const
    {
      States returns;
      const typename Info::Returns pred = T_info.predTrans(callSite);
      for( typename Info::returnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        returns.insert(getReturnSite(*it));
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::States & TransSet<St,Sym>::getReturnSites( St exit, St callSite ) const
    {
      States returns;
      const typename Info::Returns pred = T_info.predTrans(callSite);
      for( typename Info::returnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( getExit(*it) == exit )
          returns.insert(getReturnSite(*it));
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::States TransSet<St,Sym>::getCallSites( St exitSite, St returnSite ) const
    {
      States calls;
      const typename Info::Returns exit = T_info.exitTrans(exitSite);
      for( typename Info::returnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( getReturnSite(*it) == returnSite )
          calls.insert(getCallSite(*it));  
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::States & TransSet<St,Sym>::getEntries( St callSite ) const
    {
      States entries;
      const typename Info::Calls cll = T_info.callTrans(callSite);
      for( typename Info::callIterator it = cll.begin(); it != cll.end(); it++ )
      {
        entries.insert(getEntry(*it));
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::States & TransSet<St,Sym>::getTargets( St source ) const
    {
      States targets;
      const typename Info::Internals src = T_info.fromTrans(source);
      for( typename Info::internalIterator it = src.begin(); it != src.end(); it++ )
      {
        targets.insert(getTarget(*it));
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
    template <typename St,typename Sym>
    void TransSet<St,Sym>::dupTransOutgoing( St orig, St dup )
    { 
      //Duplicate outgoing internal transitions.
      const typename Info::Internals from = T_info.fromTrans(orig);
      for( typename Info::internalIterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans = Internal(dup,getInternalSym(*it),getTarget(*it));
        addInternal(iTrans);

      }
      //Duplicate call site call transitions.
      const typename Info::Calls call = T_info.callTrans(orig);
      for( typename Info::callIterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans = Call(dup,getCallSym(*it),getEntry(*it));
        addCall(cTrans);

      }
      //Duplicate exit point return transitions.
      const typename Info::Returns exit = T_info.exitTrans(orig);
      for( typename Info::returnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans = Return(dup,getCallSite(*it),getReturnSym(*it),getReturnSite(*it));
        addReturn(rTrans);
      }
      //Duplicate call predecessor return transitions.
      const typename Info::Returns pred = T_info.predTrans(orig);
      for( typename Info::returnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans = Return(getExit(*it),dup,getReturnSym(*it),getReturnSite(*it));
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
    template <typename St,typename Sym>
    void TransSet<St,Sym>::dupTrans( St orig, St dup )
    { 
      //Duplicate outgoing internal transitions.
      const typename Info::Internals from = T_info.fromTrans(orig);
      for( typename Info::internalIterator it = from.begin(); it != from.end(); it++ )
      {
        Internal* iTrans = new Internal(dup,getInternalSym(*it),getTarget(*it));
        addInternal(*iTrans);
        if( orig == getTarget(*it) )   //Handle self-loops.
        {
          iTrans = new Internal(dup,getInternalSym(*it),dup);
          addInternal(*iTrans);
        }
      }
      //Duplicate incoming internal transitions.
      const typename Info::Internals to = T_info.toTrans(orig);
      for( typename Info::internalIterator it = to.begin(); it != to.end(); it++ )
      {
        Internal* iTrans = new Internal(getSource(*it),getInternalSym(*it),dup);
        addInternal(*iTrans);
      }
      //Duplicate call site call transitions.
      const typename Info::Calls call = T_info.callTrans(orig);
      for( typename Info::callIterator it = call.begin(); it != call.end(); it++ )
      {
        Call* cTrans = new Call(dup,getCallSym(*it),getEntry(*it));
        addCall(*cTrans);
        if( orig == getEntry(*it) )   //Handle self-loops.
        {
          cTrans = new Call(dup,getCallSym(*it),dup);
          addCall(*cTrans);
        }
      }
      //Duplicate entry point call transitions.
      const typename Info::Calls entry = T_info.entryTrans(orig);
      for( typename Info::callIterator it = entry.begin(); it != entry.end(); it++ )
      {
        Call* cTrans = new Call(getCallSite(*it),getCallSym(*it),dup);
        addCall(*cTrans);
      }
      //Duplicate exit point return transitions.
      const typename Info::Returns exit = T_info.exitTrans(orig);
      for( typename Info::returnIterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return* rTrans = new Return(dup,getCallSite(*it),getReturnSym(*it),getReturnSite(*it));
        addReturn(*rTrans);
        if( orig == getCallSite(*it) )   //Handle self-loops.
        {
          rTrans = new Return(dup,dup,getReturnSym(*it),getReturnSite(*it));
          addReturn(*rTrans);
        }
        if( orig == getReturnSite(*it) )   //Handle self-loops.
        {
          rTrans = new Return(dup,getCallSite(*it),getReturnSym(*it),dup);
          addReturn(*rTrans);
        }
        if( orig == getCallSite(*it) && orig == getReturnSite(*it) )   //Handle self-loops.
        {
          rTrans = new Return(dup,dup,getReturnSym(*it),dup);
          addReturn(*rTrans);
        }
      }
      //Duplicate call predecessor return transitions.
      const typename Info::Returns pred = T_info.predTrans(orig);
      for( typename Info::returnIterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return* rTrans = new Return(getExit(*it),dup,getReturnSym(*it),getReturnSite(*it));
        addReturn(*rTrans);
        if( orig == getReturnSite(*it) )   //Handle self-loops.
        {
          rTrans = new Return(getExit(*it),dup,getReturnSym(*it),dup);
          addReturn(*rTrans);
        }
      }
      //Duplicate return site return transitions.
      const typename Info::Returns ret = T_info.retTrans(orig);
      for( typename Info::returnIterator it = ret.begin(); it != ret.end(); it++ )
      {
        Return* rTrans = new Return(getExit(*it),getCallSite(*it),getReturnSym(*it),dup);
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
    template <typename St,typename Sym>
    void TransSet<St,Sym>::clear( )
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::Calls & TransSet<St,Sym>::getCalls() const
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::Internals & TransSet<St,Sym>::getInternals() const
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
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::Returns & TransSet<St,Sym>::getReturns() const
    {
      return returnTrans;
    }

    /**
     *
     * @brief add a call transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the call transition already exists in the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::addCall( St from, Sym sym, St to )
    {
      Call ct = Call(from,sym,to);
      return addCall(ct);
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @param - addTrans: the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */ 
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::addCall( const Call & addTrans )
    {     
      if( callTrans.count(addTrans) > 0 )
        return false;      
      
      callTrans.insert(addTrans);
      T_info.addCall(addTrans);

      return true;
    }

    /**
     *
     * @brief add an internal transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the internal transition already exists in the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::addInternal( St from, Sym sym, St to )
    {
      Internal it = Internal(from,sym,to);
      return addInternal(it);
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @param - addTrans: the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::addInternal( const Internal & addTrans )
    {
      if( internalTrans.count(addTrans) > 0 )
        return false;
      
      internalTrans.insert(addTrans);
      T_info.addIntra(addTrans);

      return true;
    }
    
    /**
     *
     * @brief add a return transition to the NWA
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the return transition already exists in the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::addReturn( St from, St pred, Sym sym, St to )
    {
      Return rt = Return(from,pred,sym,to);
      return addReturn(rt);
    }

    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @param - addTrans: the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::addReturn( const Return & addTrans )
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
    template <typename St,typename Sym>
    void TransSet<St,Sym>::addAllTrans( TransSet<St,Sym> addTransSet )
    {   
      //Add call transitions.
      for(typename TransSet<St,Sym>::callIterator it = addTransSet.beginCall(); 
            it != addTransSet.endCall(); it ++ )
      {
        addCall(*it);
      }
      //Add internal transitions.
      for(typename TransSet<St,Sym>::internalIterator it = addTransSet.beginInternal(); 
            it != addTransSet.endInternal(); it ++ )
      {
        addInternal(*it);
      }
      //Add return transitions.
      for(typename TransSet<St,Sym>::returnIterator it = addTransSet.beginReturn(); 
            it != addTransSet.endReturn(); it ++ )
      {
        addReturn(*it);
      }
    }

    /**
     *
     * @brief remove a call transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the call transition does not exist in the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeCall( St from, Sym sym, St to )
    {
      Call ct = Call(from,sym,to);
      return removeCall(ct);
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeCall( const Call & removeTrans )
    {
      if( callTrans.count(removeTrans) == 0 )
        return false;
        
      callTrans.erase(removeTrans);
      T_info.removeCall(removeTrans);

      return true;
    }

    /**
     *
     * @brief remove an internal transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return false if the internal transition does not exist in the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeInternal( St from, Sym sym, St to )
    {
      Internal it = Internal(from,sym,to);
      return removeInternal(it);
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeInternal( const Internal & removeTrans )
    {
      if( internalTrans.count(removeTrans) == 0 )
        return false;
        
      internalTrans.erase(removeTrans);
      T_info.removeIntra(removeTrans);

      return true;
    }

    /**
     *
     * @brief remove a return transition from the NWA
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  	   
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to 
     * @return false if the return transition does not exist in the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeReturn( St from, St pred, Sym sym, St to )
    {
      Return rt = Return(from,pred,sym,to);
      return removeReturn(rt);
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeReturn( const Return & removeTrans )
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
     * @param - from: the state the edge departs from  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return true if the given call transition is in this collection of 
     *          transitions
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::isCall( St from, Sym sym, St to ) const
    {
      Call ct = Call(from,sym,to);
      return isCall(ct);
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isCall( const Call & trans ) const
    {
      return (callTrans.count(trans) > 0);
    }

    /**
     *
     * @brief test whether the given internal transition is in this collection of 
     *        transitions 
     *
     * @param - from: the state the edge departs from  
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to 
     * @return true if the given internal transition is in this collection of 
     *          transitions associated with the NWA
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::isInternal( St from, Sym sym, St to ) const
    {
      Internal it = Internal(from,sym,to);
      return isInternal(it);
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isInternal( const Internal & trans ) const
    {
      return (internalTrans.count(trans) > 0);
    }

    /**
     *
     * @brief test whether the given return transition is in this collection of 
     *        transitions 
     *
     * @param - from: the state the edge departs from
     * @param - pred: the state from which the call was initiated  	   
     * @param - sym: the symbol labeling the edge
     * @param - to: the state the edge arrives to  
     * @return true if the given return transition is in this collection of 
     *          transitions
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::isReturn( St from, St pred, Sym sym, St to ) const
    {
      Return rt = Return(from,pred,sym,to);
      return isReturn(rt);
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isReturn( const Return & trans ) const
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
    template <typename St,typename Sym>
    std::ostream & TransSet<St,Sym>::print( std::ostream & o ) const
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
        printKey(o,getCallSite(*cit));
        o << ", ";
        printKey(o,getCallSym(*cit));
        o << ", "; 
        printKey(o,getEntry(*cit));
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
        if( !first )
          o << ", \n";
        o << "(";
        printKey(o,getSource(*iit));
        o << ", ";
        printKey(o,getInternalSym(*iit));
        o << ", ";
        printKey(o,getTarget(*iit));
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
        printKey(o,getExit(*rit));
        o << ", ";
        printKey(o,getCallSite(*rit));
        o << ", "; 
        printKey(o,getReturnSym(*rit));
        o << ", ";
        printKey(o,getReturnSite(*rit));
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
    template <typename St,typename Sym>
    std::ostream & TransSet<St,Sym>::print_dot( std::ostream & o) const
    {    
      //Print call transitions.
      o << "//Delta_c: \n";
      callIterator cit = callTrans.begin();
      callIterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        cit->first.print(o << "\"") << "\"";
        o << "->";
        cit->third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        cit->second.print(o);
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
        iit->first.print(o << "\"") << "\"";
        o << "->";
        iit->third.print(o << "\"") << "\"";
        o << "[";
        o << " label=\"";
        iit->second.print(o);
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
        o << "\"" << std::dec << rit->second << std::dec << ", " << rit->first << "\"";
        o <<"[ shape=box ];\n";

        // exit to dummy
        rit->first.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << rit->second <<", " << rit->first << "\"";
        o << "[";
        o << " label=\"";
        rit->third.print(o);
        o << "\"";
        o << " color=red";
        o << "];\n";

        // call to dummy
        rit->second.print(o << "\"") << "\"";
        o << "->";
        o << "\"" << rit->second <<", " << rit->first << "\"";
        o << "[";
        o << " color=blue";
        o << "];\n";

        //dummy to ret
        o << "\"" << rit->second <<", " << rit->first << "\"";
        o << "->";
        rit->fourth.print(o << "\"") << "\"";
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::operator==( const TransSet<St,Sym> & other ) const
    { // Q: Do I want a deeper check here?
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
    template <typename St,typename Sym>
    inline
    typename TransSet<St,Sym>::callIterator TransSet<St,Sym>::beginCall() const
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
    template <typename St,typename Sym>
    inline
    typename TransSet<St,Sym>::internalIterator TransSet<St,Sym>::beginInternal() const
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
    template <typename St,typename Sym>
    inline
    typename TransSet<St,Sym>::returnIterator TransSet<St,Sym>::beginReturn() const
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
    template <typename St,typename Sym>
    inline
    typename TransSet<St,Sym>::callIterator TransSet<St,Sym>::endCall() const
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
    template <typename St,typename Sym>
    inline
    typename TransSet<St,Sym>::internalIterator TransSet<St,Sym>::endInternal() const
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
    template <typename St,typename Sym>
    inline
    typename TransSet<St,Sym>::returnIterator TransSet<St,Sym>::endReturn() const
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
    template <typename St,typename Sym>
    inline
    size_t TransSet<St,Sym>::sizeCall( ) const
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
    template <typename St,typename Sym>
    inline
    size_t TransSet<St,Sym>::sizeInternal( ) const
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
    template <typename St,typename Sym>
    inline
    size_t TransSet<St,Sym>::sizeReturn( )  const
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
    template <typename St,typename Sym>
    size_t TransSet<St,Sym>::size( ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Internals TransSet<St,Sym>::getTransFrom( St state ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Internals TransSet<St,Sym>::getTransTo( St state ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Calls TransSet<St,Sym>::getTransCall( St state ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Calls TransSet<St,Sym>::getTransEntry( St state ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Returns TransSet<St,Sym>::getTransExit( St state ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Returns TransSet<St,Sym>::getTransPred( St state ) const
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Returns TransSet<St,Sym>::getTransRet( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isFrom( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isTo( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isCall( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isEntry( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isExit( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isPred( St state ) const
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
    template <typename St,typename Sym>
    inline
    bool TransSet<St,Sym>::isRet( St state ) const
    {
      return T_info.isRet( state );
    }

    /** 
     *   
     * @brief removes all transitions involving the given state  
     *
     * @param - state: the state whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeTransWith( St state )
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeCallTransWith( St state )
    {  
      Calls removeTrans = Calls();
      //Find all transitions to remove.
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (getCallSite(*cit) == state) ||
            (getEntry(*cit) == state) )
            removeTrans.insert(*cit);
      }     
      //Remove the transitions.
      for( callIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeInternalTransWith( St state )
    {
      Internals removeTrans = Internals();
      //Find transitions to remove.
      for( internalIterator iit = beginInternal(); iit != endInternal(); iit++ )
      {
        if( (getSource(*iit) == state) ||
            (getSource(*iit) == state) )
            removeTrans.insert(*iit);
      }     
      //Remove transitions.
      for( internalIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeReturnTransWith( St state )
    {
      Returns removeTrans = Returns();
      //Find transitions to remove.
      for( returnIterator rit = beginReturn(); rit != endReturn(); rit++ )
      {
        if( (getExit(*rit) == state) ||
            (getCallSite(*rit) == state) ||
            (getReturnSite(*rit) == state) )
            removeTrans.insert(*rit);
      }     
      //Remove transitions.
      for( returnIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0;
    }

   /** 
     *
     * @brief removes all transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeTransSym( Sym sym )
    {
      bool found = false;
      found = found || removeCallTransSym(sym);
      found = found || removeInternalTransSym(sym);
      found = found || removeReturnTransSym(sym);
      return found;
    }
    
    /** 
     *
     * @brief removes all call transitions with the given symbol 
     *
     * @param - sym: the symbol whose transitions to remove
     * @return false if no transitions were removed, true otherwise
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeCallTransSym( Sym sym )
    {
      Calls removeTrans = Calls();
      //Find transitions to remove.
      for( callIterator cit = callTrans.begin(); cit != callTrans.end(); cit++ )
      {
        if( getCallSym(*cit) == sym )
          removeTrans.insert(*cit);
      }     
      //Remove transitions.
      for( callIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeInternalTransSym( Sym sym )
    {
      Internals removeTrans = Internals();
      //Find transitions to remove.
      for( internalIterator iit = internalTrans.begin(); iit != internalTrans.end(); iit++ )
      {
        if( getInternalSym(*iit) == sym )
          removeTrans.insert(*iit);
      }     
      //Remove transitions.
      for( internalIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::removeReturnTransSym( Sym sym )
    {
      Returns removeTrans = Returns();
      //Find transitions to remove.
      for( returnIterator rit = returnTrans.begin(); rit != returnTrans.end(); rit++ )
      {
        if( getReturnSym(*rit) == sym )
          removeTrans.insert(*rit);
      }     
      //Remove transitions.
      for( returnIterator rit = removeTrans.begin(); rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     * Q: how can this be optimized?
     * @brief test if there exists a call transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the call transition
     * @param - sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     *          symbol in this collection of transitions 
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::callExists( St from, Sym sym ) const
    {
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (getCallSite(*cit) == from) && (getCallSym(*cit) == sym) )
          return true;
      }  
      return false;    
    }
    
    /**
     * Q: how can this be optimized?
     * @brief provides access to all call transitions with the given from state
     *        and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the call transitions
     * @param - sym: the desired symbol for the call transitions
     * @return the set of call transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::Calls TransSet<St,Sym>::getCalls( St from, Sym sym ) const 
    {
      Calls result;
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (getCallSite(*cit) == from) && (getCallSym(*cit) == sym) )
          result.insert(*cit);
      } 
      return result;
    }
    
    /**
     * Q: how can this be optimized?
     * @brief test if there exists an internal transition with the given from state 
     *        and symbol in this collection of transitions 
     *
     * @param - from: the desired from state for the internal transition
     * @param - sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     *          symbol in this collection of transitions
     *
     */
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::internalExists( St from, Sym sym ) const
    {
      for( internalIterator iit = beginInternal(); iit != endInternal(); iit++ )
      {
        if( (getSource(*iit) == from) && (getInternalSym(*iit) == sym) )
          return true;    
      }     
      return false;
    }
    
    /**
     * Q: how can this be optimized?
     * @brief provides access to all internal transitions with the given from 
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the internal transitions
     * @param - sym: the desired symbol for the internal transitions
     * @return the set of internal transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::Internals TransSet<St,Sym>::getInternals( St from, Sym sym ) const
    {
      Internals result;
      for( internalIterator iit = beginInternal(); iit != endInternal(); iit++ )
      {
        if( (getSource(*iit) == from) && (getInternalSym(*iit) == sym) )
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
    template <typename St,typename Sym>
    inline
    const typename TransSet<St,Sym>::Internals TransSet<St,Sym>::getInternalsFrom( St from ) const
    {
      return fromTrans(from);
    }


    /**
     * Q: how can this be optimized?
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
    template <typename St,typename Sym>
    bool TransSet<St,Sym>::returnExists( St from, St pred, Sym sym ) const
    {
      for( returnIterator rit = beginReturn(); rit != endReturn(); rit++ )
      {
        if( (getExit(*rit) == from) && (getCallSite(*rit) == pred) && (getReturnSym(*rit) == sym) )
            return true;      
      }     
      return false;
    }   
    
    /**
     * Q: how can this be optimized?
     * @brief provides access to all return transitions with the given from
     *        state and symbol in this collection of transitions
     *
     * @param - from: the desired from state for the return transitions
     * @param - sym: the desired symbol for the return transitions
     * @return the set of return transitions with the given from state and symbol
     *
     */
    template <typename St,typename Sym>
    const typename TransSet<St,Sym>::Returns TransSet<St,Sym>::getReturns( St from, Sym sym ) const
    {
      Returns result;
      for( returnIterator rit = beginReturn(); rit != endReturn(); rit++ )
      {
        if( (getExit(*rit) == from) && (getReturnSym(*rit) == sym) )
          result.insert(*rit);
      } 
      return result;
    } 
  }
}
#endif
