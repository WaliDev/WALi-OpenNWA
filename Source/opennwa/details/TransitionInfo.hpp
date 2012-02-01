#ifndef wali_nwa_TransitionInfo_GUARD
#define wali_nwa_TransitionInfo_GUARD 1

/**
 * @author Amanda Burton
 */

//#define LABEL

#include "opennwa/NwaFwd.hpp"
 
// ::wali
#include "wali/KeyContainer.hpp"
#include "opennwa/details/SymbolStorage.hpp"
#include "wali/Key.hpp" 
 
// std::c++
#include <map> 
#include <set>
 
namespace opennwa
{ 
  namespace details
  {
      
    /**
     *
     * This class is used to keep track of the maps from state to transition for an NWA.
     *
     */
    class TransitionInfo
    {    
    public:

      typedef wali::Triple<State,Symbol,State> Internal;
      typedef wali::Triple<State,Symbol,State> Call;
      typedef wali::Quad<State,State,Symbol,State> Return;

      typedef wali::Triple<State,Label,State> Int;
      typedef wali::Triple<State,Label,State> Cal;
      typedef wali::Quad<State,State,Label,State> Ret;

      typedef std::set<Internal> Internals;
      DEPRECATE("Use capitalized version (InternalIterator), not that you shoud be using the one from this class in the first place.")
      typedef Internals::const_iterator internalIterator;
      typedef Internals::const_iterator InternalIterator;
        
      typedef std::set<Call> Calls;
      DEPRECATE("Use capitalized version (CallIterator), not that you shoud be using the one from this class in the first place.")
      typedef Calls::const_iterator callIterator;
      typedef Calls::const_iterator CallIterator;
        
      typedef std::set<Return> Returns;
      DEPRECATE("Use capitalized version (ReturnIterator), not that you shoud be using the one from this class in the first place.")
      typedef Returns::const_iterator returnIterator;
      typedef Returns::const_iterator ReturnIterator;
#if !defined(LABEL)
      typedef std::map<State,Internals> IntraMap;
      typedef std::map<State,Calls> CallMap;
      typedef std::map<State,Returns> RetMap;
#else
      typedef std::map<St,Int> IntraMap;
      typedef std::map<St,Cal> CallMap;
      typedef std::map<St,Ret> RetMap;
#endif

      static Internals const & emptyInternals() {
        static Internals r;
        return r;
      }
      static Calls const & emptyCalls() {
        static Calls c;
        return c;
      }
      static Returns const & emptyReturns() {
        static Returns r;
        return r;
      }

      //
      // Methods
      //
      
    public:
      
      //Constructors and Destructor
      TransitionInfo & operator=( const TransitionInfo & other )
      {
        if( this == &other )     
          return *this;
        
        clearMaps();
        
        from_ITrans = other.from_ITrans;
        to_ITrans = other.to_ITrans;
        
        call_CTrans = other.call_CTrans;
        entry_CTrans = other.entry_CTrans;
        
        exit_RTrans = other.exit_RTrans;
        pred_RTrans = other.pred_RTrans;
        ret_RTrans = other.ret_RTrans;
        
        return *this;
      }
  
     
#if !defined(LABEL)
      /**
       * 
       * @brief add an internal transition to the maps
       *
       * This method updates the maps associated with the source and target of the given
       * internal transition.  
       *
       * @param - intra: the internal transition to add to the maps
       *  
       */
      void addIntra( const Internal & intra )
      {
        from_ITrans[intra.first].insert(intra);
        to_ITrans[intra.third].insert(intra);
      }
      
      /**
       * 
       * @brief remove an internal transition from the maps
       *
       * This method updates the maps associated with the source and target of the given
       * internal transition.  
       *
       * @param - intra: the internal transition to remove from the maps
       *  
       */
      void removeIntra( const Internal & intra )
      {
        //Update the map for the source of the transition.
        IntraMap::iterator it = from_ITrans.find(intra.first);
        if( it != from_ITrans.end() )
        {
          it->second.erase(intra);
          if( it->second.empty() )
            from_ITrans.erase(it);
        }
        
        //Update the map for the target of the transition.
        it = to_ITrans.find(intra.third);
        if( it != to_ITrans.end() )
        {
          it->second.erase(intra);
          if( it->second.empty() )
            to_ITrans.erase(it);
        }
      }

      /**
       * 
       * @brief add a call transition to the maps
       *
       * This method updates the maps associated with the call point and entry point of
       * the given call transition.  
       *
       * @param - call: the call transition to add to the maps
       *  
       */
      void addCall( const Call & call )
      {
        call_CTrans[call.first].insert(call);
        entry_CTrans[call.third].insert(call);
      }
      
      /**
       * 
       * @brief remove a call transition from the maps
       *
       * This method updates the maps associated with the call point and entry point of
       * the given call transition.  
       *
       * @param - call: the call transition to remove from the maps
       *  
       */
      void removeCall( const Call & call )
      {
        //Update the maps for the call point of the transition.
        CallMap::iterator it = call_CTrans.find(call.first);
        if( it != call_CTrans.end() )
        {
          it->second.erase(call);
          if( it->second.empty() )
            call_CTrans.erase(it);
        }
        
        //Update the maps for the entry point of the transition.
        it = entry_CTrans.find(call.third);
        if( it != entry_CTrans.end() )
        {
          it->second.erase(call);
          if( it->second.empty() )
            entry_CTrans.erase(it);
        }
      }
      
      /**
       * 
       * @brief add a return transition to the maps
       *
       * This method updates the maps associated with the exit point, call predecessor,
       * and return point of the given return transition.  
       *
       * @param - ret: the return transition to add to the maps
       *  
       */
      void addRet( const Return & ret )
      {
        exit_RTrans[ret.first].insert(ret);
        pred_RTrans[ret.second].insert(ret);
        ret_RTrans[ret.fourth].insert(ret);
      }
      
      /**
       * 
       * @brief remove a return transition from the maps
       *
       * This method updates the maps associated with the exit point, call predecessor,
       * and return point of the given return transition.  
       *
       * @param - ret: the return transition to remove from the maps
       *  
       */
      void removeRet( const Return & ret )
      { 
        //Update the maps for the exit point of the transition.
        RetMap::iterator it = exit_RTrans.find(ret.first);
        if( it != exit_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            exit_RTrans.erase(it);
        }
        
        //Update the maps for the call predecessor of the transition.
        it = pred_RTrans.find(ret.second);
        if( it != pred_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            pred_RTrans.erase(it);
        }
        
        //Update the maps for the return point of the transition.
        it = ret_RTrans.find(ret.fourth);
        if( it != ret_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            ret_RTrans.erase(it);
        }
      }
      
      /**
       *  
       * @brief returns all internal transitions with the given state as the source
       *
       * This method returns the set of all internal transitions whose source is the
       * given state.
       *
       * @param - name: the state whose outgoing transitions to obtain
       * @return a set of outgoing transitions for the given state
       *
       */
      const Internals & fromTrans( State state ) const
      {
        IntraMap::const_iterator it = from_ITrans.find(state);
        if( it == from_ITrans.end() )
          return emptyInternals();
        else
          return it->second;
      }
      
      /**
       *  
       * @brief returns all internal transitions with the given state as the target
       *
       * This method returns the set of all internal transitions whose target is the 
       * given state.
       *
       * @param - name: the state whose incoming transitions to obtain
       * @returm a set of incoming transitions for the given state
       *
       */
      const Internals & toTrans( State state ) const
      {
        IntraMap::const_iterator it = to_ITrans.find(state);
        if( it == to_ITrans.end() )
          return emptyInternals();
        else
          return it->second;
      } 
      
      /**
       *  
       * @brief returns all call transitions with the given state as the call site
       *
       * This method returns the set of all call transitions whose call site is the
       * given state.
       *
       * @param - name: the state whose call site transitions to obtain
       * @return a set of call transitions with the given state playing the part of 
       *         the call site
       *
       */
      const Calls & callTrans( State state ) const
      {
        CallMap::const_iterator it = call_CTrans.find(state);
        if( it == call_CTrans.end() )
          return emptyCalls();
        else
          return it->second;
      }
      
      /**
       *  
       * @brief returns all call transitions with the given state as the entry point
       *
       * This method returns the set of all call transitions whose entry point is the
       * given state.
       *
       * @param - name: the state whose entry transitions to obtain
       * @return a set of call transitions with the given state playing the part of
       *         the entry point
       *
       */
      const Calls & entryTrans( State state ) const
      { 
        CallMap::const_iterator it = entry_CTrans.find(state);
        if( it == entry_CTrans.end() )
          return emptyCalls();
        else
          return it->second;
      }
      
      /**
       *  
       * @brief returns all return transitions with the given state as the exit point
       *
       * This method returns the set of all return transitions whose exit point is
       * the given state.
       *
       * @param - name: the state whose exit transitions to obtain
       * @return a set of return transitions with the given state playing the part of
       *         the exit point
       *
       */
      const Returns & exitTrans( State state ) const
      {
        RetMap::const_iterator it = exit_RTrans.find(state);
        if( it == exit_RTrans.end() )
          return emptyReturns();
        else
          return it->second;
      }
      
      /**
       *  
       * @brief returns all return transitions with the given state as the call predecessor
       *
       * This method returns the set of all return transitions whose call predecessor
       * is the given state.
       *
       * @param - name: the state whose call predecessor transitions to obtain
       * @return a set of return transitions with the given state playing the part of 
       *         the call predecessor.
       *
       */
      const Returns & predTrans( State state )const
      {
        RetMap::const_iterator it = pred_RTrans.find(state);
        if( it == pred_RTrans.end() )
          return emptyReturns();
        else
          return it->second;
      }
      
      /**
       *  
       * @brief returns all return transitions with the given state as the return site
       *
       * This method returns the set of all return transitions whose return site is
       * the given state.
       *
       * @param - name: the state whose return site transitions to obtain
       * @return a set of return transitions with the given state playing the part
       *         of the return site.
       *
       */
      const Returns & retTrans( State state )const
      {
        RetMap::const_iterator it = ret_RTrans.find(state);
        if( it == ret_RTrans.end() )
          return emptyReturns();
        else
          return it->second;
      }
      
      /**
       *  
       * @brief tests whether the given state is the source of any internal transition
       *
       * This method determines whether the given state is the source of any
       * internal transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the source of some internal transition,
       *         false otherwise
       *
       */
      bool isFrom( State state ) const
      {
        Internals const & outgoing = fromTrans(state);
        return !outgoing.empty();
      }
      
      /**
       *  
       * @brief tests whether the given state is the target of any internal transition
       *
       * This method determines whether the given state is the target of any
       * internal transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the target of some internal transition,
       *         false otherwise
       *
       */
      bool isTo( State state ) const
      { 
        Internals const & incoming = toTrans(state);
        return !incoming.empty();
      }
      
      /**
       * 
       * @brief tests whether the given state is the call site of any call transition
       *
       * This method determines whether the given state is the call site of any
       * call transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the call site of some call transition,
       *         false otherwise
       *
       */
      bool isCall( State state ) const
      {
        Calls const & outgoing = callTrans(state);
        return !outgoing.empty();
      }
      
      /**
       *  
       * @brief tests whether the given state is the entry point of any call transition
       *
       * This method determines whether the given state is the entry point of any
       * call transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the entry point of some call transition,
       *         false otherwise
       *
       */
      bool isEntry( State state ) const
      {
        Calls const & incoming = entryTrans(state);
        return !incoming.empty();
      }
      
      /**
       *  
       * @brief tests whether the given state is the exit point of any return transition
       * 
       * This method determines whether the given state is the exit point of any
       * return transition.
       * 
       * @param - name: the state to test
       * @return true if the given state is the exit point of some return transition,
       *         false otherwise
       *
       */
      bool isExit( State state ) const
      {
        Returns const & outgoing = exitTrans(state);
        return !outgoing.empty();
      }
      
      /**
       *  
       * @brief tests whether the given state is the call predecessor for any return
       *        transition
       *
       * This method determines whether the given state is the call predecessor for
       * any return transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the call predecessor of some return 
       *         transition, false otherwise
       *
       */
      bool isPred( State state ) const
      {
        Returns const & outgoing = predTrans(state);
        return !outgoing.empty();
      }
      
      /**
       *  
       * @brief tests whether the given state is the return site of any return transition
       *
       * This method determines whether the given state is the return site of any
       * return transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the return site of some return transition,
       *         false otherwise
       *
       */
      bool isRet( State state ) const
      {
        Returns const & incoming = retTrans(state);
        return !incoming.empty();
      }
#else
      /**
       * 
       * @brief add an internal transition to the maps
       *
       * This method updates the maps associated with the source and target of the given
       * internal transition.  
       *
       * @param - intra: the internal transition to add to the maps
       *  
       */
      void addIntra( const Internal & intra )
      {
        //Update the map for the source of the transition.
        IntraMap::iterator it = from_ITrans.find(intra.first);
        if( it == from_ITrans.end() )
        {
          //Make a new label
          Label lbl;
          lbl.addSymbol(intra.second);
          Int iTrans = Triple(intra.first,lbl,intra.third);
          from_ITrans.insert(std::pair<St,Int>(intra.first,iTrans));
        }
        else  
        {
          //Add a symbol to the label.
          ((it->second).second).addSymbol(intra.second);
        }
        
        //Update the map for the target of the transition.
        it = to_ITrans.find(intra.third);
        if( it == to_ITrans.end() )
        {
          //Make a new label
          Label lbl;
          lbl.addSymbol(intra.second);
          Int iTrans = Triple(intra.first,lbl,intra.third);
          to_ITrans.insert(std::pair<St,Int>(intra.third,iTrans));
        }
        else
        {
          //Add a symbol to the label.
          ((it->second).second).addSymbol(intra.second);     
        }
      }
      
      /**
       * 
       * @brief remove an internal transition from the maps
       *
       * This method updates the maps associated with the source and target of the given
       * internal transition.  
       *
       * @param - intra: the internal transition to remove from the maps
       *  
       */
      void removeIntra( const Internal & intra )
      {
/*        //Update the map for the source of the transition.
          IntraMap::iterator it = from_ITrans.find(intra.first);
          if( it != from_ITrans.end() )
          {
          it->second.erase(intra);
          if( it->second.empty() )
          from_ITrans.erase(it);
          }
        
          //Update the map for the target of the transition.
          it = to_ITrans.find(intra.third);
          if( it != to_ITrans.end() )
          {
          it->second.erase(intra);
          if( it->second.empty() )
          to_ITrans.erase(it);
          }
*/      }

      /**
       * 
       * @brief add a call transition to the maps
       *
       * This method updates the maps associated with the call point and entry point of
       * the given call transition.  
       *
       * @param - call: the call transition to add to the maps
       *  
       */
      void addCall( const Call & call )
      {
        /*       //Update the maps for the call point of the transition.
                 CallMap::iterator it = call_CTrans.find(call.first);
                 if( it == call_CTrans.end() )
                 {
                 Calls cTrans;
                 cTrans.insert(call);
                 call_CTrans.insert(std::pair<St,Calls>(call.first,cTrans));
                 }
                 else  
                 it->second.insert(call);
        
                 //Update the maps for the entry point of the transition.
                 it = entry_CTrans.find(call.third);
                 if( it == entry_CTrans.end() )
                 { 
                 Calls cTrans;
                 cTrans.insert(call);
                 entry_CTrans.insert(std::pair<St,Calls>(call.third,cTrans));
                 }
                 else
                 it->second.insert(call);
        */    }
      
      /**
       * 
       * @brief remove a call transition from the maps
       *
       * This method updates the maps associated with the call point and entry point of
       * the given call transition.  
       *
       * @param - call: the call transition to remove from the maps
       *  
       */
      void removeCall( const Call & call )
      {
/*        //Update the maps for the call point of the transition.
          CallMap::iterator it = call_CTrans.find(call.first);
          if( it != call_CTrans.end() )
          {
          it->second.erase(call);
          if( it->second.empty() )
          call_CTrans.erase(it);
          }
        
          //Update the maps for the entry point of the transition.
          it = entry_CTrans.find(call.third);
          if( it != entry_CTrans.end() )
          {
          it->second.erase(call);
          if( it->second.empty() )
          entry_CTrans.erase(it);
          }
*/     }
      
      /**
       * 
       * @brief add a return transition to the maps
       *
       * This method updates the maps associated with the exit point, call predecessor,
       * and return point of the given return transition.  
       *
       * @param - ret: the return transition to add to the maps
       *  
       */
      void addRet( const Return & ret )
      {
        /*       //Update the maps for the exit point of the transition.
                 RetMap::iterator it = exit_RTrans.find(ret.first);
                 if( it == exit_RTrans.end() )
                 {
                 Returns rTrans;
                 rTrans.insert(ret);
                 exit_RTrans.insert(std::pair<St,Returns>(ret.first,rTrans));
                 }
                 else  
                 it->second.insert(ret);
        
                 //Update the maps for the call predecessor of the transition.
                 it = pred_RTrans.find(ret.second);
                 if( it == pred_RTrans.end() )
                 { 
                 Returns rTrans;
                 rTrans.insert(ret);
                 pred_RTrans.insert(std::pair<St,Returns>(ret.second,rTrans));
                 }
                 else
                 it->second.insert(ret);
          
                 //Update the maps for the return point of the transition.  
                 it = ret_RTrans.find(ret.fourth);
                 if( it == ret_RTrans.end() )
                 { 
                 Returns rTrans;
                 rTrans.insert(ret);
                 ret_RTrans.insert(std::pair<St,Returns>(ret.fourth,rTrans));
                 }
                 else
                 it->second.insert(ret);  
        */     }
      
      /**
       * 
       * @brief remove a return transition from the maps
       *
       * This method updates the maps associated with the exit point, call predecessor,
       * and return point of the given return transition.  
       *
       * @param - ret: the return transition to remove from the maps
       *  
       */
      void removeRet( const Return & ret )
      { 
/*        //Update the maps for the exit point of the transition.
          RetMap::iterator it = exit_RTrans.find(ret.first);
          if( it != exit_RTrans.end() )
          {
          it->second.erase(ret);
          if( it->second.empty() )
          exit_RTrans.erase(it);
          }
        
          //Update the maps for the call predecessor of the transition.
          it = pred_RTrans.find(ret.second);
          if( it != pred_RTrans.end() )
          {
          it->second.erase(ret);
          if( it->second.empty() )
          pred_RTrans.erase(it);
          }
        
          //Update the maps for the return point of the transition.
          it = ret_RTrans.find(ret.fourth);
          if( it != ret_RTrans.end() )
          {
          it->second.erase(ret);
          if( it->second.empty() )
          ret_RTrans.erase(it);
          }
*/      }
      
      /**
       *  
       * @brief returns all internal transitions with the given state as the source
       *
       * This method returns the set of all internal transitions whose source is the
       * given state.
       *
       * @param - name: the state whose outgoing transitions to obtain
       * @return a set of outgoing transitions for the given state
       *
       */
      const Internals fromTrans( St state ) const
      {
        /*       IntraMap::const_iterator it = from_ITrans.find(state);
                 if( it == from_ITrans.end() )
        */         return Internals();
        /*       else
                 return it->second;
        */     }
      
      /**
       *  
       * @brief returns all internal transitions with the given state as the target
       *
       * This method returns the set of all internal transitions whose target is the 
       * given state.
       *
       * @param - name: the state whose incoming transitions to obtain
       * @returm a set of incoming transitions for the given state
       *
       */
      const Internals toTrans( St state ) const
      {
        /*       IntraMap::const_iterator it = to_ITrans.find(state);
                 if( it == to_ITrans.end() )
        */         return Internals();
        /*       else
                 return it->second;
        */     } 
      
      /**
       *  
       * @brief returns all call transitions with the given state as the call site
       *
       * This method returns the set of all call transitions whose call site is the
       * given state.
       *
       * @param - name: the state whose call site transitions to obtain
       * @return a set of call transitions with the given state playing the part of 
       *         the call site
       *
       */
      const Calls callTrans( St state ) const
      {
        /*       CallMap::const_iterator it = call_CTrans.find(state);
                 if( it == call_CTrans.end() )
        */        return Calls();
        /*      else
                return it->second;
        */   }
       
      /**
       *  
       * @brief returns all call transitions with the given state as the entry point
       *
       * This method returns the set of all call transitions whose entry point is the
       * given state.
       *
       * @param - name: the state whose entry transitions to obtain
       * @return a set of call transitions with the given state playing the part of
       *         the entry point
       *
       */
      const Calls entryTrans( St state ) const
      { 
        /*      CallMap::const_iterator it = entry_CTrans.find(state);
                if( it == entry_CTrans.end() )
        */       return Calls();
        /*     else
               return it->second;
        */   }
      
      /**
       *  
       * @brief returns all return transitions with the given state as the exit point
       *
       * This method returns the set of all return transitions whose exit point is
       * the given state.
       *
       * @param - name: the state whose exit transitions to obtain
       * @return a set of return transitions with the given state playing the part of
       *         the exit point
       *
       */
      const Returns exitTrans( St state ) const
      {
        /*      RetMap::const_iterator it = exit_RTrans.find(state);
                if( it == exit_RTrans.end() )
        */        return Returns();
        /*      else
                return it->second;
        */    }
      
      /**
       *  
       * @brief returns all return transitions with the given state as the call predecessor
       *
       * This method returns the set of all return transitions whose call predecessor
       * is the given state.
       *
       * @param - name: the state whose call predecessor transitions to obtain
       * @return a set of return transitions with the given state playing the part of 
       *         the call predecessor.
       *
       */
      const Returns predTrans( St state )const
      {
        /*      RetMap::const_iterator it = pred_RTrans.find(state);
                if( it == pred_RTrans.end() )
        */        return Returns();
        /*      else
                return it->second;
        */    }
      
      /**
       *  
       * @brief returns all return transitions with the given state as the return site
       *
       * This method returns the set of all return transitions whose return site is
       * the given state.
       *
       * @param - name: the state whose return site transitions to obtain
       * @return a set of return transitions with the given state playing the part
       *         of the return site.
       *
       */
      const Returns retTrans( St state )const
      {
        /*    RetMap::const_iterator it = ret_RTrans.find(state);
              if( it == ret_RTrans.end() )
        */        return Returns();
        /*     else
               return it->second;
        */   }
      
      /**
       *  
       * @brief tests whether the given state is the source of any internal transition
       *
       * This method determines whether the given state is the source of any
       * internal transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the source of some internal transition,
       *         false otherwise
       *
       */
      bool isFrom( St state ) const
      {
        /*       IntraMap::iterator it = from_ITrans.find(state);
                 if( it != from_ITrans.end() )
                 {
                 if( !(it.second.empty()) )
                 return true;
                 }
        */        return false;
      }
      
      /**
       *  
       * @brief tests whether the given state is the target of any internal transition
       *
       * This method determines whether the given state is the target of any
       * internal transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the target of some internal transition,
       *         false otherwise
       *
       */
      bool isTo( St state ) const
      { 
        /*       IntraMap::iterator it = to_ITrans.find(state);
                 if( it != to_ITrans.end() )
                 {
                 if( !(it.second.empty()) )
                 return true;
                 }
        */      return false;
      }
      
      /**
       * 
       * @brief tests whether the given state is the call site of any call transition
       *
       * This method determines whether the given state is the call site of any
       * call transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the call site of some call transition,
       *         false otherwise
       *
       */
      bool isCall( St state ) const
      {
        /*       CallMap::iterator it = call_CTrans.find(state);
                 if( it != call_CTrans.end() )
                 {
                 if( !(it.second.empty()) )
                 return true;
                 }
        */       return false;
      }
      
      /**
       *  
       * @brief tests whether the given state is the entry point of any call transition
       *
       * This method determines whether the given state is the entry point of any
       * call transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the entry point of some call transition,
       *         false otherwise
       *
       */
      bool isEntry( St state ) const
      {
        /*       CallMap::iterator it = entry_CTrans.find(state);
                 if( it != entry_CTrans.end() )
                 {
                 if( !(it.second.empty()) )
                 return true;
                 }
        */      return false;
      }
      
      /**
       *  
       * @brief tests whether the given state is the exit point of any return transition
       * 
       * This method determines whether the given state is the exit point of any
       * return transition.
       * 
       * @param - name: the state to test
       * @return true if the given state is the exit point of some return transition,
       *         false otherwise
       *
       */
      bool isExit( St state ) const
      {
/*        RetMap::iterator it = exit_RTrans.find(state);
          if( it != exit_RTrans.end() )
          {
          if( !(it.second.empty()) )
          return true;
          }
*/       return false;
      }
      
      /**
       *  
       * @brief tests whether the given state is the call predecessor for any return
       *        transition
       *
       * This method determines whether the given state is the call predecessor for
       * any return transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the call predecessor of some return 
       *         transition, false otherwise
       *
       */
      bool isPred( St state ) const
      {
        /*       RetMap::iterator it = pred_RTrans.find(state);
                 if( it != pred_RTrans.end() )
                 {
                 if( !(it.second.empty()) )
                 return true;
                 }
        */      return false;
      }
      
      /**
       *  
       * @brief tests whether the given state is the return site of any return transition
       *
       * This method determines whether the given state is the return site of any
       * return transition.
       *
       * @param - name: the state to test
       * @return true if the given state is the return site of some return transition,
       *         false otherwise
       *
       */
      bool isRet( St state ) const
      {
        /*       RetMap::iterator it = ret_RTrans.find(state);
                 if( it != ret_RTrans.end() )
                 {
                 if( !(it.second.empty()) )
                 return true;
                 }
        */      return false;
      }
#endif
      
      /**
       *
       * @brief tests whether this collection of transition maps is equivalent 
       *        to the collection of transition maps 'other'
       *
       * This method tests the equivalence of these transition maps and the transition
       * maps in 'other'.
       *
       * @param - other: the TransitionInfo to compare this TransitionInfo to
       * @return true if this TransitionInfo is equivalent to the TransitionInfo
       *         'other'
       *
       */
      bool operator==( const TransitionInfo & other ) const
      {
        return ( (from_ITrans == other.from_ITrans) &&  
                 (to_ITrans == other.to_ITrans) && 
                 (call_CTrans == other.call_CTrans) &&  
                 (entry_CTrans == other.entry_CTrans) &&  
                 (exit_RTrans == other.exit_RTrans) && 
                 (pred_RTrans == other.pred_RTrans) && 
                 (ret_RTrans == other.ret_RTrans) );
      }
            
      /**
       *  
       * @brief clears the transition maps
       *
       * This method removes all transition information from the transition maps.
       *
       */
      void clearMaps()
      {
        from_ITrans.clear();
        to_ITrans.clear();
        
        call_CTrans.clear();
        entry_CTrans.clear();
        
        exit_RTrans.clear();
        pred_RTrans.clear();
        ret_RTrans.clear();
      }
    
      //
      // Variables
      //

    private:
      
      // maps to speed up transition search
      IntraMap from_ITrans;
      IntraMap to_ITrans;
        
      CallMap call_CTrans;
      CallMap entry_CTrans;
        
      RetMap exit_RTrans;
      RetMap pred_RTrans;  
      RetMap ret_RTrans;
    };


  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


#endif
