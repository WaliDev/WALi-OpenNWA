#ifndef wali_domains_lh_LH_GUARD
#define wali_domains_lh_LH_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * A Lock History (LH) is an abstraction of a path in a program. 
 * For 
 *   (1) \PA \A = (Q,Id,\Sigma,\delta), and 
 *   (2) [k] locks
 * an LH is a 5-tuple of the form
 * (q,(R,RH,U,AH,L)), where
    o q is a state in the \PA
    o \R is the set of initially-held locks that are released along the path
    o \RH is the release-history map
    o \U is the set of locks used as "U" symbols in (U* R)^i U* (A U*)^j)
    o \AH is the acquisition-history map
    o \L is the set of locks held at the end of the path 
       - (including initially-held locks that were not released)

 */

#include <iostream>
#include <vector>

#include "buddy/bdd.h"

#include "wali/Common.hpp"

namespace wali
{
  namespace domains
  {
    namespace lh
    {
      class LH
      {
        public:
          friend class PhaseLH;

        public:
          /** The maximum number of locks LH can handle */
          static const int MAX_LOCKS;

          /** @return number of bdd vars to represent v */
          static int getln( int v );

          /** @return number of locks */
          static int get_lock_count();
          
          /** @return true if LH::initialize succeeded */
          static bool is_initialized();
          
          /** 
           * Initialize the LH domain with [num_locks] locks.
           * [Q] is the number of states in the phase automaton.
           * @return true on success.
           * @see LH::allocate
           * @see LH::init_vars
           */
          static bool initialize( int num_locks, int Q );

          /** @return {} -> {} */
          static LH Empty();

          /** @return \LH . LH */
          static LH Id();

          /** @return \LH . false */
          static LH Null();

          /** @return \LH -> LH + [lock] */
          static LH Acquire(int lock);

          /** @return \LH -> LH - [lock] */
          static LH Release(int lock);

          /** @return true if Compatible([a],[b]) */
          static bool Compatible( const LH& a , const LH& b);

          /** @return true if Compatible(v_1,...,v_N) */
          static bool Compatible( const std::vector<LH>& v );

          /** @return Summary LH for LHs [a] and [b] */
          static LH Summarize( const LH& a, const LH& b );

        public:
          LH(const LH& that);
          virtual ~LH();

          std::ostream& print( std::ostream& o ) const;

          /** Print only difference with ID */
          std::ostream& prettyPrint(
              std::ostream& o, 
              const std::string& name, 
              bool subID=false) const;

          /** @return [this] == [that]  */
          bool is_equal(const LH& that) const;

          /** @return [this] == Id()    */
          bool is_id() const;

          /** @return [this] == Empty() */
          bool is_empty() const;

          /** @return [this] == Null()  */
          bool is_null() const;

          /** @return composition of [this] and [that] */
          LH Compose( const LH& that ) const;

          /** @return intersection of [this] and [that] */
          LH Intersect( const LH& that ) const;

          /** @return the Transition LH */
          LH Transition(int q1, int q2) const;

          /** @return union of [this] and [that] */
          LH Union( const LH& that ) const;

          /** @return Union of [this] and [that] */
          LH operator|(const LH& that) const;

          /** @return Compose of [this] and [that] */
          LH operator*(const LH& that) const;

          /** @return Intersect of [this] and [that] */
          LH operator&(const LH& that) const;

          /** @return true if [this] == [that] */
          bool operator==(const LH& that) const;

          /** @return true if [this] != [that] */
          bool operator!=(const LH& that) const;

        private: // Helper methods
          /** Initialize buddy and allocate DxRxS vars */
          static bool allocate( int num_locks, int Q );

          /** 
           * Initialize the static class vars.
           * Seperated out so that LH::BASE could be 
           * adjusted for SlidingLH.
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
          static void printLH(
              std::ostream& o,
              bdd R,
              bool subID=false);

          /** 
           * The relation for Compatible(LH_1,LH_2)
           * is used to compute a yes/no answer, and
           * has no use for the third (scratch) ply.
           * The relation for Summarize further restricts
           * Compatilbe by using the third ply to "summarize"
           * two LHs. The rules are as follows (plies are the indexes):
           *
           *   (1) l \in L1     &  l \notin L2 => l \in L3
           *   (2) l \notin L1  &  l \in L2    => l \in L3
           *   (3) r \in R1     &  r \notin R2 => r \in R3
           *   (4) r \notin R1  &  r \in R2    => r \in R3
           *   (5) l \in U1     |  l \in U2    => l \in U3
           *   (6) i \in AH1[j] | i \in AH2[j] => i \in AH3[j]
           *   (7) i \in RH1[j] | i \in RH2[j] => i \in RH3[j]
           *   (8) j \in AH1[i] & h \in AH2[j] => h \in AH3[i]
           *   (9) j \in RH1[i] & h \in RH2[j] => h \in RH3[i]
           */
          static bdd Summarizer();

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

          /** Defaults to 3. Modified by PhaseLH */
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
          LH( bdd R );
          bdd R;

      }; // class LH

    } // namespace lh

  } // namespace domains

} // namespace wali


std::ostream& operator<<(std::ostream&, const wali::domains::lh::LH& lh);

#endif  // wali_domains_lh_LH_GUARD



