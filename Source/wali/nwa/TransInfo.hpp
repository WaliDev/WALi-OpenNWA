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
        if (this == &other)     
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
       * TODO
       */
      void addIntra(const Internal & intra)
      {
        IntraMap::iterator it = from_ITrans.find(intra.first->getStateKey());
        if( it == from_ITrans.end() )
        {
          Internals iTrans;
          iTrans.insert(intra);
          from_ITrans.insert(std::pair<wali::Key,Internals>(intra.first->getStateKey(),iTrans));
        }
        else  
          it->second.insert(intra);
        
        it = to_ITrans.find(intra.third->getStateKey());
        if( it == to_ITrans.end() )
        {
          Internals iTrans;
          iTrans.insert(intra);
          to_ITrans.insert(std::pair<wali::Key,Internals>(intra.third->getStateKey(),iTrans));
        }
        else
          it->second.insert(intra);       
      }
      
      /**
       * TODO
       */
      void removeIntra(const Internal & intra)
      {
        IntraMap::iterator it = from_ITrans.find(intra.first->getStateKey());
        if( it != from_ITrans.end() )
        {
          it->second.erase(intra);
          if( it->second.empty() )
            from_ITrans.erase(it);
        }
        it = to_ITrans.find(intra.third->getStateKey());
        if( it != from_ITrans.end() )
        {
          it->second.erase(intra);
          if( it->second.empty() )
            to_ITrans.erase(it);
        }
      }

      /**
       * TODO
       */
      void addCall(const Call & call)
      {
        CallMap::iterator it = call_CTrans.find(call.first->getStateKey());
        if( it == call_CTrans.end() )
        {
          Calls cTrans;
          cTrans.insert(call);
          call_CTrans.insert(std::pair<wali::Key,Calls>(call.first->getStateKey(),cTrans));
        }
        else  
          it->second.insert(call);
        
        it = entry_CTrans.find(call.third->getStateKey());
        if( it == entry_CTrans.end() )
        { 
          Calls cTrans;
          cTrans.insert(call);
          entry_CTrans.insert(std::pair<wali::Key,Calls>(call.third->getStateKey(),cTrans));
        }
        else
          it->second.insert(call);
      }
      /**
       * TODO
       */
      void removeCall(const Call & call)
      {
        CallMap::iterator it = call_CTrans.find(call.first->getStateKey());
        if( it != call_CTrans.end() )
        {
          it->second.erase(call);
          if( it->second.empty() )
            call_CTrans.erase(it);
        }
        it = entry_CTrans.find(call.third->getStateKey());
        if( it != entry_CTrans.end() )
        {
          it->second.erase(call);
          if( it->second.empty() )
            entry_CTrans.erase(it);
        }
      }
      
      /**
       * TODO
       */
      void addRet(const Return & ret)
      {
        RetMap::iterator it = exit_RTrans.find(ret.first->getStateKey());
        if( it == exit_RTrans.end() )
        {
          Returns rTrans;
          rTrans.insert(ret);
          exit_RTrans.insert(std::pair<wali::Key,Returns>(ret.first->getStateKey(),rTrans));
        }
        else  
          it->second.insert(ret);
        
        it = pred_RTrans.find(ret.second->getStateKey());
        if( it == pred_RTrans.end() )
        { 
          Returns rTrans;
          rTrans.insert(ret);
          pred_RTrans.insert(std::pair<wali::Key,Returns>(ret.second->getStateKey(),rTrans));
        }
        else
          it->second.insert(ret);
          
        it = ret_RTrans.find(ret.fourth->getStateKey());
        if( it == ret_RTrans.end() )
        { 
          Returns rTrans;
          rTrans.insert(ret);
          ret_RTrans.insert(std::pair<wali::Key,Returns>(ret.fourth->getStateKey(),rTrans));
        }
        else
          it->second.insert(ret);  
      }
      /**
       * TODO
       */
      void removeRet(const Return & ret)
      { 
        RetMap::iterator it = exit_RTrans.find(ret.first->getStateKey());
        if( it != exit_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            exit_RTrans.erase(it);
        }
        it = pred_RTrans.find(ret.second->getStateKey());
        if( it != pred_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            pred_RTrans.erase(it);
        }
        it = ret_RTrans.find(ret.fourth->getStateKey());
        if( it != ret_RTrans.end() )
        {
          it->second.erase(ret);
          if( it->second.empty() )
            ret_RTrans.erase(it);
        }
      }
      
      const Internals fromTrans( St* name ) const
      {
        IntraMap::const_iterator it = from_ITrans.find(name->getStateKey());
        if( it == from_ITrans.end() )
          return Internals();
        else
          return it->second;
      }
      const Internals toTrans( St* name ) const
      {
        IntraMap::const_iterator it = to_ITrans.find(name->getStateKey());
        if( it == to_ITrans.end() )
          return Internals();
        else
          return it->second;
      } 
      const Calls callTrans( St* name ) const
      {
        CallMap::const_iterator it = call_CTrans.find(name->getStateKey());
        if( it == call_CTrans.end() )
          return Calls();
        else
          return it->second;
      }
      const Calls entryTrans( St* name ) const
      { 
        CallMap::const_iterator it = entry_CTrans.find(name->getStateKey());
        if( it == entry_CTrans.end() )
          return Calls();
        else
          return it->second;
      }
      const Returns exitTrans( St* name ) const
      {
        RetMap::const_iterator it = exit_RTrans.find(name->getStateKey());
        if( it == exit_RTrans.end() )
          return Returns();
        else
          return it->second;
      }
      const Returns predTrans( St* name )const
      {
        RetMap::const_iterator it = pred_RTrans.find(name->getStateKey());
        if( it == pred_RTrans.end() )
          return Returns();
        else
          return it->second;
      }
      const Returns retTrans( St* name )const
      {
        RetMap::const_iterator it = ret_RTrans.find(name->getStateKey());
        if( it == ret_RTrans.end() )
          return Returns();
        else
          return it->second;
      }
      
      bool isFrom( St* name ) const
      {
        IntraMap::iterator it = from_ITrans.find(name->getStateKey());
        if( it != from_ITrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      bool isTo( St* name ) const
      { 
        IntraMap::iterator it = to_ITrans.find(name->getStateKey());
        if( it != to_ITrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      bool isCall( St* name ) const
      {
        CallMap::iterator it = call_CTrans.find(name->getStateKey());
        if( it != call_CTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      bool isEntry( St* name ) const
      {
        CallMap::iterator it = entry_CTrans.find(name->getStateKey());
        if( it != entry_CTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      bool isExit( St* name ) const
      {
        RetMap::iterator it = exit_RTrans.find(name->getStateKey());
        if( it != exit_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      bool isPred( St* name ) const
      {
        RetMap::iterator it = pred_RTrans.find(name->getStateKey());
        if( it != pred_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      bool isRet( St* name ) const
      {
        RetMap::iterator it = ret_RTrans.find(name->getStateKey());
        if( it != ret_RTrans.end() )
        {
          if( !(it.second.empty()) )
            return true;
        }
        return false;
      }
      
      /**
       * TODO
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
       * TODO
       */
      bool operator<( const TransInfo & rhs ) const
      {
        if( from_ITrans.size() == rhs.from_ITrans.size() )
        {
          if( to_ITrans.size() == rhs.to_ITrans.size() )
          {
            if( call_CTrans.size() == rhs.call_CTrans.size() )
            {
              if( entry_CTrans.size() == rhs.entry_CTrans.size() )
              {
                if( exit_RTrans.size() == rhs.exit_RTrans.size() )
                {
                  if( pred_RTrans.size() == rhs.pred_RTrans.size() )
                  {
                    return ( ret_RTrans.size() < rhs.ret_RTrans.size() )
                  }
                  else
                    return ( pred_RTrans.size() < rhs.pred_RTrans.size() );
                }
                else
                  return ( exit_RTrans.size() < rhs.exit_RTrans.size() );
              }
              else
                return ( entry_CTrans.size() < rhs.entry_CTrans.size() );
            }
            else
              return ( call_CTrans.size() < rhs.call_CTrans.size() );
          }
          else
            return ( to_ITrans.size() < rhs.to_ITrans.size() )
        }
        else
          return ( from_ITrans.size() < rhs.from_ITrans.size() );
      }   
      
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
#endif