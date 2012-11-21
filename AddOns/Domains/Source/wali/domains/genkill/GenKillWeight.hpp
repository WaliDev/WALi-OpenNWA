/* GT_EXTERNAL_LEGEND(2010) */

#ifndef WALI_DOMAINS_GENKILL_GEN_KILL_WEIGHT_INCLUDED
#define WALI_DOMAINS_GENKILL_GEN_KILL_WEIGHT_INCLUDED

#include <iostream>
#include <climits>
#include <cassert>

#include "wali/SemElem.hpp"

namespace wali {
  namespace domains {
    namespace genkill {

      /*!
       * @class GenKillWeight
       *
       * GenKillWeight is a templated class that implements the
       * semiring needed for a gen-kill dataflow-analysis problem. The
       * template parameter must implement the "Set" interface defined below
       *

       class Set {
         public:
           // not needed by GenKillWeight but a good idea
           Set();

           // Only Set constructor GenKillWeight invokes
           Set( const Set& );

           // not needed by GenKillWeight but a good idea
           Set& operator=( const Set& );

           static bool Eq( const Set& x, const Set& y );

           static Set Diff( const Set& x, const Set& y,
           bool normalizing = false );

           static Set Union( const Set& x, const Set& y );

           static Set Intersect( const Set& x, const Set& y );

           // Optional (but recommended) constant generator
           static const Set& UniverseSet(); 

           // Optional (but recommended) constant generator
           static const Set& EmptySet();

           std::ostream& print(std::ostream& o);
       };

       For normal elements, a semiring element is represented by a pair of
       sets (k,g), which have the meaning \x.(x - k) union g.

       Note that if k and g were allowed to be arbitrary sets, it would
       introduce redundant elements into the domain: e.g., ({a,b}, {c,d})
       would have the same meaning as ({a,b,c,d}, {c,d}).  Therefore, there
       is a class invariant that k intersect g = empty, and the operation
       that builds a semiring element performs the normalization (k,g) |->
       (k-g,g).  However, often the universe of the domain is too large for
       k-g to be represented efficiently when k is the universal set; in this
       case, it may be acceptable to allow k and g to intersect iff k is the
       universe.  (This also holds when Set is a cross product of domains and
       k has a universe component.)  To support this, the normalization
       invokes the function Diff with normalizing==true.

       There are three special elements:
         1. zero
         2. one = (emptyset, emptyset) = \x.x
         3. bottom = (emptyset, Univ) = \x.Univ        
       Note that zero is a different element from the element (Univ,
       emptyset)

       The implementation maintains unique representations for zero

      */
      template< typename Set >
      class GenKillWeight
        : public GenKillBase<GenKillWeight<Set>, Set>
      {
      public: // methods

        typedef GenKillBase<GenKillWeight<Set>, Set> BaseClass;
        friend class GenKillBase<GenKillWeight<Set>, Set>; // god c++ is stupid sometimes
       

        wali::sem_elem_t bottom() const { return makeBottom(); }


        // A client uses makeGenKillWeight to create a
        // GenKillWeight instead of calling the constructor directly;
        //
        // makeGenKillWeight normalizes the stored kill and gen sets
        // so that kill intersect gen == emptyset.
        //
        // makeGenKillWeight also maintains unique representatives for
        // the special semiring values one, and bottom.
        //
        static
        ref_ptr<GenKillWeight>
        makeGenKillWeight(const Set& k, const Set& g )
        {
          Set k_normalized = Set::Diff(k, g, true);
          if (shouldMakeBottom(k_normalized, g)) {
            return makeBottom();
          }
          else if (shouldMakeOne(k_normalized, g)) {
            return BaseClass::makeOne();
          }
          else {
            return new GenKillWeight(k_normalized, g);
          }
        }


        static
        ref_ptr<GenKillWeight>
        makeBottom()
        {
          // Uses a method-static variable to avoid problems with
          // static-initialization order
          static GenKillWeight* BOTTOM = 
            new GenKillWeight(Set::EmptySet(), Set::UniverseSet(), 1);
          return BOTTOM;
        }

        bool IsBottom() const
        {
          if(this == makeBottom().get_ptr()) 
            return true;

          assert(!Set::Eq(this->getKill(), Set::EmptySet()) 
                 || !Set::Eq(this->getGen(), Set::UniverseSet()));

          return false;
        }


        //
        // extend
        //
        // Return the extend of x (this) and y.
        // Considering x and y as functions, x extend y = y o x,
        // where (g o f)(v) = g(f(v)).
        //
        // FIXME: const: wali::SemElem::extend is not declared as const
        wali::sem_elem_t
        extend( wali::SemElem* _y )
        {
          // Handle special case for either argument being zero()
          if( this->equal(this->zero()) || _y->equal(this->zero()) ) {
            return this->zero(); // zero extend _y = zero; this extend zero = zero
          }

          if( _y->equal(bottom()) ) {
            return this->bottom();
          }

          return BaseClass::extend(_y);
        }


        // FIXME: const: wali::SemElem::combine is not declared as const
        wali::sem_elem_t
        combine( wali::SemElem* _y )
        {
          // Handle special case for either argument being bottom()
          if( this->equal(bottom()) || _y->equal(bottom()) ) {
            return bottom(); // bottom combine _y = bottom;
          }                  // this combine bottom = bottom 

          return BaseClass::combine(_y);
        }


        wali::sem_elem_t
        quasiOne() const
        {
          return this->one();
        }

        //
        // diff(GenKillWeight* y)
        //
        // Return the difference between x (this) and y.
        //
        // The return value r has two properties:
        // 1. r ]= x,
        //    i.e., isEqual(combine(x,r), x) == true
        // 2. y combine r = y combine a,
        //    i.e., isEqual(combine(y,r), combine(y,a)) == true
        //
        wali::sem_elem_t
        diff( wali::SemElem* _y ) // const
        {
          // Handle special case for either argument being zero()
          if( this->equal(this->zero()) ) {
            return this->zero(); // zero - _y = zero
          }
          if( _y->equal(this->zero()) ) {
            return this; // this - zero = this
          }

          // Handle special case for second argument being bottom()
          if( _y->equal(bottom()) ) {
            return this->zero(); // this - bottom = zero
          }

          const GenKillWeight* y = dynamic_cast<GenKillWeight*>(_y);
          // Both *this and *y are proper (non-zero) values

          Set temp_k( Set::Diff(Set::UniverseSet(),Set::Diff(y->getKill(), this->getKill())) ); 
          Set temp_g( Set::Diff(this->getGen(),y->getGen()) ); 

          return makeGenKillWeight(temp_k, temp_g);
        }


        std::ostream& print( std::ostream& o ) const 
        {
          if(this->IsBottom())
            return o << "<bottom>";

          return BaseClass::print(o);
        }

        static std::ostream& print_static_transformers( std::ostream& o )
        {
          o << "ONE\t=\t";    BaseClass::makeOne()->print(o);  o << std::endl;
          o << "ZERO\t=\t";   BaseClass::makeZero()->print(o); o << std::endl;
          o << "BOTTOM\t=\t"; makeBottom()->print(o); o << std::endl;
          return o;
        }

      private: // methods -----------------------------------------------------------

        // Constructors
        // The constructors are private to ensure uniqueness of one, zero, and bottom

        // Constructor for legitimate values
        GenKillWeight(const Set& k, const Set& g, unsigned int c=0)
          : BaseClass(k, g, c)
        {}

        // Constructor for zero
        GenKillWeight(unsigned int c=0)
          : BaseClass(c)
        {}


        static
        bool
        shouldMakeBottom(Set const & k_normalized, Set const & g)
        {
          return Set::Eq(k_normalized, Set::EmptySet())
            && Set::Eq(g, Set::UniverseSet());
        }

      };



    } // namespace genkill
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
