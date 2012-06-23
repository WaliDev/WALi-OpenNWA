/*!
 * @author Nicholas Kidd
 *
 * @version $Id$
 */

#include "wali/Common.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/Visitor.hpp"
#include "wali/witness/VisitorPrinter.hpp"

#include <typeinfo>
#include <map>

namespace wali
{
  namespace witness
  {
      // struct witness_t
    witness_t::witness_t() : Parent()
    {
    }

    witness_t::witness_t( sem_elem_t se ) : Parent( getWitness(se) )
    {
    }

    witness_t::witness_t( Witness* wit ) : Parent( wit )
    {
    }

    witness_t& witness_t::operator=( sem_elem_t se )
    {
      Parent::operator=( getWitness(se) );
      return *this;
    }

    witness_t& witness_t::operator=( Witness* wit )
    {
      Parent::operator=( wit );
      return *this;
    }

    Witness* witness_t::getWitness( sem_elem_t se )
    {
      Witness* witness = dynamic_cast< Witness* >(se.get_ptr());
      //se->print( std::cerr << "\n\t+++ " ) << std::endl;
      if( NULL == witness ) {
        *waliErr << "[WARNING] witness_t::getWitness - failed downcast.\n";
        *waliErr << "          Param sem_elem_t is not a witness (line 53)." << std::endl;
        *waliErr << "          se == ";
        se->print(*waliErr) << std::endl;
        assert(false);
      }
      return witness;
    }

    int Witness::COUNT = 0;

    Witness::Witness( sem_elem_t set )
      : user_se(set)
      , isEmpty(false)
    {
      COUNT++;
    }

    Witness::Witness( sem_elem_t se, bool ie)
      : user_se(se)
      , isEmpty(ie)
    {
      COUNT++;
    }

    Witness::~Witness()
    { 
      COUNT--;
    }

    // Test if the Witness has user weight ZERO
    bool Witness::isZero()
    {
      return user_se->equal( user_se->zero() );
    }

    // Test if the Witness has user weight ONE
    bool Witness::isOne()
    {
      return user_se->equal( user_se->one() );
    }

    sem_elem_t Witness::one() const
    {
      return new Witness(user_se->one(), true);
    }

    sem_elem_t Witness::zero() const
    {
      sem_elem_t user_zero = user_se->zero();
      
#if defined(WALI_WITNESS_CACHE_ZEROES)
      // Currently Witness will create a new zero every time it is called. If
      // this becomes a problem, here's an alternate implementation that does
      // not. However, it can be a regression if the weights the witnesses
      // are wrapping don't cache their own zeros and ones, or if many many
      // many different kinds of weights are used.

      typedef std::map<sem_elem_t, sem_elem_t> ZeroCache;
      static ZeroCache zero_cache;
     
      ZeroCache::iterator zit = zero_cache.find(user_zero);
      if (zit == zero_cache.end()) {
        zit = zero_cache.insert(std::make_pair(user_zero, new Witness(user_zero, true))).first;
      }
      assert(zit->second.get_ptr() != NULL);
      return zit->second;
#else
      return new Witness(user_zero, true);
#endif
    }

    sem_elem_t Witness::extend( SemElem * se )
    {
      // TODO : make this a debug check
      // and non debug use static_cast
      Witness *that = dynamic_cast< Witness * >(se);
      if( 0 == that )
      {
        *waliErr << "SemElem is \"" << typeid(se).name() << "\"\n";
        se->print( *waliErr << "   That : " ) << std::endl;
        assert( 0 );
      }
      if( isEmpty && isOne() ) {
        return that;
      }
      else if( that->isEmpty && that->isOne() ) {
        return this;
      }
      return new WitnessExtend( user_se->extend(that->user_se), this, that );
    }

    /*
    */
    sem_elem_t Witness::combine( SemElem * se )
    {
      // TODO : make this a debug check
      // and non debug use static_cast
      Witness *that = dynamic_cast< Witness * >(se);
      if( 0 == that )
      {
        *waliErr << "SemElem is \"" << typeid(se).name() << "\"\n";
        assert( 0 );
      }

      if( isZero() ) {
        return that;
      } else if( that->isZero() ) {
        return this;
      }
      else {

        // Do this here as both branches perform the combine
        sem_elem_t combinedUserSe = user_se->combine(that->user_se);
        if( combinedUserSe->equal(that->user_se) ) {
          return that;
        }
        else if( combinedUserSe->equal(this->user_se) ) {
          return this;
        }
        else {

          WitnessCombine * newwc = new WitnessCombine( combinedUserSe );
          newwc->addChild(this);
          newwc->addChild(that);
          return newwc;
        }
      }
    }

    bool Witness::equal( SemElem * se ) const
    {
      // TODO : make this a debug check
      // and non debug use static_cast
      Witness *that = dynamic_cast< Witness * >(se);
      assert( 0 != that );
      return user_se->equal( that->user_se );
    }

    std::ostream& Witness::print( std::ostream& o ) const
    {
      o << "\n";
      //VisitorPrinter vp(o);
      //accept(vp,true);
      //reset_marks();

      prettyPrint(o,0);

      return o;
    }

    void Witness::accept( Visitor& v, bool visitOnce ATTR_UNUSED )
    {
      (void) visitOnce;
      mark();
      //assert(0);
      //*waliErr << "[?ERROR?] Witness::accept called\n";
      v.visit( this );
    }

    // Adds 2 spaces and a vertical bar for each depth
    std::ostream& Witness::prettyPrint( std::ostream& o, size_t depth ) const
    {
      formatDepth( o , depth );
      o << "Witness: ";
      user_se->print(o) << std::endl;
      return o;
    }

    std::ostream& Witness::formatDepth( std::ostream& o , size_t depth ) const
    {
      for( ; depth > 0 ; depth-- )
      {
        o << "  |";
      }
      return o;
    }

    void Witness::reset_marks() const
    {
      unmark();
    }

  }   // namespace witness

}   // namespace wali

/*
 * $Id$
 */

