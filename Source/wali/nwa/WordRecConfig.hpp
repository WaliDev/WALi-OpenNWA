#ifndef wali_nwa_WordRecConfig_GUARD
#define wali_nwa_WordRecConfig_GUARD 1

/**
 * @author Amanda Burton
 */
 
// ::wali
#include "wali/nws/NWS.hpp"
#include "wali/nws/NWSNode.hpp"
#include "wali/nwa/State.hpp"

// std::c++
#include <deque>

namespace wali
{
  namespace nwa
  {
    template<typename StKey>
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
      State<StKey> getCallState();
      
      /**
       *
       */
      void addCallState( State<StKey> call );
      
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
        std::deque<State<StKey>> st;
        wali::nws::NWS word;
    };
    
    //Constructors and Destructor
    
    template<typename StKey>  
    WordRecConfig<StKey>::WordRecConfig()
    {
      
    }  
      
    template<typename StKey>  
    WordRecConfig<StKey>::WordRecConfig( wali::nws::NWS word )
    {
      this->word = word;
    }
    
    template<typename StKey>
    WordRecConfig<StKey>::WordRecConfig( const WordRecConfig & other )
    {
      word = other.word;
      st = other.st;
    }
    
    template<typename StKey>
    WordRecConfig<StKey> & WordRecConfig<StKey>::operator=( const WordRecConfig & other )
    {
      this->word = other.word;
      this->st = other.st;
      
      return *this;
    }
    
    template<typename StKey>
    WordRecConfig<StKey>::~WordRecConfig()
    {
      st.clear();
    }
      
    //Accessors
    
    /**
     *
     */
    template<typename StKey>
    bool WordRecConfig<StKey>::isEmpty()
    {
      return word.isEmpty();
    }
    
    /**
     *
     */
    template<typename StKey> 
    bool WordRecConfig<StKey>::hasNext()
    {
      return !(word.nextNode() == NULL);
    }
     
    /**
     *
     */
    template<typename StKey>
    wali::nws::NWSNode * WordRecConfig<StKey>::getNext()
    {
      return word.nextNode();
    }
    
    /**
     *
     */
    template<typename StKey> 
    void WordRecConfig<StKey>::removeNext()
    {
      word.removeNode();
    }
    
    /**
     *
     */
    template<typename StKey> 
    wali::nws::NWSNode * WordRecConfig<StKey>::getCall()
    {
      return word.stackTop();
    }
      
    /** 
     *
     */
    template<typename StKey> 
    void WordRecConfig<StKey>::setWord( wali::nws::NWS word )
    {
      this->word = word;
    }

    /**
     *
     */
    template<typename StKey> 
    bool WordRecConfig<StKey>::hasCallState()
    {
      return !st.empty();
    }

    /**
     *
     */
    template<typename StKey> 
    State<StKey> WordRecConfig<StKey>::getCallState()
    {
      return st.back();
    }
      
    /**
     *
     */
    template<typename StKey> 
    void WordRecConfig<StKey>::addCallState( State<StKey> call )
    {
      st.push_back(call);
    }
      
    /**
     *
     */
    template<typename StKey> 
    void WordRecConfig<StKey>::removeCallState()
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
    template<typename StKey> 
    std::ostream & WordRecConfig<StKey>::print( std::ostream & o ) const
    {
      word.print(o);
      
      o << "[";
      bool first = true;
      for( std::deque<State>::const_iterator it = st.begin();
            it != st.end(); it++ )
      {
        if( !first )
          o << ", ";
        else
          first = false;
        it->print(o);
      }
      o << "]";
      
      return o;
    }
      
    /** 
     *
     */
    template<typename StKey> 
    bool WordRecConfig<StKey>::operator==( const WordRecConfig & other ) const
    {
      if( word == other.word )
      {
        if( st.size() == other.st.size() )
        {
          for( std::deque<State>::const_iterator it = st.begin(), oit = other.st.begin();
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
    template<typename StKey> 
    bool WordRecConfig<StKey>::operator <( const WordRecConfig & other ) const
    {
      //TODO: construct a better metric for this!
      return st.size() < other.st.size();
    }
    
  }
}
#endif