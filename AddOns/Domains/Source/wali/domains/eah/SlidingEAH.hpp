#ifndef wali_domains_eah_SLIDINGEAH_GUARD
#define wali_domains_eah_SLIDINGEAH_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * Implements a weight domain for answering
 * phased-bounded queries using extended acquisition histories (EAH).
 * Is in between pairing (PhasedEAH .. bad name) and full tensors.
 *
 * o A phased path abstraction is represented as a relation on EAHs.
 *   Represented as DR (for domain and range).
 *
 * o A path abstraction is a sequence of phased path abstractions.
 *   E.g., for two phases, it would be XYZ, where (XY) is the
 *   phased path abstraction for phase 1, and (YZ) for phase 2.
 *
 * o "Freezing" occurs on a phase change. Thus, if we have 
 *   a path abstraction ABCD, then AB and BC are frozen or captured for that
 *   abstraction, and CD is the current phase being modified.
 *
 * o A path abstraction ABCD can be thought of as being open on both ends.
 *   It represents that starting from A, the program can go to B,
 *   perform a phase change and freeze, same for B to C, and then
 *   C to D is the current operation.
 *
 * o Composing two path abstractions \rho1 and \rho2 
 *   is relational composition for the the end of \rho1 and the beginning
 *   of \rho2.
 *   E.g., \rho1 = _abcS and \rho2 = _xyzS. 
 *      - Both \rho1 and \rho2 have seen one phase change.
 *      - Result must piece them together. Accomplished by
 *        enforcing \rho2 to begin at "c" (via c & x).
 *        This procudes _cy'z'S. Then, quantifiy out the c
 *        to get: 
 *           aby'z'S.
 *        which is a path abstraction that has seen two phase changes.
 *   
 *
 * @see wali::domains::eah::EAH
 */

#include "wali/domains/eah/EAH.hpp"
#include <vector>

namespace wali
{
  namespace domains
  {
    namespace eah
    {
      class SlidingEAH
      {
        public: // static
          static bool DO_INV_CHECK;

          /** @return true if SlidingEAH has been initialized. */
          static bool is_initialized();

          /**
           * Initialize the SlidingEAH domain with 
           * [locks] locks
           * [phases] phases
           * [Q] states in the PA
           */
          static bool initialize( int locks , int phases, int Q );

          /**
           * Empty Phased EAH is a vector of EAH::Empty().
           * @return the Empty Phased EAH 
           */
          static SlidingEAH Empty();

          /** 
           * Identity Phased EAH is a vector of EAH::Id().
           * @return the Identity Phased EAH 
           */
          static SlidingEAH Id();

          /**
           * Null Phased EAH is a vector of EAH::Null().
           * @return the Null Phased EAH 
           */
          static SlidingEAH Null();

          /** 
           * @return SlidingEAH that acquires lock [lock] in
           * phase [phase].
           */
          static SlidingEAH Acquire( int lock , int phase );

          /** 
           * @return SlidingEAH that releases lock [lock] in
           * phase [phase].
           */
          static SlidingEAH Release( int lock , int phase );

          /**
           * @return SlidingEAH that transitions from phase
           * [from] to phase [to]. 
           * Invariant : from == to+1
           */
          static SlidingEAH Transition( int from , int to );

          /** 
           * @return true if there exists a path {p1 \in t1}
           *  and {p2 \in t2} such that Compatible(p1,p2).
           */
          static bool Compatible( SlidingEAH t1, SlidingEAH t2 );

          /**
           * @return true if there exists phased path abstractions \rho_1,...,\rho_n
           *  for processes P_1,...,P_n such that Compatible(\rho_1,...,\rho_n).
           */
          static bool Compatible( std::vector< SlidingEAH >& v );

        public:  // member methods
          SlidingEAH( const SlidingEAH& P );

          virtual ~SlidingEAH();

          /** @return true of [this == that] */
          bool is_equal( const SlidingEAH& that ) const;

          /** @return true if [this == Empty()] */
          bool is_empty() const;

          /** @return true if [this == Id()] */
          bool is_id()    const;

          /** @return true if [this == Null()] */
          bool is_null()  const;

          SlidingEAH operator|( const SlidingEAH& that) const;

          SlidingEAH operator*( const SlidingEAH& that) const;

          SlidingEAH operator&( const SlidingEAH& that) const;

          bool operator==( const SlidingEAH& that) const;

          std::ostream& prettyPrint( std::ostream& o ) const;
          std::ostream& print( std::ostream& o ) const;

        private: // static
          /** Holds number of phases */
          static int PHASES;

          /** Holds number of plys */
          static int NUMPLYS;

          /** bddpair for shifting to the left (right) */
          static bddPair* big_shift_left;
          static bddPair* big_shift_right;

          /** bdd to represent all but last 3 plys */
          static bdd* mask_for_phaseX;

          /** array of bdds that select all vars in a ply */
          static bdd* ply_selectors;

          /** array of bddPair* that shifts left starting at a ply */
          static bddPair** from_ply_shift_pairs;

          /** Kludge for printing so we can record the phase number */
          static int printKludge;

          /** For printing */
          static void printHandler(char* v, int size);

          /**
           * Before checking compatability, must 
           * extend BDD vars. Flag denotes whether
           * this has been done yet.
           */
          static bool is_compat_initialized;

          /**
           * @brief extend the number of vars for doing the tensor.
           * @return the old number of allocated vars
           */
          static bdd compat_init();

          /**
           * @brief verify that plys are empty before fPhase for R.
           */
          static bool invariant_check( bdd X , int phaseX );

        private: // methods
          SlidingEAH( bdd R, int phase );

        private: // member vars
          bdd R;
          /**
           * Number of phase changes that have been observed.
           * If in [0,PHASES), then is a path abstraction.
           *
           * If {fPhase == -1}, then {this} is a phase transition.
           */
          int fPhase;

      }; // class SlidingEAH

    } // namespace wali

  } // namespace domains

} // namespace wali

#endif  // wali_domains_eah_SLIDINGEAH_GUARD

