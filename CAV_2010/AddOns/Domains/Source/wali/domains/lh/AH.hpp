#ifndef wali_domains_lh_AH_GUARD
#define wali_domains_lh_AH_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * An Acquisition History is an abstraction of a
 * path in a program. For [k] locks, it keeps track
 * of the number of locks held at the end of the path,
 * along with, for each held lock [l], the locks
 * that are acquired after [l].
 */

#include "bdd.h"
#include <iostream>
#include "wali/Common.hpp"

namespace wali
{
  namespace domains
  {
    namespace lh
    {
      class AH
      {
        public:
          static const int MAX_LOCKS;
          static int get_lock_count();
          static bool is_initialized();
          static void initialize( int num_locks );
          /** @return \AH . AH */
          static AH Id();

          /** @return \AH . false */
          static AH Null();

          /** @return {} -> {} */
          static AH Empty();
          static AH Acquire(int lock);
          static AH Release(int lock);

          /** @return true if Compatible([a],[b]) */
          static bool Compatible( const AH& a, const AH& b);

        public:
          AH(const AH& that);
          virtual ~AH();

          std::ostream& print( std::ostream& o ) const;

          /** Print only difference with ID */
          std::ostream& prettyPrint(
              std::ostream& o, 
              const std::string& name, 
              bool subID=false) const;

          /** @return true if [this] == [that] */
          bool is_equal(const AH& that) const;

          /** @return true if [this] equals identity relation */
          bool is_id() const;

          /** @return true if [this] equals the Null relation */
          bool is_null() const;

          /** @return composition of [this] and [that] */
          AH Compose( const AH& that ) const;

          /** @return union of [this] and [that] */
          AH Union( const AH& that ) const;

          /** @return Union of [this] and [that] */
          AH operator|(const AH& that);

          /** @return Compose of [this] and [that] */
          AH operator*(const AH& that);

          /** @return Intersect of [this] and [that] */
          AH operator&(const AH& that);

          /** @return true if [this] equals [that] */
          bool operator==(const AH& that) const;

        private: // Helper methods

          /** @assert that [lock] is valid */
          static bool check_lock( int lock );

          /** @return BDD var number for lock [l] in ply [ply] */
          static int vn(int ply, int l);

          /** @return the relation {} -> {} */
          static bdd empty();

          /** @return the relation forall S -> S. I.e., \lam S.S */
          static bdd identity();

          /** @return relation R[true/(b \in ply)] */
          static bdd set(bdd R, int ply, int b);

          /** @return relation R[false/(b \in ply)] */
          static bdd unset(bdd R, int ply, int b);

          /** @return relational composition of R1 and R2 */
          static bdd compose(bdd R1, bdd R2);

          /**
           * @return starting index for the set of vars
           * that denote the AH for lock [lock].
           */
          static int get_ah_start(int lock);

          /**
           * @return bdd for acquiring lock [lock].
           *
           * First, acquire the lock [lock]
           *
           *    bdd set_lock = \ L, AH_0, ..., AH_{LOCKS-1} . L+{lock} , ...
           *
           * Second, for each lock [l \in LOCKS]
           * generate the relation:
           *
           *  \ L , AH_0,...,AH_{LOCKS-1} .
           *    if (l \in L)
           *       L , AH_0 , ... , AH_l + {lock} , ... , AH_{LOCKS-1}
           *       L , AH_0 , ... , AH_{LOCKS-1}
           *
           * Finally, all of these should be composed together.
           */
          static bdd acquire_lock( int lock );

          /**
           * @return bdd for releasing lock [lock].
           *
           * First, release the lock [lock]
           *
           *    bdd unset_lock = \ L, AH_0, ..., AH_{LOCKS-1} . L - {lock} , ...
           *
           * Second, set AH_{lock} = \emptyset
           *
           *  \ L , AH_0,...,AH_{LOCKS-1} .
           *       L , AH_0 , ... , AH_{lock}=\emtyset , ... , AH_{LOCKS-1}
           *
           * Finally, compose the relations.
           */
          static bdd release_lock( int lock );

          static void printPly(char* v, int size, int ply);
          static void printHandler(char* v, int size);
          static void printAH(bdd R,const std::string& name,bool subID=false);

        private: // static vars

          /** The base "offset" where BDD var numbers begin */
          static int BASE;

          /** The number of locks in the program */
          static int LOCKS;

          /**
           * The domain and range (i.e., a ply) is [L,AH_0,...,AH_{LOCKS-1}].
           * L and each AH_i is of size |LOCKS|.
           */
          static int PLYVARS;

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

          /** Pointer to identity bdd */
          static bdd* R_ident;

          /** Pointer to empty bdd */
          static bdd* R_empty;

          /** Array of BDDs for acquiring locks */
          static bdd* R_acquires;

          /** Array of BDDs for releasing locks */
          static bdd* R_releases;

          /** Pointer to BDD that checks compatability */
          static bdd* R_checker;

        private: // instance stuff
          AH( bdd R );
          bdd R;

      }; // class AH

    } // namespace lh

  } // namespace domains

} // namespace wali


std::ostream& operator<<(std::ostream&, const wali::domains::lh::AH& ah);

#endif  // wali_domains_lh_AH_GUARD


