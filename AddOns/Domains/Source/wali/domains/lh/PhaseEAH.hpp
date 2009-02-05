#ifndef wali_domains_eah_PhaseEAH_GUARD
#define wali_domains_eah_PhaseEAH_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * An Extended Acquisition History is an abstraction of a
 * path in a program. For [k] locks, a path abstraction is a 5-tuple of the form
 * (q,(R,RH,U,AH,L)), where
    o q is a state in the PA
    o R is the set of initially-held locks that are released along the path
    o RH is the release-history map
    o U is the set of locks used as "U" symbols in (U* R)^i U* (A U*)^j)
    o AH is the acquisition-history map
    o L is the set of locks held at the end of the path (including
      initially-held locks that were not released)

 */

#include <iostream>
#include <vector>

#include "bdd.h"

#include "wali/Common.hpp"

namespace wali
{
  namespace domains
  {
    namespace eah
    {
      class PhaseEAH
      {
        public:
          /** Whether to do the integrity checks */
          static bool DO_INV_CHECK;

          /** The maximum number of locks PhaseEAH can handle */
          static const int MAX_LOCKS;

          /** @return number of bdd vars to represent v */
          static int getln( int v );

          /** @return number of locks */
          static int get_lock_count();
          
          /** @return true if PhaseEAH::initialize succeeded */
          static bool is_initialized();
          
          /** 
           * Initialize the PhaseEAH domain with [num_locks] locks.
           * [Q] is the number of states in the phase automaton.
           * @return true on success.
           * @see PhaseEAH::allocate
           * @see PhaseEAH::init_vars
           */
          static bool initialize( int num_locks, int phases, int Q=-1 );

          /** @return {} -> {} */
          static PhaseEAH Empty();

          /** @return \PhaseEAH . PhaseEAH */
          static PhaseEAH Id();

          /** @return \PhaseEAH . false */
          static PhaseEAH Null();

          /** @return \PhaseEAH -> PhaseEAH + [lock] */
          static PhaseEAH Acquire(int lock, int p=0);

          /** @return \PhaseEAH -> PhaseEAH - [lock] */
          static PhaseEAH Release(int lock, int p=0);

          /** @return the Transition PhaseEAH */
          static PhaseEAH Transition(int q1, int q2);

        public:
          PhaseEAH(const PhaseEAH& that);
          virtual ~PhaseEAH();

          std::ostream& prettyPrint( std::ostream& o ) const;
          std::ostream& print( std::ostream& o ) const;

          /** Print only difference with ID */
          std::ostream& prettyPrint(
              std::ostream& o, 
              const std::string& name, 
              bool subID=false) const;

          /** @return [this] == [that]  */
          bool is_equal(const PhaseEAH& that) const;

          /** @return [this] == Id()    */
          bool is_id() const;

          /** @return [this] == Empty() */
          bool is_empty() const;

          /** @return [this] == Null()  */
          bool is_null() const;

          /** @return Union of [this] and [that] */
          PhaseEAH operator|(const PhaseEAH& that) const;

          /** @return Compose of [this] and [that] */
          PhaseEAH operator*(const PhaseEAH& that) const;

          /** @return Intersect of [this] and [that] */
          PhaseEAH operator&(const PhaseEAH& that) const;

          /** @return true if [this] equals [that] */
          bool operator==(const PhaseEAH& that) const;

          /** 
           * @return true if there exists a path {p1 \in t1}
           *  and {p2 \in t2} such that Compatible(p1,p2).
           */
          static bool Compatible( PhaseEAH t1, PhaseEAH t2 );

          /** @return true iff Compatible(p \in [v]) */
          static bool Compatible( std::vector< PhaseEAH >& v );

        private: // Helper methods
          /** Initialize buddy and allocate (phases+2)*D vars */
          static bool allocate( int num_locks, int phases, int Q );

          /** 
           * Initialize the static class vars.
           * Seperated out so that PhaseEAH::BASE could be 
           * adjusted for SlidingPhaseEAH.
           */
          static bool init_vars();

          /** @assert that [lock] is valid */
          static bool check_lock( int lock );

          /** @assert that [q] is valid */
          static bool check_q( int q );

          /** @return index for var [v] in ply [ply] */
          static int vidx_base(int base, int ply, int v);
          static int vidx(int ply, int v);
          static int cidx(int N, int proc , int phase , int v);

          /** @return BDD for var [v] in ply [ply] */
          static bdd var(int ply, int v);

          /** @return BDD for \not var [v] in ply [ply] */
          static bdd nvar(int ply, int v);

          /** @return the relation {} -> {} */
          static bdd empty();

          /** @return the relation forall S -> S. I.e., \lam S.S */
          static bdd identity();

          /** @return relation R[true/(b \in ply)] */
          static bdd set(bdd R, int ply, int b);

          /** @return relation R[false/(b \in ply)] */
          static bdd unset(bdd R, int ply, int b);

          /**
           * @return starting index for the set of vars
           * that denote the RH for the lock [lock].
           */
          static int get_rh_start(int lock);

          /**
           * @return BDD var number for lock in R
           */
          static int get_lock_in_R( int lock );

          /**
           * @return BDD var number for lock in U
           */
          static int get_lock_in_U( int lock );

          /**
           * @return BDD var number for lock in L
           */
          static int get_lock_in_L( int lock );

          /**
           * @return starting index for the set of vars
           * that denote the AH for lock [lock].
           */
          static int get_ah_start(int lock);

          /** @return bdd with bits set to represent q */
          static void print_q( int ply, bdd r);
          static bdd q_in_ply( int q, int ply );

          static void printHandler(char* v, int size);
          static void printPhaseEAH(
              std::ostream& o,
              bdd R,
              bool subID=false);

          /** Verify integrity of PhaseEAH bdd */
          static bool invariant_check( bdd X , int phaseX );

          static void print_r_compat(char* v, int size);

        private: // static vars

          /** The offset where numbering of BDD vars begins */
          static int BASE;

          /** The number of locks in the program */
          static int LOCKS;

          /** The number of phases in the program */
          static int PHASES;

          /** The number of states in the PA */
          static int Q;
          
          /** The number of vars to represent Q */
          static int QVARS;

          /**
           * The domain and range (i.e., a ply) is [L,AH_0,...,AH_{LOCKS-1}].
           * L and each AH_i is of size |LOCKS|.
           */
          static int PLYVARS;

          /** Number of plies to represent a PhaseEAH.
           *  Equal to PHASES + 2
           */
          static int NUMPLYS;

          /** 
           * Relation PLY x PLY x PLY requires more variables.
           * PLY 1 is {from}, PLY 2 is {to}, and PLY 3 is {scratch}.
           * Notation is L and AH_i for PLY 1.
           * Notation is L' and AH'_i for PLY 2.
           */
          static int NUMVARS;
          
          /** shift plys to the right */
          static bddPair* shiftright;

          /** shift plys to the left */
          static bddPair* shiftleft;
          
          /** Point to BDDs that select plys */
          static bdd* ply_selectors;
          
          /** array of bddPairs that shift all vars left starting
           * in ply [ply]. */ 
          static bddPair** left_from_ply;

          /** For printing a PhaseEAH, we have to
           * pass the phase number to a static function */
          static int kludge;

          static int N;

        //private: // instance stuff
        public: // instance stuff
          PhaseEAH( bdd R , int phaseX );
          bdd R;
          int phaseX;

      }; // class PhaseEAH

    } // namespace eah

  } // namespace domains

} // namespace wali


std::ostream& operator<<(std::ostream&, const wali::domains::eah::PhaseEAH& eah);

#endif  // wali_domains_eah_PhaseEAH_GUARD


