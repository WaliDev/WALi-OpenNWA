/* GT_EXTERNAL_LEGEND(2010) */

#ifndef GEN_KILL_TRANSFORMER_GUARD
#define GEN_KILL_TRANSFORMER_GUARD 1

#include <iostream>
#include <climits>
#include <cassert>
//#include "common.h"
#include "gtr/src/ref_ptr/ref_ptr.hpp"

/*!
 * @class GenKillTransformer_T
 *
 * GenKillTransformer_T is a templated class that implements the semiring
 * needed for a gen-kill dataflow-analysis problem. The template
 * parameter must implement the "Set" interface defined below
 *

class Set {

    public:
        // not needed by GenKillTransformer_T but a good idea
        Set();

        // Only Set constructor GenKillTransformer_T invokes
        Set( const Set& );

        // not needed by GenKillTransformer_T but a good idea
        Set& operator=( const Set& );

        static bool Eq( const Set& x, const Set& y );

        static Set Diff( const Set& x, const Set& y );

        static Set Union( const Set& x, const Set& y );

        static Set Intersect( const Set& x, const Set& y );

        // Optional (but recommended) constant generator
        static const Set& UniverseSet(); 

        // Optional (but recommended) constant generator
        static const Set& EmptySet();

        std::ostream& print(std::ostream& o);
};

*/

#include "wali/SemElem.hpp"

using wali::SemElem;
using wali::sem_elem_t;

template< typename Set > class GenKillTransformer_T : public wali::SemElem {
public: // methods

  sem_elem_t one()    const { return MkOne();    }
  sem_elem_t zero()   const { return MkZero();   }
  sem_elem_t bottom() const { return MkBottom(); }

  // A client uses makeGenKillTransformer_T to create a
  // GenKillTransformer_T instead of calling the constructor directly;
  // makeGenKillTransformer_T maintains unique representatives for the
  // special semiring values one and bottom.
  static sem_elem_t makeGenKillTransformer_T(const Set& k, const Set& g )
  {
      Set k_normalized = Set::Diff(k,g); 
      if (Set::Eq(k_normalized, Set::EmptySet())&& 
              Set::Eq(g, Set::UniverseSet()))
      {
          return MkBottom();
      }
      else if (Set::Eq(k_normalized, Set::EmptySet()) && 
              Set::Eq(g, Set::EmptySet()))
      {
          return MkOne();
      }
      else {
          return new GenKillTransformer_T(k_normalized, g);
      }
  }

  ~GenKillTransformer_T() {}

  //-------------------------------------------------
  // Semiring methods
  //-------------------------------------------------
  static sem_elem_t MkOne()
  {
      // Uses a method-static variable to avoid
      // problems with static-initialization order
      //static GenKillTransformer_T* ONE =
      //    new GenKillTransformer_T(Set::UniverseSet(),Set::EmptySet(),1);
      GenKillTransformer_T* ONE =
#ifdef __TSL_DASH
          new GenKillTransformer_T(Set::EmptySet(),Set::EmptySet()); // FIXME
#else
          new GenKillTransformer_T(Set::UniverseSet(),Set::EmptySet());
#endif
      return ONE;
  }

  // Zero is a special value that doesn't map to any gen/kill pair,
  // so all we really want out of this is a unique representative.
  // The gen/kill sets with which it is initialized are arbitrary.
  static sem_elem_t MkZero()
  {
      // Uses a method-static variable to avoid
      // problems with static-initialization order
      //static GenKillTransformer_T* ZERO =
      //    new GenKillTransformer_T(Set::EmptySet(),Set::EmptySet(),1);
      GenKillTransformer_T* ZERO =
#ifdef __TSL_DASH
          new GenKillTransformer_T(Set::UniverseSet(),Set::EmptySet()); // FIXME
#else
          new GenKillTransformer_T(Set::EmptySet(),Set::EmptySet());
#endif
      return ZERO;
  }

  static sem_elem_t MkBottom()
  {
      // Uses a method-static variable to avoid
      // problems with static-initialization order
      //static GenKillTransformer_T* BOTTOM = 
      //    new GenKillTransformer_T(Set::EmptySet(),Set::UniverseSet(),1);
      GenKillTransformer_T* BOTTOM = 
          new GenKillTransformer_T(Set::EmptySet(),Set::UniverseSet());
      return BOTTOM;
  }

  //
  // extend
  //
  // Return the extend of x (this) and y.
  // Considering x and y as functions, x extend y = y o x,
  // where (g o f)(v) = g(f(v)).
  //
  sem_elem_t extend( SemElem* _y )
  {
      // Handle special case for either argument being zero()
      if( this->equal(zero()) ) {
          return zero();
      }
      else if( _y->equal(zero()) ) {
          return zero();
      }
      // Handle special case for either argument being one().
      if( this->equal(one()) ) {
          return _y;//new GenKillTransformer_T(*y);
      }
      else if( _y->equal(one()) ) {
          return new GenKillTransformer_T(*this);
      }

      const GenKillTransformer_T* y = dynamic_cast<const GenKillTransformer_T*>(_y);

      // Handle the general case
      Set temp_k( Set::Union( kill, y->kill ) );
      Set temp_g( Set::Union( Set::Diff(gen,y->kill), y->gen) );
      return makeGenKillTransformer_T( temp_k,temp_g );
  }

  sem_elem_t combine( SemElem* _y )
  {
      // Handle special case for either argument being zero()
      if( this->equal(zero()) ) {
          return _y;
      }
      else if( _y->equal(zero()) ) {
          return new GenKillTransformer_T(*this);
      }

      const GenKillTransformer_T* y = dynamic_cast<const GenKillTransformer_T*>(_y);
      // Handle the general case
      Set temp_k( Set::Intersect( kill, y->kill ) );
      Set temp_g( Set::Union( gen, y->gen ) );

      return makeGenKillTransformer_T( temp_k,temp_g );
  }

  sem_elem_t quasiOne() const
  {
      return one();
  }

  //
  // diff(GenKillTransformer_T* y)
  //
  // Return the difference between x (this) and y.
  //
  // The return value r has two properties:
  // 1. r ]= x,
  //    i.e., isEqual(combine(x,r), x) == true
  // 2. y combine r = y combine a,
  //    i.e., isEqual(combine(y,r), combine(y,a)) == true
  //
  sem_elem_t diff( SemElem* _y )
  {
      // Handle special case for either argument being zero
      if( this->equal(zero()) ) {
          return zero();
      }
      else if( _y->equal(zero()) ) {
          return new GenKillTransformer_T(*this);
      }

      const GenKillTransformer_T* y = dynamic_cast<const GenKillTransformer_T*>(_y);
      // Both *this and *y are proper (non-zero) values

      Set temp_k( Set::Diff(Set::UniverseSet(),Set::Diff(y->kill,kill)) ); 
      Set temp_g( Set::Diff(gen,y->gen) ); 

      // Test for whether zero should be returned,
      // i.e., if *this >= *y.
      if (Set::Eq(temp_k, Set::UniverseSet()) && 
          Set::Eq(temp_g, Set::EmptySet()))
      {
          return zero();
      }

      // Handle the general case
      return makeGenKillTransformer_T(temp_k, temp_g);
  }

  // Zero is a special representative that must be compared
  // by address rather by its contained Gen/Kill sets.
  bool isEqual(const GenKillTransformer_T* y) const
  {
      const GenKillTransformer_T* _zero = dynamic_cast<const GenKillTransformer_T*>(zero().get_ptr());
      const GenKillTransformer_T* _one = dynamic_cast<const GenKillTransformer_T*>(one().get_ptr());

      if ( this == _one && y == _one )
          return true;
      if ( this == _zero && y == _zero )
          return true;

      return Set::Eq(kill,y->kill) && Set::Eq(gen,y->gen);
  }

  bool equal(SemElem* _y) const {
    const GenKillTransformer_T* y = dynamic_cast<const GenKillTransformer_T*>(_y);
    return this->isEqual(y);
  }

  bool equal(sem_elem_t _y) const {
    const GenKillTransformer_T* y = dynamic_cast<const GenKillTransformer_T*>(_y.get_ptr());
    return this->isEqual(y);
  }

  std::ostream& print( std::ostream& o ) const {
      o << "<\\S.(S - {" << kill << "}) U {" << gen << "}>";
      return o;
  }

  std::ostream& prettyPrint( std::ostream& o ) const {
      return this->print(o);
  }

  //-------------------------------------------------
  // Other useful methods
  //-------------------------------------------------

  Set apply( const Set& input ) {
      assert ( !(this == zero()) );
      return Set::Union( Set::Diff(input,kill),gen );
  }

  Set& getKill() {
      return kill;
  }

  const Set& getKill() const {
      return kill;
  }

  Set& getGen() {
      return gen;
  }

  const Set& getGen() const {
      return gen;
  }

  static std::ostream& print_static_transformers( std::ostream& o )
  {
      o << "ONE\t=\t";    one()->print(o);  o << std::endl;
      o << "ZERO\t=\t";   zero()->print(o); o << std::endl;
      o << "BOTTOM\t=\t"; bottom()->print(o); o << std::endl;
      return o;
  }

  std::ostream& print( std::ostream& o )
  {
      o << "GenKillTransformer\n";
      return o;
  }

private: // methods -----------------------------------------------------------

  // Constructor
  // The constructor is private to ensure uniqueness of one, zero, and bottom
  GenKillTransformer_T(const Set& k, const Set& g, unsigned int c=0) :
      kill(k), gen(g), count(c)
  {
#if 0
      std::cerr << "GenKillTransformer_T(" << k << ", " << g << ")" << std::endl;
#endif
  }
  // Constructor: JUNGHEE's addition
  // The constructor is private to ensure uniqueness of one, zero, and bottom
  GenKillTransformer_T(const GenKillTransformer_T& a) :
      kill(a.kill), gen(a.gen), count(0)
  {
#if 0
      std::cerr << "GenKillTransformer_T(" << a.kill << ", " << a.gen << ")" << std::endl;
#endif
  }

public:
GenKillTransformer_T() {}

private: // members -----------------------------------------------------------
  Set kill, gen;   // Used to represent the function \S.(S - kill) U gen

public: // members -----------------------------------------------------------
  RefCounter count;
};

template< typename Set >
std::ostream& operator<< (std::ostream& out, const GenKillTransformer_T<Set>& t)
{
  t.print(out);
  return(out);
}

#endif
