#ifndef wali_nwa_State_GUARD
#define wali_nwa_State_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/nwa/ClientInfo.hpp"

// ::std
#include <utility> 
#include <assert.h>

namespace wali
{
  namespace nwa
  {
    /**
     *
     *  This class is used to denote the states of an NWA.
     *
     */ 
    template<typename T,typename Client = ClientInfo>
    class State : public Printable
    {
      //The types of states that are possible:
      //  Ordinary: most will by of this type
      //  Stuck: represents the stuck state of an automaton
      enum Type { Ordinary, Stuck };

      public:
        typedef ref_ptr<Client> ClientInfoRefPtr;
    
      /**
       *  TODO
       * @brief constructs a stuck state 
       *
       */                                                                                                            
      //static State& stuck() 
      //{                              
      //  static State * stuck = new State();                   
      //  return *stuck;                                          
      //}      
           
      //
      // Methods
      //

      public:
      
      //Constructors and Destructor
      State( );
      State( T name );
      State( const State & other );
      State & operator=( const State & other );
  
      virtual ~State( ) { }


      //Accessors
        
      /**
       *
       * @brief access the stuck state
       *
       * This method returns the stuck state.
       *
       * @return the stuck state
       *
       */
      static State<T,Client> getStuckState( );
      
      /**
       *  
       * @brief test whether this is the stuck state
       * 
       * This method determines whether this is the stuck state.
       *
       * @return true if this is the stuck state, false otherwise
       *
       */
      bool isStuckState( ) const;
      
      /**
       *  TODO: Should this be allowed?
       * @brief access the Key associated with this state
       *
       * This method provides access to the wali::Key associated with this state.
       * Note: This method should never be called on the stuck state.
       *
       * @return the Key associated with this state
       *
       */
      Key getStateKey( ) const;
      
      /** 
       *  TODO: what should stuck return from this?
       * @brief access the name associated with this state
       *
       * This method provides access to the name associated with this state.
       * Note: This method should never be called on the stuck state.
       *
       * @return the name associated with this state
       *
       */
      T getName( ) const;
      
      /**
       * 
       * @brief access the client information associated with this state
       *
       * This method provides access to the client information associated with this state.
       *
       * @return the client information associated with this state
       *
       */
      ClientInfoRefPtr getClientInfo( ) const;

      /**
       * 
       * @brief set the client information associated with this state
       *
       * This method sets the client information associated with this state to the client
       * information provided.
       *
       * @param - c: the desired client information for this state
       *
       */
      void setClientInfo( const ClientInfoRefPtr c );

      /** 
       *  TODO: Why is this method here?  Why is there no corresponding method for final?
       */
      bool isInitial( ) const
      {
        return initial;
      }

      /**
       *  TODO: Why is this method here?  It should at least be protected with only NWA able to call it!
       */
      void setAsInitial( )
      {
        initial = true;
      }

      /** 
       *  TODO: Why is this method here?  It should at least be protected with only NWA able to call it!
       */
      void unsetAsInitial( )
      {
        initial = false;
      }

      //Intersection of states
      /**
       *  TODO: move nodeIntersection back to here!
       * @brief creates the state that is the join of this state with the given state 
       *
       * This method checks that this state and the given state 'other' can be joined and
       * creates the resulting state.  If these two states can be joined, true is returned 
       * and the resulting state is passed back via the address 'result'.  Otherwise, 
       * false is returned and no new state is created.
       * Note: If some state-matching metric other than no states can be 
       *       joined is desired(this should be always!), this method will 
       *       need to be overridden.
       *
       * @param - other: the state to join with this state
       * @param - result: the address to use in passing back the joint state created when
       *                  it is possible to join the two states
       * @result true if the two states can be joined, false otherwise
       *
       */
      /*virtual bool intersect( State * other, State & result ) const
      {      
        //Note: When overriding this method your metric must determine whether the given
        // states are compatible, then create a state and set result to the state just 
        // created if they are compatible.
        
        result = NULL;
        return false;
      }*/
      
      //Utilities
      
      /** 
       * 
       * @brief print the State
       *
       * This method prints out the State to the output stream provided.
       * Note: T must have a print function.
       *
       * @param - o: the output stream to which to print the State
       * @return the output stream to which the State was printed
       *
       */
      std::ostream & print( std::ostream & o ) const;
      
      /** 
       *
       * @brief tests whether this State is equivalent to the State 'other'.
       * 
       * This method tests the equivalence of this State and the State 'other'.
       *
       * @param - other: the State to compare this State to
       * @return true if this State is equivalent to the State 'other', false otherwise
       *
       */
      bool operator==( const State<T,Client> & other ) const;
      
      /**
       *
       * @brief tests the relationship between this State and the State 'other'.
       *
       * This method tests whether this State is 'less than' the State 'other' in some
       * way.  The default is to order the States based on the ordering of their names.  
       * Note: The stuck state is less than all other states.
       *
       * @param - rhs: the State to compare this State to
       * @return true if this State is 'less than' the State 'other', false otherwise
       *
       */
      bool operator<( const State<T,Client> & rhs ) const;

      //
      // Variables
      //
      
      protected:
      
      T name;
      Type stateType;
      Key stateKey;
      ref_ptr<ClientInfo> clientInfo; 
      bool initial;
    };

    //Constructors
    template <typename T,typename Client>
    State<T,Client>::State( )
    {
      //This creates a stuck state.
      name = T();
      stateType = Stuck;
      stateKey = wali::WALI_EPSILON;
      initial = false;
    }

    template <typename T,typename Client>
    State<T,Client>::State( T name )
    {
      this->name = name;
      stateType = Ordinary;
      stateKey = wali::getKey(name);
      initial = false;
    }

    /* TODO */
    template <typename T,typename Client>
    State<T,Client>::State( const State & other )
    {
      //if( other.isStuckState() )
      //  *this = stuck;
      //else
      //{
        name = other.name;
        stateType = other.stateType;
        stateKey = other.stateKey;
        clientInfo = other.clientInfo;
        initial = other.initial;
      //} 
    }

    /* TODO */
    template <typename T,typename Client>
    State<T,Client> & State<T,Client>::operator=( const State & other )
    {
      if (this == &other)     
        return *this;        
      //if( other.isStuckState() )
      //  *this = stuck;
      //else*/
      //{
        name = other.name;
        stateType = other.stateType;
        stateKey = other.stateKey;
        clientInfo = other.clientInfo;
        initial = other.initial;
      //}    
      return *this;
    }
         
    /**
     *  TODO
     * @brief access the stuck state
     *
     * @return the stuck state
     *
     */
    template <typename T,typename Client>
    State<T,Client> State<T,Client>::getStuckState( )
    {
      return State<T,Client>();
      //return stuck;
    }
    
    /**
     *
     * @brief test whether this is the stuck state
     * 
     * @return true if this is the stuck state, false otherwise
     *
     */
    template <typename T,typename Client>
    bool State<T,Client>::isStuckState( ) const
    {
      return stateType == Stuck;
    }
    
    /**
     *
     * @brief access the Key associated with this state
     *
     * @return the Key associated with this state
     *
     */
    template <typename T,typename Client>
    Key State<T,Client>::getStateKey( ) const
    {
      return stateKey;
    }
    
    /** 
     * 
     * @brief access the name associated with this state
     *
     * @return the name associated with this state
     *
     */
    template <typename T,typename Client>
    typename T State<T,Client>::getName( ) const
    {
      assert(stateType != Stuck);
      return name;
    }
    
    /**
     * 
     * @brief access the client information associated with this state
     *
     * @return the client information associated with this state
     *
     */
    template<typename T,typename Client>
    typename State<T,Client>::ClientInfoRefPtr State<T,Client>::getClientInfo( ) const
    {
      return clientInfo;
    }

    /**
     * 
     * @brief set the client information associated with this state
     *
     * @param - c: the desired client information for this state
     *
     */
    template<typename T,typename Client>
    void State<T,Client>::setClientInfo( const ClientInfoRefPtr c )
    {
      clientInfo = c;
    }

    /** 
     *
     * @brief print the State
     *
     * @param - o: the output stream to which to print the State
     * @return the output stream to which the State was printed
     *
     */
    template <typename T,typename Client>
    std::ostream & State<T,Client>::print( std::ostream & o ) const
    {
      if( isStuckState() )
        o << "stuck";
      else
        o << "non-stuck " << name;
      //  name.print(o);  TODO: restore this!
      return o;
    }
    
    /** 
     *
     * @brief tests whether this State is equivalent to the State 'other'.
     * 
     * @param - other: the State to compare this State to
     * @return true if this State is equivalent to the State 'other', false otherwise
     *
     */
    template <typename T,typename Client>
    bool State<T,Client>::operator==( const State<T,Client> & other ) const
    {
      if( isStuckState() )
        return other.isStuckState();
      else if( other.isStuckState() )
        return false;
      else 
        return ( name == other.name );
    }

    /**
     *
     * @brief tests the relationship between this State and the State 'other'.
     *
     * @param - rhs: the State to compare this State to
     * @return true if this State is 'less than' the State 'other', false otherwise
     *
     */
    template <typename T,typename Client>
    bool State<T,Client>::operator<( const State<T,Client> & rhs ) const
    {
      if(operator==(rhs))
        return false;
        
      // invariant: not equal
      
      if( isStuckState() )
        return true;
      else
        return ( name < rhs.name );  
    }
  }
}
#endif