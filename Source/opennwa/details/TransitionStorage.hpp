#ifndef wali_nwa_TransitionStorage_GUARD
#define wali_nwa_TransitionStorage_GUARD 1

/**
 * @author Amanda Burton
 */

#include "opennwa/NwaFwd.hpp"

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "opennwa/details/StateStorage.hpp"
#include "opennwa/details/TransitionInfo.hpp"

// std::c++
#include <iostream>
#include <set>
#include <assert.h>

namespace opennwa
{
  namespace details
  {
    
    /**
     *
     * This class is used to keep track of the transitions of an NWA.
     *
     */
    class TransitionStorage : public wali::Printable
    {
    public:    
      typedef std::set<State> States;

      DEPRECATE("Use (captial-S) StateIterator, not that you should be using this one anyway")
      typedef States::const_iterator stateIterator;
      typedef States::const_iterator StateIterator;

      typedef TransitionInfo::Call Call;
      typedef TransitionInfo::Internal Internal;
      typedef TransitionInfo::Return Return;

      typedef TransitionInfo::Calls Calls;
      typedef TransitionInfo::Internals Internals;
      typedef TransitionInfo::Returns Returns;

      DEPRECATE("Use capitalized version (CallIterator) instead. (Not that you should be using it from TransitionStorage in the first place.)")
      typedef Calls::const_iterator callIterator;
      DEPRECATE("Use capitalized version (InternalIterator) instead. (Not that you should be using it from TransitionStorage in the first place.)")
      typedef Internals::const_iterator internalIterator;
      DEPRECATE("Use capitalized version (ReturnIterator) instead. (Not that you should be using it from TransitionStorage in the first place.)")
      typedef Returns::const_iterator returnIterator;

      typedef Calls::const_iterator CallIterator;
      typedef Internals::const_iterator InternalIterator;
      typedef Returns::const_iterator ReturnIterator;

      // The following macro fakes static data declarations with
      // initializers in a template class to work around C++ being
      // dumb. Static data in a template class is almost useless
      // because you have to explicitly define it for every
      // instantiation. Instead, make a static member function that
      // returns the value. (In this case, it's stored as a
      // function-static variable, but this is somewhat irrelevant.
#define DEFINE_FAKE_STATIC_DATA(name, value)    \
      static std::string const & name() {       \
        static std::string ret = value;         \
        return ret;                             \
      }

      DEFINE_FAKE_STATIC_DATA(XMLFromAttr, "from");
      DEFINE_FAKE_STATIC_DATA(XMLPredAttr, "call");
      DEFINE_FAKE_STATIC_DATA(XMLSymbolAttr, "symbol");
      DEFINE_FAKE_STATIC_DATA(XMLToAttr, "to");

      DEFINE_FAKE_STATIC_DATA(InternalXMLTag, "InternalTrans");
      DEFINE_FAKE_STATIC_DATA(CallXMLTag, "CallTrans");
      DEFINE_FAKE_STATIC_DATA(ReturnXMLTag, "ReturnTrans");

#undef DEFINE_FAKE_STATIC_DATA
        
      typedef TransitionInfo Info;
      
      //
      // Methods
      //

    public:
      
      //Constructors and Destructor
      TransitionStorage & operator=( const TransitionStorage & other );

      
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
      static State getCallSite( const Call & callTrans ) 
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
      static Symbol getCallSym( const Call & callTrans )
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
      static State getEntry( const Call & callTrans ) 
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
      static State getSource( const Internal & internalTrans ) 
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
      static Symbol getInternalSym( const Internal & internalTrans ) 
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
      static State getTarget( const Internal & internalTrans ) 
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
      static State getExit( const Return & returnTrans ) 
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
      static State getCallSite( const Return & returnTrans ) 
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
      static Symbol getReturnSym( const Return & returnTrans ) 
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
      static State getReturnSite( const Return & returnTrans ) 
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
      static St getCallSite( const Info::Cal & callTrans ) 
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
      static Sym getCallSym( const Info::Cal & callTrans )
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
      static Label getCallLabel( const Info::Cal & callTrans )
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
      static St getEntry( const Info::Cal & callTrans ) 
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
      static St getSource( const Info::Int & internalTrans ) 
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
      static Sym getInternalSym( const Info::Int & internalTrans ) 
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
      static Label getInternalLabel( const Info::Int & internalTrans ) 
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
      static St getTarget( const Info::Int & internalTrans ) 
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
      static St getExit( const Info::Ret & returnTrans ) 
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
      static St getCallSite( const Info::Ret & returnTrans ) 
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
      static Sym getReturnSym( const Info::Ret & returnTrans ) 
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
      static Label getReturnLabel( const Info::Ret & returnTrans ) 
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
      static St getReturnSite( const Info::Ret & returnTrans ) 
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
      bool getSymbol( State from, State to, Symbol & sym ) const;

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
      bool findTrans( State from, Symbol sym, State to ) const;

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
      const States getReturnSites( State callSite ) const;

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
      States getReturnSites( State exit, State callSite ) const;

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
      const States getCallSites( State exitSite, State returnSite ) const;

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
      const States getEntries( State callSite ) const;

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
      const States getTargets( State source ) const;

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
      void dupTransOutgoing( State orig, State dup );

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
      void dupTrans( State orig, State dup );

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
      bool addCall( State from, Symbol sym, State to );

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
      bool addInternal( State from, Symbol sym, State to );

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
      bool addReturn( State from, State pred, Symbol sym, State to );

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
       * @param - addTransitionStorage: the collection of transitions to add to this 
       *          collection of transitions
       *
       */
      void addAllTrans( TransitionStorage addTransitionStorage );
      
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
      bool removeCall( State from, Symbol sym, State to );

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
      bool removeInternal( State from, Symbol sym, State to );

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
      bool removeReturn( State from, State pred, Symbol sym, State to );

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
      bool isCall( State from, Symbol sym, State to ) const;

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
      bool isInternal( State from, Symbol sym, State to ) const;

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
      bool isReturn( State from, State pred, Symbol sym, State to ) const; 

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
       * @param - other: the TransitionStorage to compare this TransitionStorage to
       * @return true if this TransitionStorage is equivalent to the TransitionStorage 'other'
       *
       */
      bool operator==( const TransitionStorage & other ) const;

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
      CallIterator beginCall( ) const;
        
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
      InternalIterator beginInternal( ) const;
       
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
      ReturnIterator beginReturn( ) const;
      
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
      CallIterator endCall( ) const;
         
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
      InternalIterator endInternal( ) const;
         
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
      ReturnIterator endReturn( ) const;

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
      const Internals & getTransFrom( State state ) const;
        
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
      const Internals & getTransTo( State state ) const;
        
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
      const Calls & getTransCall( State state ) const;
        
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
      const Calls & getTransEntry( State state ) const;
        
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
      const Returns & getTransExit( State state ) const;
        
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
      const Returns & getTransPred( State state ) const;
        
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
      const Returns & getTransRet( State state ) const;
        
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
      bool isFrom( State state ) const;
        
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
      bool isTo( State state ) const;
        
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
      bool isCall( State state ) const;
        
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
      bool isEntry( State state ) const;
        
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
      bool isExit( State state ) const;
        
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
      bool isPred( State state ) const;
        
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
      bool isRet( State state ) const;

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
      bool removeTransWith( State state );   

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
      bool removeTransSym( Symbol sym );
      
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
      bool callExists( State from, Symbol sym ) const;
        
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
      const Calls getCalls( State from, Symbol sym ) const;
        
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
      bool internalExists( State from, Symbol sym ) const;
        
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
      const Internals getInternals( State from, Symbol sym ) const;

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
      const Internals getInternalsFrom( State from ) const;
        
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
      bool returnExists( State from, State pred, Symbol sym ) const;
        
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
      const Returns getReturns( State from, Symbol sym ) const;

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
      bool removeCallTransWith( State state );        
      
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
      bool removeInternalTransWith( State state );
      
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
      bool removeReturnTransWith( State state );  

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
      bool removeCallTransSym( Symbol sym );
        
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
      bool removeInternalTransSym( Symbol sym );
        
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
      bool removeReturnTransSym( Symbol sym );

      //
      // Variables
      //
      
    protected: 
       
      Calls callTrans;
      Internals internalTrans;
      Returns returnTrans;
        
      Info T_info;
    };


  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


#endif
