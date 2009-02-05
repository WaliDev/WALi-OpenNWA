#ifndef wali_domains_eah_PHASEDEAH_GUARD
#define wali_domains_eah_PHASEDEAH_GUARD 1

/**
 * @author Nicholas Kidd
 * Phased-Based Model Checking using a paired
 * domain instead of sliding or full tensor.
 */

#include "wali/SemElem.hpp"
#include "wali/domains/eah/EAH.hpp"

#include <vector>

namespace wali
{
  namespace domains
  {
    namespace eah
    {
      class PairedEAH
      {
        public: // static

          /** @return true if PairedEAH has been initialized. */
          static bool is_initialized();

          /**
           * Initialize the PairedEAH domain with [locks] locks
           * and [phases] phases.
           */
          static bool initialize( int locks , int phases );

          /**
           * Empty Phased EAH is a vector of EAH::Empty().
           * @return the Empty Phased EAH 
           */
          static PairedEAH Empty();

          /** 
           * Identity Phased EAH is a vector of EAH::Id().
           * @return the Identity Phased EAH 
           */
          static PairedEAH Id();

          /**
           * Null Phased EAH is a vector of EAH::Null().
           * @return the Null Phased EAH 
           */
          static PairedEAH Null();

          /** 
           * @return PairedEAH that acquires lock [lock] in
           * phase [phase].
           */
          static PairedEAH Acquire( int lock , int phase );

          /** 
           * @return PairedEAH that releases lock [lock] in
           * phase [phase].
           */
          static PairedEAH Release( int lock , int phase );

          /**
           * @return PairedEAH that transitions from phase
           * [from] to phase [to]. 
           * Invariant : from == to+1
           */
          static PairedEAH Transition( int from , int to );

          /** 
           * @return true if there exists a path {p1 \in t1}
           *  and {p2 \in t2} such that Compatible(p1,p2).
           */
          static bool Compatible( PairedEAH t1, PairedEAH t2 );

        public:  // member methods
          PairedEAH( const PairedEAH& P );

          virtual ~PairedEAH();

          /** @return true of [this == that] */
          bool is_equal( const PairedEAH& that ) const;

          /** @return true if [this == Empty()] */
          bool is_empty() const;

          /** @return true if [this == Id()] */
          bool is_id()    const;

          /** @return true if [this == Null()] */
          bool is_null()  const;

          PairedEAH operator|( const PairedEAH& that) const;

          PairedEAH operator*( const PairedEAH& that) const;

          PairedEAH operator&( const PairedEAH& that) const;

          bool operator==( const PairedEAH& that) const;

          std::ostream& print( std::ostream& o ) const;

          std::ostream& prettyPrint( std::ostream& o ) const
          {
            return print(o);
          }

        private: // static
          static int PHASES;

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

        private: // methods
          PairedEAH( 
              const std::vector<EAH>& v , 
              int from = -1,
              int to   = -1);

        private: // member vars
          std::vector<EAH> v;
          /**
           * When from and to are set, then the PhaseEAH denotes
           * some transition in the state space. 
           * Let {P} be the result of composing {P1} and {P2}.
           *
           *   {P.v[to] = P1.v[from] * P2.v[to]}
           *
           * and
           *
           *   {P.v[i] = P1.v[i] * P2.v[i]} : i != to
           *
           * For Acquire and Relase, from == to.
           * For Transition, to = from + 1.
           */
          int from; // Only set by Acquire ,Release , and Transition
          int to  ; // Only set by Acquire ,Release , and Transition

      }; // class PairedEAH

    } // namespace wali

  } // namespace domains

} // namespace wali

#endif  // wali_domains_eah_PHASEDEAH_GUARD


