/**
 * @author Nicholas Kidd
 * @version $Id: AH.cpp 533 2008-12-11 21:14:12Z kidd $
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
#include <sstream>
#include <iomanip>
#include <cassert>

#include "wali/domains/lh/AH.hpp"

using namespace wali::domains::lh;
using wali::waliErr;

const int AH::MAX_LOCKS = 3;
int AH::BASE            = -1;
int AH::LOCKS           = -1;
int AH::PLYVARS         = -1;
int AH::NUMVARS         = -1;
bddPair* AH::shiftright = 0;
bddPair* AH::shiftleft  = 0;
bddPair* AH::shift32    = 0;
bdd*  AH::pply1         = 0;
bdd*  AH::pply2         = 0;
bdd*  AH::pply3         = 0;
bdd*  AH::R_ident       = 0;
bdd*  AH::R_empty       = 0;
bdd*  AH::R_acquires    = 0;
bdd*  AH::R_releases    = 0;
bdd*  AH::R_checker     = 0;


bool AH::is_initialized()
{
  return (LOCKS != -1);
}

void AH::initialize( int num_locks )
{
  using std::cout;
  using std::endl;
  if (is_initialized())
  {
    *waliErr << "[WARNING] AH is already initialized." << endl;
    return;
  }
  if (num_locks < 0)
  {
    *waliErr << "[ERROR] Must be a positive number of locks." << endl;
    assert(false);
  }
  else if (num_locks > MAX_LOCKS)
  {
    *waliErr << "[ERROR] Can only have " << MAX_LOCKS << " locks." << endl;
    assert(false);
  }

  // ///////////////////////
  // Begin initialize BuDDy

  if (0 == bdd_isrunning())
  {
    int rc = bdd_init( BDDMEMSIZE,100000 );
    if( rc < 0 ) {
      *waliErr << "[ERROR] " << bdd_errstring(rc) << endl;
      assert( 0 );
    }
    // Default is 50,000 (1 Mb),memory is cheap, so use 100,000
    bdd_setmaxincrease(100000);
  }
  else {
    *waliErr << "[WARNING] BuDDy already initialized." << endl;
  }

  // End initialize BuDDy
  // ///////////////////////

  LOCKS    = num_locks;         // Set number of locks;
  PLYVARS  = (LOCKS+1) * LOCKS; // LOCKS+1 sets of size LOCKS
  NUMVARS  = PLYVARS * 3;       // Total number of vars
  cout << "[INFO] There are " << NUMVARS << " vars for " << LOCKS << " locks." << endl;
  BASE     = bdd_extvarnum(NUMVARS);

  // ////////////////////////
  // Creat shiftright/shiftleft pairs
  {
    shiftright    = bdd_newpair();
    shiftleft     = bdd_newpair();
    int from_base = BASE + PLYVARS;
    int to_base   = BASE + (2*PLYVARS);
    for (int i=0; i < PLYVARS; i++)
    {
      // PLY_1 -> PLY_2
      bdd_setpair( shiftright , i           , from_base+i);
      // PLY_2 -> PLY_3
      bdd_setpair( shiftright , from_base+i , to_base+i );

      // PLY_2 -> PLY_1
      bdd_setpair( shiftleft , from_base+i  , i );
      // PLY_3 -> PLY_2
      bdd_setpair( shiftleft , to_base+i    , from_base+i );
    }
  }

  // ////////////////////////
  // Creat shift32 pair
  {
    shift32       = bdd_newpair();
    int to_base   = BASE + PLYVARS;
    int from_base = BASE + (2*PLYVARS);
    for (int i=0; i < PLYVARS; i++)
    {
      // PLY_3 -> PLY_2
      bdd_setpair(shift32,from_base+i,to_base+i);
    }
  }

  // ////////////////////////
  // create var sets for plys 1-3
  {
    int base1 = BASE + 0;
    int base2 = BASE + PLYVARS;
    int base3 = BASE + PLYVARS*2;
    bdd p1 = bdd_ithvar(base1);
    bdd p2 = bdd_ithvar(base2);
    bdd p3 = bdd_ithvar(base3);
    for (int i=1; i < PLYVARS; i++)
    {
      p1 = p1 & bdd_ithvar( base1 + i );
      p2 = p2 & bdd_ithvar( base2 + i );
      p3 = p3 & bdd_ithvar( base3 + i );
    }
    pply1 = new bdd(p1);
    pply2 = new bdd(p2);
    pply3 = new bdd(p3);
  }

  // /////////////////////////
  // create identity bdd
  R_ident = new bdd(identity());

  // /////////////////////////
  // create empty bdd
  R_empty = new bdd(empty());

  // /////////////////////////
  // create acquisition bdds
  {
    R_acquires = new bdd[LOCKS]();
    for (int l = 0 ; l < LOCKS ; l++)
      R_acquires[l] = acquire_lock(l);
  }

  // /////////////////////////
  // create release bdds
  {
    R_releases = new bdd[LOCKS]();
    for (int l = 0 ; l < LOCKS ; l++)
      R_releases[l] = release_lock(l);
  }

  // /////////////////////////
  // create the "checker" BDD
  {
    bdd tmp_checker = bddfalse;
    for (int i=0; i < LOCKS ; i++)
    {
      for (int j=0; j < LOCKS ; j++)
      {
        // Not performing this check alleviates
        // from testing 
        //    L'_1 \cap L'_2 = \emptyset
        //if (i==j) continue;

        // Select lock j in AH_i
        int i_in_Aj = BASE + (i+1)*LOCKS + j;

        // Select lock i in AH'_j
        int j_in_Api = BASE + PLYVARS + ((j+1)*LOCKS + i);
        bdd test = bdd_ithvar(i_in_Aj) & bdd_ithvar(j_in_Api);
        tmp_checker = tmp_checker | test;
      }
    }
    R_checker = new bdd(bdd_not(tmp_checker));
    //printAH(*R_checker, "Checker");
  }
}

AH AH::Id()
{
  assert(check_lock(0));
  return AH(*R_ident);
}

AH AH::Null()
{
  assert(check_lock(0));
  return AH(bddfalse);
}

AH AH::Empty()
{
  assert(check_lock(0));
  return AH(*R_empty);
}

AH AH::Acquire(int lock)
{
  assert(check_lock(lock));
  return AH(R_acquires[lock]);
}

AH AH::Release(int lock)
{
  assert(check_lock(lock));
  return AH(R_releases[lock]);
}

AH::AH( const AH& that )
  : R(that.R)
{
}

AH::~AH()
{
}

std::ostream& AH::print( std::ostream& o ) const
{
  if (is_id())
    std::cout << "ID";
  else if (is_null())
    std::cout << "NULL";
  else
    bdd_allsat(R,AH::printHandler);
  std::cout << "\n";
  return o;
}

std::ostream& AH::prettyPrint(std::ostream& o, const std::string& name, bool subID) const
{
  if (is_id())
    std::cout << "ID";
  else if (is_null())
    std::cout << "NULL";
  else if (subID)
  {
    bdd tmp = R - Id().R;
    printAH(tmp,name,subID);
  }
  else 
  {
    printAH(R,name,subID);
  }
  std::cout << "\n";
  return o;
}

bool AH::is_equal( const AH& that) const
{
  return (R == that.R);
}

bool AH::is_id() const
{
  return is_equal(Id());
}

bool AH::is_null() const
{
  return is_equal(Null());
}

/** @return composition of [this] and [that] */
AH AH::Compose( const AH& that ) const
{
  bdd X = compose(R,that.R);
  return AH(X);
}

/** @return union of [this] and [that] */
AH AH::Union( const AH& that ) const
{
  return AH(R | that.R);
}

/** @return true if Compatible([a],[b]) */
bool AH::Compatible( const AH& a, const AH& b )
{
  AH path1 = Empty().Compose(a);
  //path1.prettyPrint(std::cout,"Path 1");

  AH path2 = Empty().Compose(b);
  //path2.prettyPrint(std::cout,"Path 2");

  // ///////////////////////////////////
  // Perform test
  //    \exists li,lj . li \in AH_j && lj \in AH_i 
  // 
  // Note that L \cap L' not necessary b/c of Peter's mod to definition of 
  // AH and putting lock [l] in [AH_l].
  //
  // (1) existentially quantify out PLY 1 in path1 and path 2
  // (2) Shift path 1 :: PLY 2 -> PLY1 
  // (3) Logically AND path 1 and path 2
  // (4) Check if solution exists.
  AH quantify_ply_1_for_path1 = bdd_exist( path1.R , *pply1 );
  AH quantify_ply_1_for_path2 = bdd_exist( path2.R , *pply1 );

  AH path1_shiftleft = bdd_replace(quantify_ply_1_for_path1.R,shiftleft);
  //path1_shiftleft.prettyPrint(std::cout,"SHIFT< Path 1");

  AH p1_then_p2 = path1_shiftleft & quantify_ply_1_for_path2;
  //p1_then_p2.prettyPrint(std::cout,"P1 :> P2");

  AH step2 = p1_then_p2 & AH(*R_checker);
  //step2.prettyPrint(std::cout, "Compatible Paths");
  return (step2.R != bddfalse);
}

AH AH::operator|(const AH& that)
{
  return this->Union(that);
}

AH AH::operator*(const AH& that)
{
  return this->Compose(that);
}

AH AH::operator&(const AH& that)
{
  return AH( ( R & that.R) );
}

bool AH::operator==(const AH& that) const
{
  return (R == that.R);
}

AH::AH( bdd R ) : R(R)
{
}

bool AH::check_lock(int lock)
{
  if (!is_initialized())
  {
    *waliErr << "[ERROR] AH not initialized" << std::endl;
    return false;
  }
  else if ((lock < 0) || (lock >= LOCKS))
  {
    *waliErr << "[ERROR] Invalid lock " << lock << std::endl;
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& o, const AH& ah)
{
  ah.print(o);
  return o;
}

int AH::vn(int ply, int l)
{
  assert((0<=ply) && (ply < 3));
  assert((0<=l)   && (l < PLYVARS));
  return BASE + (ply * PLYVARS) + l;
}

/* Return the relation {} -> {} */
bdd AH::empty()
{
  bdd R = bdd_nithvar(BASE);
  int max = BASE + (PLYVARS * 2);
  for (int i = BASE+1 ; i < max ; i++)
    R = R & bdd_nithvar(i);
  return R;
}

/* Return the relation forall S -> S. I.e., \lam S.S */
bdd AH::identity()
{
  int from = BASE;
  int to   = BASE + PLYVARS;
  bdd R = bdd_biimp( bdd_ithvar(from) , bdd_ithvar(to) );
  for (int i=1; i < PLYVARS; i++)
    R = R & 
      bdd_biimp(
          bdd_ithvar(from+i),
          bdd_ithvar(to + i));
  return R;
}

/* Return relation R[true/(b \in ply)] */
bdd AH::set(bdd R, int ply, int b)
{
  int var = vn(ply,b);
  bdd x = bdd_ithvar(var);
  return bdd_exist(R,x) & x;
}

/* Return relation R[false/(b \in ply)] */
bdd AH::unset(bdd R, int ply, int b)
{
  int var = vn(ply,b);
  return bdd_exist(R,bdd_ithvar(var)) & bdd_nithvar(var);
}

/* Relation composition of R1 and R2 */
bdd AH::compose(bdd R1, bdd R2)
{
  bdd p2 = *pply2;
  bdd sh23 = bdd_replace(R2,shiftright);
  //cout << left << setw(20) << "[ >> ]" << sh23 << endl;
  bdd tmp = bdd_relprod(R1,sh23,p2);
  //cout << left << setw(20) << "[R1 x >>]" << bddset << tmp << endl;
  return bdd_replace(tmp,shift32);
}

int AH::get_ah_start(int lock)
{
  return (LOCKS *(lock+1));
}

bdd AH::acquire_lock( int lock )
{
  assert(check_lock(lock));
  AH ID = Id();

  // (1)
  bdd set_lock = set(ID.R,1,lock);
  //printAH(set_lock,"\\L . L+{b}");

  // (2)
  bdd R = set_lock;
  for (int l = 0 ; l < LOCKS ; l++)
  {
    bdd if_l_in_L = bdd_ithvar( vn(0,l) );

    bdd lock_in_AH_l = set( ID.R , 1 , get_ah_start(l)+lock );

    bdd add_lock_to_AH_l = 
      bdd_ite(
          if_l_in_L,
          lock_in_AH_l,
          ID.R );
    R = compose( R , add_lock_to_AH_l );
  }

  return R;
}

bdd AH::release_lock( int lock )
{
  assert(check_lock(lock));
  AH ID = Id();

  // (1)
  bdd unset_lock = unset( ID.R , 1 , lock );
  
  // (2)
  bdd R = unset_lock;
  for (int l=0 ; l < LOCKS ; l++)
  {
    bdd unset_l_in_AH_lock = unset( ID.R , 1 , get_ah_start(lock)+l );
    R = compose( R , unset_l_in_AH_lock );
  }

  return R;
}

void AH::printPly(char* v, int size, int ply)
{
  using namespace std;
  std::stringstream ss;
  assert(size == NUMVARS);
  assert((0<=ply) && (ply < 3));
  // for L,AH_1,...,AH_n
  int plybegin = BASE + (ply * PLYVARS);
  // A ply has |LOCKS+1| sets of vars of size |LOCKS|
  for (int i = 0 ; i <= LOCKS ; i++)
  {
    // for lock l \in [0,LOCKS)
    ss << "{";
    bool first = true;
    for (int l = 0; l < LOCKS ; l++)
    {
      int offset = plybegin + (i*LOCKS + l);
      assert(offset < size);
      if (v[offset]) {
        if (first)
          first = false;
        else
          ss << ", ";
        switch(v[offset]) {
          case -1:
            ss << "*";
          case 1:
            ss << (char)('a'+l);
            break;
          case 0:
            break;
          default:
            assert(0);
        }
      }
    }
    ss << "} ";
  }
  cout << left << setw(30) << ss.str();
}

void AH::printHandler(char* v, int size)
{
  std::cout << "  ";
  printPly(v,size,0);
  std::cout << "  ==>  ";
  printPly(v,size,1);
  std::cout << std::endl;
}

void AH::printAH(bdd R,const std::string& name,bool subID)
{
  std::stringstream ss;
  ss << "[" << name;
  std::cout << std::left << std::setw(20) << ss.str() << "] : \n";
  if (subID)
    bdd_allsat((R - identity()),printHandler);
  else
    bdd_allsat(R,printHandler);
}

