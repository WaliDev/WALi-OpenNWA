/**
 * @author Nicholas Kidd
 * @version $Id: AHval.cpp 465 2008-11-05 20:39:17Z kidd $
 */

#define BDDBIGMEM 0
#if BDDBIGMEM

// For large memory machines (i.e., >= 2GB) allocs ~1500 MB
//#define BDDMEMSIZE 75000000
// 25000000 ~~ 500 MB, change the multiple to get what you need.
#define FIVE_CENT_MB 25000000
#define BDDMEMSIZE (FIVE_CENT_MB*1)

#else
#define BDDMEMSIZE 10000000
#endif


#include <iostream>
#include <cassert>

#include "wali/domains/lh/intra/AHval.hpp"
#include "wali/Common.hpp"

using namespace wali::domains::lh::intra;

int AHval::LOCKCNT         = -1;
int AHval::NUMVARS         = -1;
const int AHval::MAX_LOCKS = 2;

bool AHval::is_initialized()
{
  return (LOCKCNT != -1);
}

void AHval::initialize( int num_locks )
{
  if (num_locks < 0)
  {
    std::cerr << "Must be a positive number of locks." << std::endl;
    assert(0);
  }
  else if (num_locks > MAX_LOCKS)
  {
    std::cerr << "Can only have " << MAX_LOCKS << " locks." << std::endl;
    assert(0);
  }
  else if (LOCKCNT != -1)
  {
    std::cerr << "AHval::initialize can only be invoked once." << std::endl;
    assert(0);
  }

  // ///////////////////////
  // Begin initialize BuDDy

  if (0 == bdd_isrunning())
  {
    int rc = bdd_init( BDDMEMSIZE,100000 );
    if( rc < 0 ) {
      std::cerr << "[ERROR] " << bdd_errstring(rc) << std::endl;
      assert( 0 );
    }
  }
  else {
    std::cerr << "[ERROR] BuDDy already initialized?" << std::endl;
    assert(0);
  }
  // Default is 50,000 (1 Mb),memory is cheap, so use 100,000
  bdd_setmaxincrease(100000);

  // End initialize BuDDy
  // ///////////////////////

  LOCKCNT = num_locks;
  // Need LOCKCNT+1 sets of size LOCKCNT
  NUMVARS   = (LOCKCNT+1) * LOCKCNT;
  std::cout << "[INFO] There are " << NUMVARS << " vars for " << LOCKCNT << " locks." << std::endl;
  bdd_setvarnum(NUMVARS);

}

AHval::AHval()
{
  assert( is_initialized() );
  fVal = bdd_nithvar(0);
  for (int i=1; i < NUMVARS ; i++)
    fVal = fVal & bdd_nithvar(i);
}

AHval::AHval( const AHval& that )
  : fVal(that.fVal)
{
  assert( is_initialized() );
}

AHval::AHval( bdd R ) : fVal(R)
{
  assert( is_initialized() );
}


AHval AHval::acquire( int lock ) const
{
  check_lock(lock);

  //bdd bddlock = fdd_ithvar(0,lock);
  bdd bddlock = bdd_ithvar(lock);
  assert((fVal & bddlock) == bddfalse);

  //bdd R = add_lock_to_set(fVal,0,lock);
  //bdd exout = bdd_ithvar((setnum * LOCKCNT) + lock);
  //return bdd_exist(R,exout) & exout;


  // First, existentially quantify out the lock for
  // the held lockset L
  bdd exout = bdd_ithvar(lock);
  // Second, update the AH for each held lock [l].
  // I.e., AH_l = AH_l \cup {lock}
  for (int l=0; l < LOCKCNT ; l++)
  {
    // Is lock [l] in the set of held locks?
    bdd has_lock_l = fVal & bdd_ithvar(l);
    if ((l == lock) || has_lock_l != bddfalse )
    {
      int AH_l = (l+1)*LOCKCNT;
      exout = exout & bdd_ithvar( AH_l + lock);
    }
  }
  bdd R = bdd_exist(fVal,exout) & exout;
  return AHval(R);
}

AHval AHval::release( int lock ) const
{
  check_lock(lock);

  bdd bddlock = bdd_ithvar(lock);
  assert((fVal & bddlock) != bddfalse);
  // First, remove [lock] from L
  bdd R = bdd_exist(fVal,bdd_ithvar(lock)) & bdd_nithvar(lock);
  //std::cout << "   R1 : " << bddset << R << std::endl;

  // Second, remove AH_{lock}
  int AH_lock = (lock+1) * LOCKCNT;
  bdd exout = bdd_ithvar(AH_lock);
  bdd exin  = bdd_nithvar(AH_lock);
  for (int i=1 ; i < LOCKCNT ; i++) 
  {
    int varnum = AH_lock+i;
    exout = exout & bdd_ithvar(varnum);
    exin = exin & bdd_nithvar(varnum);
  }
  R = bdd_exist(R,exout) & exin;

  return AHval(R);
}

void AHval::printHandler(char* v, int size)
{
  assert(size == NUMVARS);
  // for L,AH_1,...,AH_n
  for (int i = 0 ; i <= LOCKCNT ; i++)
  {
    // for lock l \in [0,LOCKCNT)
    std::cout << "{";
    bool first = true;
    for (int l = 0; l < LOCKCNT ; l++)
    {
      int offset = i*LOCKCNT + l;
      assert(offset < size);
      if (v[offset]) {
        if (first)
          first = false;
        else
          std::cout << ", ";
        std::cout << "l" << l;
      }
    }
    std::cout << "} ";
  }
}

std::ostream& AHval::print( std::ostream& o ) const
{
  bdd_allsat(fVal,printHandler);
  std::cout << "\n";
  return o;
}

void AHval::check_lock(int lock)
{
  if (LOCKCNT == -1)
  {
    std::cerr << "[ERROR] AH not initialized" << std::endl;
    assert(0);
  }
  else if (lock >= MAX_LOCKS)
  {
    std::cerr << "[ERROR] Invalid lock " << lock << std::endl;
    assert(0);
  }
}

bdd AHval::add_lock_to_set(bdd R, int setnum, int lock)
{
  bdd exout = bdd_ithvar((setnum * LOCKCNT) + lock);
  return bdd_exist(R,exout) & exout;
}

std::ostream& operator<<(std::ostream& o, const AHval& ah)
{
  ah.print(o);
  return o;
}

