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
    template<typename Client>
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
      State<Client> getCallState();
      
      /**
       *
       */
      void addCallState( State<Client> call );
      
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
        std::deque<State<Client>> st;
        wali::nws::NWS word;
    };
    
    //Constructors and Destructor
    
    template<typename Client>  
    WordRecConfig<Client>::WordRecConfig()
    {
      
    }  
      
    template<typename Client>  
    WordRecConfig<Client>::WordRecConfig( wali::nws::NWS word )
    {
      this->word = word;
    }
    
    template<typename Client>
    WordRecConfig<Client>::WordRecConfig( const WordRecConfig & other )
    {
      word = other.word;
      st = other.st;
    }
    
    template<typename Client>
    WordRecConfig<Client> & WordRecConfig<Client>::operator=( const WordRecConfig & other )
    {
      this->word = other.word;
      this->st = other.st;
      
      return *this;
    }
    
    template<typename Client>
    WordRecConfig<Client>::~WordRecConfig()
    {
      st.clear();
    }
      
    //Accessors
    
    /**
     *
     */
    template<typename Client>
    bool WordRecConfig<Client>::isEmpty()
    {
      return word.isEmpty();
    }
    
    /**
     *
     */
    template<typename Client> 
    bool WordRecConfig<Client>::hasNext()
    {
      return !(word.nextNode() == NULL);
    }
     
    /**
     *
     */
    template<typename Client>
    wali::nws::NWSNode * WordRecConfig<Client>::getNext()
    {
      return word.nextNode();
    }
    
    /**
     *
     */
    template<typename Client> 
    void WordRecConfig<Client>::removeNext()
    {
      word.removeNode();
    }
    
    /**
     *
     */
    template<typename Client> 
    wali::nws::NWSNode * WordRecConfig<Client>::getCall()
    {
      return word.stackTop();
    }
      
    /** 
     *
     */
    template<typename Client> 
    void WordRecConfig<Client>::setWord( wali::nws::NWS word )
    {
      this->word = word;
    }

    /**
     *
     */
    template<typename Client> 
    bool WordRecConfig<Client>::hasCallState()
    {
      return !st.empty();
    }

    /**
     *
     */
    template<typename Client> 
    State<Client> WordRecConfig<Client>::getCallState()
    {
      return st.back();
    }
      
    /**
     *
     */
    template<typename Client> 
    void WordRecConfig<Client>::addCallState( State<Client> call )
    {
      st.push_back(call);
    }
      
    /**
     *
     */
    template<typename Client> 
    void WordRecConfig<Client>::removeCallState()
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
    template<typename Client> 
    std::ostream & WordRecConfig<Client>::print( std::ostream & o ) const
    {
      word.print(o);
      
      o << "[";
      bool first = true;
      for( std::deque<State<Client>>::const_iterator it = st.begin();
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
    template<typename Client> 
    bool WordRecConfig<Client>::operator==( const WordRecConfig & other ) const
    {
      if( word == other.word )
      {
        if( st.size() == other.st.size() )
        {
          for( std::deque<State<Client>>::const_iterator it = st.begin(), oit = other.st.begin();
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
    template<typename Client> 
    bool WordRecConfig<Client>::operator <( const WordRecConfig & other ) const
    {
      //TODO: construct a better metric for this!
      return st.size() < other.st.size();
    }
    
  }
}
#endif