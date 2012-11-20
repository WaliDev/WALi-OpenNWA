/* GT_EXTERNAL_LEGEND(2010) */

#ifndef GEN_KILL_TRANSFORMER_GUARD
#define GEN_KILL_TRANSFORMER_GUARD 1

#include <iostream>
#include <climits>
#include <cassert>

#include "wali/SemElem.hpp"

namespace wali {
  namespace domains {
    namespace genkill {

      /*!
       * @class GenKillWeightNoBottom
       *
       * This is a templated class that implements the semiring needed for a
       * gen-kill dataflow-analysis problem. The template parameter must
       * implement the "Set" interface defined below
       *

       class Set {
         public:
           // Only Set constructor GenKillWeightNoBottom invokes
           Set(Set const &);

           static bool Eq(Set const & x, Set const & y);

           static Set Diff(Set const & x,
                           Set const & y,
                           bool normalizing = false);

           static Set Union(Set const & x, Set const & y);

           static Set Intersect(Set const & x, Set const & y);

           static Set const & EmptySet();

           std::ostream & print(std::ostream & o);
       };

       The above exact signatures are only recommended; for instance, the
       functions could also take non-const references (but why would you do
       that to yourself?) or by value; or they could take other optional
       arguments; Diff could be overloaded instead of using a default
       parameter, etc.

       For normal elements, a semiring element is represented by a pair of
       sets (k,g), which have the meaning \x.(x - k) union g.

       Note that if k and g were allowed to be arbitrary sets, it would
       introduce redundant elements into the domain: e.g., ({a,b}, {c,d})
       would have the same meaning as ({a,b,c,d}, {c,d}).  Therefore,
       GenKillWeightNoBottom maintains an invarient that (k intersect g = empty),
       and the operation that builds a semiring element performs the
       normalization (k, g) |-> (k-g, g). When computing (k-g) for purposes
       of this normalization, the Set::Diff function is called with
       'normalizing' equal to 'true.' (At other times, e.g. in 'extend', this
       parameter is left out.) See GenKillWeight for commentary about why
       this might be useful.

       The special elements are
           1. zero
           2. one = (emptyset, emptyset) = \x.x
       Note that zero is a different element from the element (Univ,
       emptyset); it is not just a transformer with specific gen & kill sets,
       it is a completely special value.

       The implementation maintains unique representations for zero and one.
      */
      template< typename Set >
      class GenKillWeightNoBottom : public wali::SemElem
      {
      public: // methods

        wali::sem_elem_t one()    const { return makeOne();    }
        wali::sem_elem_t zero()   const { return makeZero();   }


        static
        wali::ref_ptr<GenKillWeightNoBottom>
        downcast(wali::sem_elem_t se)
        {
          wali::ref_ptr<GenKillWeightNoBottom> down
            = dynamic_cast<GenKillWeightNoBottom *>(se.get_ptr());
          return down;
        }


        /// A client uses 'make()' to create a GenKillWeightNoBottom instead
        /// of calling the constructor directly.
        ///
        /// 'make()' normalizes the kill and gen sets before storing them so
        /// that (kill intersect gen == emptyset). It also ensures that there
        /// is a unique representitive of semiring 1 (gen = kill = empty).
        static
        wali::ref_ptr<GenKillWeightNoBottom>
        make(Set const & kill, Set const & gen)
        {
          Set k_normalized = Set::Diff(kill, gen, true);
          if (Set::Eq(k_normalized, Set::EmptySet())
              && Set::Eq(gen, Set::EmptySet()))
          {
            return makeOne();
          }
          else {
            return new GenKillWeightNoBottom(k_normalized, gen);
          }
        }

  
        static
        wali::ref_ptr<GenKillWeightNoBottom>
        makeZero()
        {
          // Uses a method-static variable to avoid
          // problems with static-initialization order
          static GenKillWeightNoBottom* the_zero =
            new GenKillWeightNoBottom(1);
          return the_zero;
        }
  

        static
        wali::ref_ptr<GenKillWeightNoBottom>
        makeOne()
        {
          // Uses a method-static variable to avoid problems with
          // static-initialization order
          static GenKillWeightNoBottom* the_one =
            new GenKillWeightNoBottom(Set::EmptySet(), Set::EmptySet(), 1);
          return the_one;
        }
  

        virtual
        ~GenKillWeightNoBottom() {}

        
        bool IsOne() const
        {
          if(this == makeOne().get_ptr()) 
            return true;

          assert(!Set::Eq(kill, Set::EmptySet())
                 || !Set::Eq(gen, Set::EmptySet()));

          return false;
        }

        // Zero is a special value that doesn't map to any gen/kill pair, so
        // all we really want out of this is a unique representative.  The
        // gen/kill sets with which it is initialized are arbitrary.
        bool IsZero() const { return is_zero; }


        //-------------------------------------------------
        // Semiring methods
        //-------------------------------------------------

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
          if( this->equal(zero()) || _y->equal(zero()) ) {
            return zero(); // zero extend _y = zero; this extend zero = zero
          }

          // Handle special case for either argument being one()
          if( this->equal(one()) ) {
            return _y; // one extend _y = _y
          }
          else if( _y->equal(one()) )
          {
            return this; // this extend one = this
          }

          const GenKillWeightNoBottom* y = dynamic_cast<GenKillWeightNoBottom*>(_y);

          Set temp_k( Set::Union( kill, y->kill ) );
          Set temp_g( Set::Union( Set::Diff(gen,y->kill), y->gen) );

          return make( temp_k,temp_g );
        }

        // FIXME: const: wali::SemElem::combine is not declared as const
        wali::sem_elem_t
        combine( wali::SemElem* _y )
        {
          // Handle special case for either argument being zero()
          if( this->equal(zero()) ) {
            return _y; // zero combine _y = _y
          }
          if( _y->equal(zero()) ) {
            return this; // this combine zero = this
          }

          const GenKillWeightNoBottom* y = dynamic_cast<GenKillWeightNoBottom*>(_y);

          Set temp_k( Set::Intersect( kill, y->kill ) );
          Set temp_g( Set::Union( gen, y->gen ) );

          return make( temp_k,temp_g );
        }

        wali::sem_elem_t
        quasiOne() const
        {
          return one();
        }

        //
        // diff(GenKillWeightNoBottom* y)
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
          if( this->equal(zero()) ) {
            return zero(); // zero - _y = zero
          }
          if( _y->equal(zero()) ) {
            return this; // this - zero = this
          }

          const GenKillWeightNoBottom* y = dynamic_cast<GenKillWeightNoBottom*>(_y);
          // Both *this and *y are proper (non-zero) values

          Set temp_k( Set::Diff(Set::UniverseSet(),Set::Diff(y->kill,kill)) ); 
          Set temp_g( Set::Diff(gen,y->gen) ); 

          return make(temp_k, temp_g);
        }

        // Zero is a special representative that must be compared by address
        // rather by its contained Gen/Kill sets.
        bool isEqual(const GenKillWeightNoBottom* y) const
        {
          // Check for identical arguments: could be two special values
          // (i.e., two zeros or two ones) or two identical instances of a
          // non-special semiring value.
          if(this == y) return true;

          // Return false if any argument is zero.  Zero has a unique
          // representative, and thus the return value could only be true via
          // the preceding check for identicalness.  The same approach could
          // be taken for one, but the extra tests are not worth it.
          if(this->IsZero() || y->IsZero())
            return false;

          return Set::Eq(kill,y->kill) && Set::Eq(gen,y->gen);
        }

        bool equal(wali::SemElem* _y) const
        {
          const GenKillWeightNoBottom* y = dynamic_cast<GenKillWeightNoBottom*>(_y);
          return this->isEqual(y);
        }

        bool equal(wali::sem_elem_t _y) const
        {
          const GenKillWeightNoBottom* y = dynamic_cast<GenKillWeightNoBottom*>
            (_y.get_ptr());
          return this->isEqual(y);
        }

        std::ostream& print( std::ostream& o ) const 
        {
          if(this->IsZero())
            return o << "<zero>";
          if(this->IsOne())
            return o << "<one>";

          o << "<\\S.(S - {";
          kill.print(o);
          o << "}) U {";
          gen.print(o);
          o << "}>";
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
          assert(!this->IsZero());
          return Set::Union( Set::Diff(input,kill), gen );
        }

        const Set& getKill() const {
          assert(!this->IsZero());
          return kill;
        }

        const Set& getGen() const {
          assert(!this->IsZero());
          return gen;
        }

        static std::ostream& print_static_transformers( std::ostream& o )
        {
          o << "ONE\t=\t";    one()->print(o);  o << std::endl;
          o << "ZERO\t=\t";   zero()->print(o); o << std::endl;
          return o;
        }

      private: // methods -----------------------------------------------------------

        // Constructors
        // The constructors are private to ensure uniqueness of one, zero, and bottom

        // Constructor for legitimate values
        GenKillWeightNoBottom(const Set& k, const Set& g, unsigned int c=0) :
          wali::SemElem(), kill(k), gen(g), is_zero(false)
        {
          count = c;
        }

        // Constructor for zero
        GenKillWeightNoBottom(unsigned int c=0) :
          wali::SemElem(), is_zero(true)
        {
          count = c;
        }

      private: // members -----------------------------------------------------------
        Set kill, gen;   // Used to represent the function \S.(S - kill) U gen
        bool is_zero;    // True for the zero element, False for all other values

      };

      template< typename Set >
      std::ostream &
      operator<< (std::ostream& out, const GenKillWeightNoBottom<Set> & t)
      {
        t.print(out);
        return out;
      }


    } // namespace genkill
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
