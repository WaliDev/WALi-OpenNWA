#ifndef wali_nwa_WordRecConfig_GUARD
#define wali_nwa_WordRecConfig_GUARD 1

/**
 * @author Amanda Burton
 */
 
// ::wali
#include "wali/nws/NWS.hpp"
#include "wali/nws/NWSNode.hpp"

// std::c++
#include <deque>

namespace wali
{
  namespace nwa
  {
    class WordRecConfig
    {
      //
      // Methods
      //

    public:
      //Constructors and Destructor
      WordRecConfig( );
      WordRecConfig( wali::nws::NWS word );
      WordRecConfig( const WordRecConfig & other );
      WordRecConfig & operator=( const WordRecConfig & other );
  
      ~WordRecConfig( );


      //Accessors
      
      /**
       *
       */
      bool isEmpty();
      
      /**
       *
       */
      bool hasNext();
      
      /**
       *
       */
      wali::nws::NWSNode * getNext();
      
      /**
       *
       */
      void removeNext();
      
      /**
       *
       */
      wali::nws::NWSNode * getCall();
      
      /** 
       *
       */
      void setWord( wali::nws::NWS word );

      /**
       *
       */
      bool hasCallState();

      /**
       *
       */
      Key getCallState();
      
      /**
       *
       */
      void addCallState( Key call );
      
      /**
       *
       */
      void removeCallState();
      
      //Utilities
      
      /** 
       *
       * @parm the output stream to which to print the node
       * @return the output stream to which the node was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       */
      bool operator==( const WordRecConfig & other ) const;
      
      /**
       *
       */
      bool operator<( const WordRecConfig & rhs ) const;
      
      //
      // Variables
      // 
      protected:
        std::deque<Key> st;
        wali::nws::NWS word;
    };
    
    //Constructors and Destructor
  
    WordRecConfig::WordRecConfig()
    {
      
    }  
      
    WordRecConfig::WordRecConfig( wali::nws::NWS word )
    {
      this->word = word;
    }
    
    WordRecConfig::WordRecConfig( const WordRecConfig & other )
    {
      word = other.word;
      st = other.st;
    }
    
    WordRecConfig & WordRecConfig::operator=( const WordRecConfig & other )
    {
      this->word = other.word;
      this->st = other.st;
      
      return *this;
    }
    
    WordRecConfig::~WordRecConfig()
    {
      st.clear();
    }
      
    //Accessors
    
    /**
     *
     */
    bool WordRecConfig::isEmpty()
    {
      return word.isEmpty();
    }
    
    /**
     *
     */
    bool WordRecConfig::hasNext()
    {
      return !(word.nextNode() == NULL);
    }
     
    /**
     *
     */
    wali::nws::NWSNode * WordRecConfig::getNext()
    {
      return word.nextNode();
    }
    
    /**
     *
     */
    void WordRecConfig::removeNext()
    {
      word.removeNode();
    }
    
    /**
     *
     */
    wali::nws::NWSNode * WordRecConfig::getCall()
    {
      return word.stackTop();
    }
      
    /** 
     *
     */
    void WordRecConfig::setWord( wali::nws::NWS word )
    {
      this->word = word;
    }

    /**
     *
     */
    bool WordRecConfig::hasCallState()
    {
      return !st.empty();
    }

    /**
     *
     */
    Key WordRecConfig::getCallState()
    {
      return st.back();
    }
      
    /**
     *
     */
    void WordRecConfig::addCallState( Key call )
    {
      st.push_back(call);
    }
      
    /**
     *
     */
    void WordRecConfig::removeCallState()
    {
      st.pop_back();
    }
      
    //Utilities
      
    /** 
     *
     * @parm the output stream to which to print the node
     * @return the output stream to which the node was printed
     *
     */
    std::ostream & WordRecConfig::print( std::ostream & o ) const
    {
      word.print(o);
      
      o << "[";
      bool first = true;
      for( std::deque<Key>::const_iterator it = st.begin();
            it != st.end(); it++ )
      {
        if( !first )
          o << ", ";
        else
          first = false;
        printKey(o,*it);
      }
      o << "]";
      
      return o;
    }
      
    /** 
     *
     */
    bool WordRecConfig::operator==( const WordRecConfig & other ) const
    {
      if( word == other.word )
      {
        if( st.size() == other.st.size() )
        {
          for( std::deque<Key>::const_iterator it = st.begin(), oit = other.st.begin();
              it != st.end() && oit != other.st.end(); it++, oit++ )
          {
            if( !(*it == *oit) )
              return false; //stack symbol difference
          }
          return true;
        }
        else
          return false; //stacks of unequal size
      }
      else
        return false; //words not equal        
    }
    
    /**
     *
     */
    bool WordRecConfig::operator <( const WordRecConfig & other ) const
    {
      //TODO: construct a better metric for this!
      return st.size() < other.st.size();
    }
    
  }
}
#endif