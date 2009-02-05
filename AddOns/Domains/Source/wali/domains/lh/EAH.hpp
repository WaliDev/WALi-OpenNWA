#ifndef wali_domains_eah_EAH_GUARD
#define wali_domains_eah_EAH_GUARD 1

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

#include "bdd.h"
#include <iostream>
#include "wali/Common.hpp"

namespace wali
{
  namespace domains
  {
    namespace eah
    {
      class EAH
      {
        public:
          friend class SlidingEAH;
          friend class PhaseEAH;

        public:
          /** The maximum number of locks EAH can handle */
          static const int MAX_LOCKS;

          /** @return number of bdd vars to represent v */
          static int getln( int v );

          /** @return number of locks */
          static int get_lock_count();
          
          /** @return true if EAH::initialize succeeded */
          static bool is_initialized();
          
          /** 
           * Initialize the EAH domain with [num_locks] locks.
           * [Q] is the number of states in the phase automaton.
           * @return true on success.
           * @see EAH::allocate
           * @see EAH::init_vars
           */
          static bool initialize( int num_locks, int Q );

          /** @return {} -> {} */
          static EAH Empty();

          /** @return \EAH . EAH */
          static EAH Id();

          /** @return \EAH . false */
          static EAH Null();

          /** @return \EAH -> EAH + [lock] */
          static EAH Acquire(int lock);

          /** @return \EAH -> EAH - [lock] */
          static EAH Release(int lock);

        public:
          EAH(const EAH& that);
          virtual ~EAH();

          std::ostream& print( std::ostream& o ) const;

          /** Print only difference with ID */
          std::ostream& prettyPrint(
              std::ostream& o, 
              const std::string& name, 
              bool subID=false) const;

          /** @return [this] == [that]  */
          bool is_equal(const EAH& that) const;

          /** @return [this] == Id()    */
          bool is_id() const;

          /** @return [this] == Empty() */
          bool is_empty() const;

          /** @return [this] == Null()  */
          bool is_null() const;

          /** @return composition of [this] and [that] */
          EAH Compose( const EAH& that ) const;

          /** @return intersection of [this] and [that] */
          EAH Intersect( const EAH& that ) const;

          /** @return the Transition EAH */
          EAH Transition(int q1, int q2) const;

          /** @return union of [this] and [that] */
          EAH Union( const EAH& that ) const;

          /** @return true if Compatible([this],[that]) */
          bool Compatible( const EAH& that);

          /** @return Union of [this] and [that] */
          EAH operator|(const EAH& that) const;

          /** @return Compose of [this] and [that] */
          EAH operator*(const EAH& that) const;

          /** @return Intersect of [this] and [that] */
          EAH operator&(const EAH& that) const;

          /** @return true if [this] equals [that] */
          bool operator==(const EAH& that) const;

        private: // Helper methods
          /** Initialize buddy and allocate DxRxS vars */
          static bool allocate( int num_locks, int Q );

          /** 
           * Initialize the static class vars.
           * Seperated out so that EAH::BASE could be 
           * adjusted for SlidingEAH.
           */
          static bool init_vars();

          /** @assert that [lock] is valid */
          static bool check_lock( int lock );

          /** @assert that [q] is valid */
          static bool check_q( int q );

          /** @return index for var [v] in ply [ply] */
          static int vidx_base(int base, int ply, int v);
          static int vidx(int ply, int v);

          /** @return BDD for var [v] in ply [ply] */
          static bdd var(int ply, int v);

          /** @return BDD for \not var [v] in ply [ply] */
          static bdd nvar(int ply, int v);

          /** @return the relation {} -> {} */
          static bdd empty();

          /** @return the relation forall S -> S. I.e., \lam S.S */
          static bdd identity();

          /** @return relation \I E L A -> L \empty L A */
          static bdd transition();

          /** @return relation R[true/(b \in ply)] */
          static bdd set(bdd R, int ply, int b);

          /** @return relation R[false/(b \in ply)] */
          static bdd unset(bdd R, int ply, int b);

          /** @return relational composition of R1 and R2 */
          static bdd compose(bdd R1, bdd R2);

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

          /**
           * @return bdd for acquiring lock [lock].
           */
          static bdd acquire_lock( int lock );

          /**
           * @return bdd for releasing lock [lock].
           */
          static bdd release_lock( int lock );

          static void printPly(char* v, int size, int ply);
          static void printHandler(char* v, int size);
          static void printEAH(
              std::ostream& o,
              bdd R,
              bool subID=false);

        private: // static vars

          /** The offset where numbering of BDD vars begins */
          static int BASE;

          /** The number of locks in the program */
          static int LOCKS;

          /** The number of states in the PA */
          static int Q;
          
          /** The number of vars to represent Q */
          static int QVARS;

          /**
           * The domain and range (i.e., a ply) is [L,AH_0,...,AH_{LOCKS-1}].
           * L and each AH_i is of size |LOCKS|.
           */
          static int PLYVARS;

          /** Defaults to 3. Modified by PhaseEAH */
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

          /** shift ply 3 to ply 2 */
          static bddPair* shift32;

          /** Pointers to BDDs denoting all vars in a ply */
          static bdd* pply1;
          static bdd* pply2;
          static bdd* pply3;

          /** Array of BDDs for acquiring locks */
          static bdd* R_acquires;

          /** Array of BDDs for releasing locks */
          static bdd* R_releases;

          /** Pointer to BDD that checks compatability */
          static bdd* R_checker;

        private: // instance stuff
          EAH( bdd R );
          bdd R;

      }; // class EAH

    } // namespace eah

  } // namespace domains

} // namespace wali


std::ostream& operator<<(std::ostream&, const wali::domains::eah::EAH& eah);

#endif  // wali_domains_eah_EAH_GUARD


