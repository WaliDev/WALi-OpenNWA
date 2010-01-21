#ifndef wali_nwa_TransInfo_GUARD
#define wali_nwa_TransInfo_GUARD 1

/**
 * @author Amanda Burton
 */
 
// ::wali
#include "wali/Key.hpp" 
 
// std::c++
#include <map> 
#include <set>
 
namespace wali
{ 
  namespace nwa
  {
    /**
     *
     * This class is used to keep track of the maps from state to transition for an NWA.
     *
     */
#ifndef KEY
    template<typename St,typename Call,typename Internal,typename Return>
   class TransInfo
    {    
      public:
        typedef std::set<Internal> Internals;
        typedef std::set<Call> Calls;
        typedef std::set<Return> Returns;
        typedef std::map<wali::Key,Internals> IntraMap;
        typedef std::map<wali::Key,Calls> CallMap;
        typedef std::map<wali::Key,Returns> RetMap;
    
      //
      // Methods
      //
      
      public:
      
      //Constructors and Destructor
      TransInfo( ) { }

      TransInfo( const TransInfo & other )
      {
        clearMaps();
        
        from_ITrans = other.from_ITrans;
        to_ITrans = other.to_ITrans;
        
        call_CTrans = other.call_CTrans;
        entry_CTrans = other.entry_CTrans;
        
        exit_RTrans = other.exit_RTrans;
        pred_RTrans = other.pred_RTrans;
        ret_RTrans = other.ret_RTrans;
      }

      TransInfo & operator=( const TransInfo & other )
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
  
      ~TransInfo( ) 
      {
        clearMaps();
      }
     
      /**
       * 
       * @brief add an internal transition to the maps
       *
       * This method updates the maps associated with the source and target of the
       * given internal transition.  
       *
       * @param - intra: the internal transition to add to the maps
       *  
       */
      void addIntra( const Internal & intra )
      {
        //Update the map for the source of the transition.
        IntraMap::iterator it = from_ITrans.find(getKey(intra.first));
        if( it == from_ITrans.end() )
        {
          Internals iTrans;
          iTrans.insert(intra);
          from_ITrans.insert(std::pair<wali::Key,Internals>(getKey(intra.first),iTrans));
        }
        else  
          it->second.insert(intra);
        
        //Update the map for the target of the transition.
        it = to_ITrans.find(getKey(intra.third));
        if( it == to_ITrans.end() )
        {
          Internals iTrans;
          iTrans.insert(intra);
          to_ITrans.insert(std::pair<wali::Key,Internals>(getKey(intra.third),iTrans));
        }
        else
          it->second.insert(intra);       
      }
      
      /**
       * 
       * @brief remove an internal transition from the maps
       *
       * This method updates the maps associated with the source and target of the
       * given internal transition.  
       *
       * @param - intra: the internal transition to remove from the maps
       *  
       */
      void removeIntra( const Internal & intra )
      {
        //Update the map for the source of the transition.
        IntraMap::iterator it = from_ITrans.find(getKey(intra.first));
        if( it != from_ITrans.end() )
        {
          it->second.erase(intra);
          if( it->second.empty() )
            from_ITrans.erase(it);
        }
        
        //Update the map for the target of the transition.
        it = to_ITrans.find(getKey(intra.third));
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
       * This method updates the maps associated with the call point and entry point 
       * of the given call transition.  
       *
       * @param - call: the call transition to add to the maps
       *  
       */
      void addCall( const Call & call )
      {
        //Update the maps for the call point of the transition.
        CallMap::iterator it = call_CTrans.find(getKey(call.first));
        if( it == call_CTrans.end() )
        {
          Calls cTrans;
          cTrans.insert(call);
          call_CTrans.insert(std::pair<wali::Key,Calls>(getKey(call.first),cTrans));
        }
        else  
          it->second.insert(call);
        
        //Update the maps for the entry point of the transition.
        it = entry_CTrans.find(getKey(call.third));
        if( it == entry_CTrans.end() )
        { 
          Calls cTrans;
          cTrans.insert(call);
          entry_CTrans.insert(std::pair<wali::Key,Calls>(getKey(call.third),cTrans));
        }
        else
          it->second.insert(call);
      }
      
      /**
       * 
       * @brief remove a call transition from the maps
       *
       * This method updates the maps associated with the call point and entry point 
       * of the given call transition.  
       *
       * @param - call: the call transition to remove from the maps
       *  
       */
      void removeCall( const Call & call )
      {
        //Update the maps for the call point of the transition.
        CallMap::iterator it = call_CTrans.find(getKey(call.first));
        if( it != call_CTrans.end() )
        {
          it->second.erase(call);
          if( it->second.empty() )
            call_CTrans.erase(it);
        }
        
        //Update the maps for the entry point of the transition.
        it = entry_CTrans.find(getKey(call.third));
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
        //Update the maps for the exit point of the transition.
        RetMap::iterator it = exit_RTrans.find(getKey(ret.first));
        if( it == exit_RTrans.end() )
        {
          Returns rTrans;
          rTrans.insert(ret);
          exit_RTrans.insert(std::pair<wali::Key,Returns>(getKey(ret.first),rTrans));
        }
        else  
          it->second.insert(ret);
        
        //Update the maps for the call predecessor of the transition.
        it = pred_RTrans.find(getKey(ret.second));
        if( it == pred_RTrans.end() )
        { 
          Returns rTrans;
          rTrans.insert(ret);
          pred_RTrans.insert(std::pair<wali::Key,Returns>(getKey(ret.second),rTrans));
        }
        else
          it->second.insert(ret);
          
        //Update the maps for the return point of the transition.  
        it = ret_RTrans.find(getKey(ret.fourth));
        if( it == ret_RTrans.end() )
        { 
          Returns rTrans;
          rTrans.insert(ret);
          ret_RTrans.insert(std::pair<wali::Key,Returns>(getKey(ret.fourth),rTrans));
        }
        else
          it->second.insert(ret);  
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
        RetMap::iterator it = exit_RTrans.find(getKey(ret.first));
        if( it != exit_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            exit_RTrans.erase(it);
        }
        
        //Update the maps for the call predecessor of the transition.
        it = pred_RTrans.find(getKey(ret.second));
        if( it != pred_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            pred_RTrans.erase(it);
        }
        
        //Update the maps for the return point of the transition.
        it = ret_RTrans.find(getKey(ret.fourth));
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
      const Internals fromTrans( const St * name ) const
      {
        //TODO fromTrans( StName )
        IntraMap::const_iterator it = from_ITrans.find(name->getStateKey());
        if( it == from_ITrans.end() )
          return Internals();
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
      const Internals toTrans( const St * name ) const
      {
        //TODO toTrans( StName )
        IntraMap::const_iterator it = to_ITrans.find(name->getStateKey());
        if( it == to_ITrans.end() )
          return Internals();
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
      const Calls callTrans( const St * name ) const
      {
        //TODO callTrans( StName )
        CallMap::const_iterator it = call_CTrans.find(name->getStateKey());
        if( it == call_CTrans.end() )
          return Calls();
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
      const Calls entryTrans( const St * name ) const
      { 
        //TODO entryTrans( StName )
        CallMap::const_iterator it = entry_CTrans.find(name->getStateKey());
        if( it == entry_CTrans.end() )
          return Calls();
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
      const Returns exitTrans( const St * name ) const
      {
        //TODO exitTrans( StName )
        RetMap::const_iterator it = exit_RTrans.find(name->getStateKey());
        if( it == exit_RTrans.end() )
          return Returns();
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
      const Returns predTrans( const St * name )const
      {
        //TODO predTrans( StName )
        RetMap::const_iterator it = pred_RTrans.find(name->getStateKey());
        if( it == pred_RTrans.end() )
          return Returns();
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
      const Returns retTrans( const St * name )const
      {
        //TODO retTrans( StName )
        RetMap::const_iterator it = ret_RTrans.find(name->getStateKey());
        if( it == ret_RTrans.end() )
          return Returns();
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
      bool isFrom( const St & name ) const
      {
        IntraMap::iterator it = from_ITrans.find(name.getStateKey());
        if( it != from_ITrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isTo( const St & name ) const
      { 
        IntraMap::iterator it = to_ITrans.find(name.getStateKey());
        if( it != to_ITrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isCall( const St & name ) const
      {
        CallMap::iterator it = call_CTrans.find(name.getStateKey());
        if( it != call_CTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isEntry( const St & name ) const
      {
        CallMap::iterator it = entry_CTrans.find(name.getStateKey());
        if( it != entry_CTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isExit( const St & name ) const
      {
        RetMap::iterator it = exit_RTrans.find(name.getStateKey());
        if( it != exit_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isPred( const St & name ) const
      {
        RetMap::iterator it = pred_RTrans.find(name.getStateKey());
        if( it != pred_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isRet( const St & name ) const
      {
        RetMap::iterator it = ret_RTrans.find(name.getStateKey());
        if( it != ret_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      
      /**
       *
       * @brief tests whether this collection of transition maps is equivalent 
       *        to the collection of transition maps 'other'
       *
       * This method tests the equivalence of these transition maps and 
       * the transition maps in 'other'.
       *
       * @param - other: the TransInfo to compare this TransInfo to
       * @return true if this TransInfo is equivalent to the TransInfo
       *         'other'
       *
       */
      bool operator==( const TransInfo & other ) const
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
#else //ifdef KEY
   template<typename St,typename Call,typename Internal,typename Return>
   class TransInfo
    {    
      public:
        typedef std::set<Internal> Internals;
        typedef std::set<Call> Calls;
        typedef std::set<Return> Returns;
        typedef std::map<wali::Key,Internals> IntraMap;
        typedef std::map<wali::Key,Calls> CallMap;
        typedef std::map<wali::Key,Returns> RetMap;
    
      //
      // Methods
      //
      
      public:
      
      //Constructors and Destructor
      TransInfo( ) { }

      TransInfo( const TransInfo & other )
      {
        clearMaps();
        
        from_ITrans = other.from_ITrans;
        to_ITrans = other.to_ITrans;
        
        call_CTrans = other.call_CTrans;
        entry_CTrans = other.entry_CTrans;
        
        exit_RTrans = other.exit_RTrans;
        pred_RTrans = other.pred_RTrans;
        ret_RTrans = other.ret_RTrans;
      }

      TransInfo & operator=( const TransInfo & other )
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
  
      ~TransInfo( ) 
      {
        clearMaps();
      }
     
      /**
       * 
       * @brief add an internal transition to the maps
       *
       * This method updates the maps associated with the source and target of the
       * given internal transition.  
       *
       * @param - intra: the internal transition to add to the maps
       *  
       */
      void addIntra( const Internal & intra )
      {
        //Update the map for the source of the transition.
        IntraMap::iterator it = from_ITrans.find(intra.first.getName());
        if( it == from_ITrans.end() )
        {
          Internals iTrans;
          iTrans.insert(intra);
          from_ITrans.insert(std::pair<wali::Key,Internals>(intra.first.getName(),iTrans));
        }
        else  
          it->second.insert(intra);
        
        //Update the map for the target of the transition.
        it = to_ITrans.find(intra.third.getName());
        if( it == to_ITrans.end() )
        {
          Internals iTrans;
          iTrans.insert(intra);
          to_ITrans.insert(std::pair<wali::Key,Internals>(intra.third.getName(),iTrans));
        }
        else
          it->second.insert(intra);       
      }
      
      /**
       * 
       * @brief remove an internal transition from the maps
       *
       * This method updates the maps associated with the source and target of the
       * given internal transition.  
       *
       * @param - intra: the internal transition to remove from the maps
       *  
       */
      void removeIntra( const Internal & intra )
      {
        //Update the map for the source of the transition.
        IntraMap::iterator it = from_ITrans.find(intra.first.getName());
        if( it != from_ITrans.end() )
        {
          it->second.erase(intra);
          if( it->second.empty() )
            from_ITrans.erase(it);
        }
        
        //Update the map for the target of the transition.
        it = to_ITrans.find(intra.third.getName());
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
       * This method updates the maps associated with the call point and entry point 
       * of the given call transition.  
       *
       * @param - call: the call transition to add to the maps
       *  
       */
      void addCall( const Call & call )
      {
        //Update the maps for the call point of the transition.
        CallMap::iterator it = call_CTrans.find(call.first.getName());
        if( it == call_CTrans.end() )
        {
          Calls cTrans;
          cTrans.insert(call);
          call_CTrans.insert(std::pair<wali::Key,Calls>(call.first.getName(),cTrans));
        }
        else  
          it->second.insert(call);
        
        //Update the maps for the entry point of the transition.
        it = entry_CTrans.find(call.third.getName());
        if( it == entry_CTrans.end() )
        { 
          Calls cTrans;
          cTrans.insert(call);
          entry_CTrans.insert(std::pair<wali::Key,Calls>(call.third.getName(),cTrans));
        }
        else
          it->second.insert(call);
      }
      
      /**
       * 
       * @brief remove a call transition from the maps
       *
       * This method updates the maps associated with the call point and entry point 
       * of the given call transition.  
       *
       * @param - call: the call transition to remove from the maps
       *  
       */
      void removeCall( const Call & call )
      {
        //Update the maps for the call point of the transition.
        CallMap::iterator it = call_CTrans.find(call.first.getName());
        if( it != call_CTrans.end() )
        {
          it->second.erase(call);
          if( it->second.empty() )
            call_CTrans.erase(it);
        }
        
        //Update the maps for the entry point of the transition.
        it = entry_CTrans.find(call.third.getName());
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
        //Update the maps for the exit point of the transition.
        RetMap::iterator it = exit_RTrans.find(ret.first.getName());
        if( it == exit_RTrans.end() )
        {
          Returns rTrans;
          rTrans.insert(ret);
          exit_RTrans.insert(std::pair<wali::Key,Returns>(ret.first.getName(),rTrans));
        }
        else  
          it->second.insert(ret);
        
        //Update the maps for the call predecessor of the transition.
        it = pred_RTrans.find(ret.second.getName());
        if( it == pred_RTrans.end() )
        { 
          Returns rTrans;
          rTrans.insert(ret);
          pred_RTrans.insert(std::pair<wali::Key,Returns>(ret.second.getName(),rTrans));
        }
        else
          it->second.insert(ret);
          
        //Update the maps for the return point of the transition.  
        it = ret_RTrans.find(ret.fourth.getName());
        if( it == ret_RTrans.end() )
        { 
          Returns rTrans;
          rTrans.insert(ret);
          ret_RTrans.insert(std::pair<wali::Key,Returns>(ret.fourth.getName(),rTrans));
        }
        else
          it->second.insert(ret);  
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
        RetMap::iterator it = exit_RTrans.find(ret.first.getName());
        if( it != exit_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            exit_RTrans.erase(it);
        }
        
        //Update the maps for the call predecessor of the transition.
        it = pred_RTrans.find(ret.second.getName());
        if( it != pred_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            pred_RTrans.erase(it);
        }
        
        //Update the maps for the return point of the transition.
        it = ret_RTrans.find(ret.fourth.getName());
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
      const Internals fromTrans( const St & name ) const
      {
        IntraMap::const_iterator it = from_ITrans.find(name.getName());
        if( it == from_ITrans.end() )
          return Internals();
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
      const Internals toTrans( const St & name ) const
      {
        IntraMap::const_iterator it = to_ITrans.find(name.getName());
        if( it == to_ITrans.end() )
          return Internals();
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
      const Calls callTrans( const St & name ) const
      {
        CallMap::const_iterator it = call_CTrans.find(name.getName());
        if( it == call_CTrans.end() )
          return Calls();
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
      const Calls entryTrans( const St & name ) const
      { 
        CallMap::const_iterator it = entry_CTrans.find(name.getName());
        if( it == entry_CTrans.end() )
          return Calls();
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
      const Returns exitTrans( const St & name ) const
      {
        RetMap::const_iterator it = exit_RTrans.find(name.getName());
        if( it == exit_RTrans.end() )
          return Returns();
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
      const Returns predTrans( const St & name )const
      {
        RetMap::const_iterator it = pred_RTrans.find(name.getName());
        if( it == pred_RTrans.end() )
          return Returns();
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
      const Returns retTrans( const St & name )const
      {
        RetMap::const_iterator it = ret_RTrans.find(name.getName());
        if( it == ret_RTrans.end() )
          return Returns();
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
      bool isFrom( const St & name ) const
      {
        IntraMap::iterator it = from_ITrans.find(name.getName());
        if( it != from_ITrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isTo( const St & name ) const
      { 
        IntraMap::iterator it = to_ITrans.find(name.getName());
        if( it != to_ITrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isCall( const St & name ) const
      {
        CallMap::iterator it = call_CTrans.find(name.getName());
        if( it != call_CTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isEntry( const St & name ) const
      {
        CallMap::iterator it = entry_CTrans.find(name.getName());
        if( it != entry_CTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isExit( const St & name ) const
      {
        RetMap::iterator it = exit_RTrans.find(name.getName());
        if( it != exit_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isPred( const St & name ) const
      {
        RetMap::iterator it = pred_RTrans.find(name.getName());
        if( it != pred_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
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
      bool isRet( const St & name ) const
      {
        RetMap::iterator it = ret_RTrans.find(name.getName());
        if( it != ret_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      
      /**
       *
       * @brief tests whether this collection of transition maps is equivalent 
       *        to the collection of transition maps 'other'
       *
       * This method tests the equivalence of these transition maps and 
       * the transition maps in 'other'.
       *
       * @param - other: the TransInfo to compare this TransInfo to
       * @return true if this TransInfo is equivalent to the TransInfo
       *         'other'
       *
       */
      bool operator==( const TransInfo & other ) const
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
#endif
  }
}
#endif