/* GT_EXTERNAL_LEGEND(2010) */

#ifndef GEN_KILL_TRANSFORMER_GUARD
#define GEN_KILL_TRANSFORMER_GUARD 1

#include <iostream>
#include <climits>
#include <cassert>
//#include "common.h"
#include "tsl/analyzer/TSLFiles/COMMON/Utilities/TslCommon.h"
#include "gtr/src/ref_ptr/ref_ptr.hpp"

#include "wali/SemElem.hpp"

using wali::SemElem;
using wali::sem_elem_t;

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

For normal elements, a semiring element is represented by
pair of sets (k,g), which have the meaning \x.(x - k) union g.
However, if k and g were allowed to be arbitrary sets,
it would introduce redundant elements into the domain:
e.g., ({a,b}, {c,d}) would have the same meaning
as ({a,b,c,d}, {c,d}).
Therefore, there is a class invariant that k intersect g = empty,
and the operation that builds a semiring element performs
the normalization (k,g) |-> (k-g,g).

There are three special elements:
1. zero
2. one = (emptyset, emptyset) = \x.x
3. bottom = (emptyset, Univ) = \x.Univ
Note that zero is a different element from the
element (Univ, emptyset)

*/
template< typename Set > class GenKillTransformer_T : public wali::SemElem {
public: // methods

  sem_elem_t one()    const { return MkOne();    }
  sem_elem_t zero()   const { return MkZero();   }
  sem_elem_t bottom() const { return MkBottom(); }

  // A client uses makeGenKillTransformer_T to create a
  // GenKillTransformer_T instead of calling the constructor directly;
  // makeGenKillTransformer_T normalizes the stored kill and gen sets
  // so that kill intersect gen == emptyset.
  // makeGenKillTransformer_T also maintains unique representatives for the
  // special semiring values one, and bottom.
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
      static GenKillTransformer_T* ONE =
          new GenKillTransformer_T(Set::EmptySet(), Set::EmptySet(), 1);
      return ONE;
  }

  bool IsOne() const
  {
    if(this == MkOne().get_ptr()) 
      return true;

    TslCommon::shouldNeverHappen( Set::Eq(kill, Set::EmptySet()) 
                                    && Set::Eq(gen, Set::EmptySet()) );

    return false;
  }

  // Zero is a special value that doesn't map to any gen/kill pair,
  // so all we really want out of this is a unique representative.
  // The gen/kill sets with which it is initialized are arbitrary.
  static sem_elem_t MkZero()
  {
      // Uses a method-static variable to avoid
      // problems with static-initialization order
      static GenKillTransformer_T* ZERO =
          new GenKillTransformer_T(1);
      return ZERO;
  }

  bool IsZero() const { return is_zero; }

  static sem_elem_t MkBottom()
  {
      // Uses a method-static variable to avoid
      // problems with static-initialization order
      static GenKillTransformer_T* BOTTOM = 
          new GenKillTransformer_T(Set::EmptySet(), Set::UniverseSet(), 1);
      return BOTTOM;
  }

  bool IsBottom() const
  {
    if(this == MkBottom().get_ptr()) 
      return true;

    TslCommon::shouldNeverHappen( Set::Eq(kill, Set::EmptySet()) 
                                    && Set::Eq(gen, Set::UniverseSet()));

    return false;
  }


  //
  // extend
  //
  // Return the extend of x (this) and y.
  // Considering x and y as functions, x extend y = y o x,
  // where (g o f)(v) = g(f(v)).
  //
  sem_elem_t extend( SemElem* _y ) // FIXME: const: wali::SemElem::extend is not declared as const
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
          return _y;
      }
      else if( _y->equal(one()) )
      {
          if(this->equal(one())) {
              return one();
          }
          else if(this->equal(bottom())) {
              return bottom();
          }
          else {
              return this;
          }
      }

      const GenKillTransformer_T* y = dynamic_cast<GenKillTransformer_T*>(_y);

      Set temp_k( Set::Union( kill, y->kill ) );
      Set temp_g( Set::Union( Set::Diff(gen,y->kill), y->gen) );

      return makeGenKillTransformer_T( temp_k,temp_g );
  }

  sem_elem_t combine( SemElem* _y ) // FIXME: const: wali::SemElem::combine is not declared as const
  {
      // Handle special case for either argument being zero()
      if( this->equal(zero()) ) {
          return _y;
      }
      else if( _y->equal(zero()) ) 
      {
          if(this->equal(one())) {
              return one();
          }
          else if(this->equal(bottom())) {
              return bottom();
          }
          else {
              return this;
          }
      }

      const GenKillTransformer_T* y = dynamic_cast<GenKillTransformer_T*>(_y);

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
  sem_elem_t diff( SemElem* _y ) const
  {
      // Handle special case for either argument being zero
      if( this->equal(zero()) ) {
          return zero();
      }
      else if( _y->equal(zero()) ) 
      {
          if(this->equal(one())) {
              return one();
          }
          else if(this->equal(bottom())) {
              return bottom();
          }
          else {
              return this;
          }
      }

      const GenKillTransformer_T* y = dynamic_cast<GenKillTransformer_T*>(_y);
      // Both *this and *y are proper (non-zero) values

      Set temp_k( Set::Diff(Set::UniverseSet(),Set::Diff(y->kill,kill)) ); 
      Set temp_g( Set::Diff(gen,y->gen) ); 

      return makeGenKillTransformer_T(temp_k, temp_g);
  }

  // Zero is a special representative that must be compared
  // by address rather by its contained Gen/Kill sets.
  bool isEqual(const GenKillTransformer_T* y) const
  {
    // covers comparisons for two zeros, two ones, two bottoms, or invoking isEqual on itself 
    if(this == y) return true;

    // covers cases when one of the params is zero 
    if(this->IsZero() || y->IsZero())
      return false;

    return Set::Eq(kill,y->kill) && Set::Eq(gen,y->gen);
  }

  bool equal(SemElem* _y) const {
    const GenKillTransformer_T* y = dynamic_cast<GenKillTransformer_T*>(_y);
    return this->isEqual(y);
  }

  bool equal(sem_elem_t _y) const {
    const GenKillTransformer_T* y = dynamic_cast<GenKillTransformer_T*>(_y.get_ptr());
    return this->isEqual(y);
  }

  std::ostream& print( std::ostream& o ) const 
  {
    if(this->IsZero())
      return o << "<zero>";
    if(this->IsOne())
      return o << "<one>";
    if(this->IsBottom())
      return o << "<bottom>";

    o << "<\\S.(S - {" << kill << "}) U {" << gen << "}>";
    return o;
  }

  std::ostream& prettyPrint( std::ostream& o ) const {
      return this->print(o);
  }

  //-------------------------------------------------
  // Other useful methods
  //-------------------------------------------------

  Set apply( const Set & input ) const
  {
    TslCommon::shouldNeverHappen( this->IsZero() );
    return Set::Union( Set::Diff(input,kill), gen );
  }

  const Set& getKill() const {
    TslCommon::shouldNeverHappen( this->IsZero() );
    return kill;
  }

  const Set& getGen() const {
    TslCommon::shouldNeverHappen( this->IsZero() );
    return gen;
  }

  static std::ostream& print_static_transformers( std::ostream& o )
  {
      o << "ONE\t=\t";    one()->print(o);  o << std::endl;
      o << "ZERO\t=\t";   zero()->print(o); o << std::endl;
      o << "BOTTOM\t=\t"; bottom()->print(o); o << std::endl;
      return o;
  }

private: // methods -----------------------------------------------------------

  // Constructors
  // The constructors are private to ensure uniqueness of one, zero, and bottom

  // Constructor for legitimate values
  GenKillTransformer_T(const Set& k, const Set& g, unsigned int c=0) :
      wali::SemElem(), kill(k), gen(g), is_zero(false)
  {
      count = c;
#if 0
      std::cerr << "GenKillTransformer_T(" << k << ", " << g << ")" << std::endl;
#endif
  }

  // Constructor for zero
  GenKillTransformer_T(unsigned int c=0) :
      wali::SemElem(), is_zero(true)
  {
      count = c;
#if 0
      std::cerr << "GenKillTransformer_T()" << std::endl;
#endif
      }

private: // members -----------------------------------------------------------
  Set kill, gen;   // Used to represent the function \S.(S - kill) U gen
  bool is_zero;    // True for the zero element, False for all other values

};

template< typename Set >
std::ostream& operator<< (std::ostream& out, const GenKillTransformer_T<Set>& t)
{
  t.print(out);
  return(out);
}

#endif
