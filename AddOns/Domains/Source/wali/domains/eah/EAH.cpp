/**
 * @author Nicholas Kidd
 * @version $Id: EAH.cpp 585 2009-01-13 19:33:45Z kidd $
 */

/**
 * Transformers implement post so that summarization of paths
 * computes the "eta" tranformer.
  
             eta([]) = (\empytset, \emptyset^k, \empytset, \emptyset^k, I0)
  eta([r1, ..., rn]) = post(eta([r1, ..., r_{n-1}]), Lab(rn)),

where I0 is the set of initially-held locks.

post((R,RH,U,AH,L),a) = (R',RH',U',AH',L'), where
 case a \notin {"(i", ")i"}:
   (R',RH',U',AH',L') = (R,RH,U,AH,L)
 case a == ")i":   // because of well-nestedness, we know that li \in L
   if AH[i] != \emptyset then
     // ")i" must represent a "U" symbol in (U* R)^i U* (A U*)^j
     R' = R
     RH' = RH
     U' = U \cup {li}    // We just added a usage of {li}
     AH'[i] = \emptyset  // Cancel the acquisition history for li
     forall j != i:
       AH'[j] = AH[j]    // The other acquisition histories are not changed
     L' = L \ {li}       // li is no longer held
   else // AH[i] == \emptyset
     // ")i" represents an "R" symbol in (U* R)^i U* (A U*)^j
     R' = R \cup {li}
     RH'[i] = {li} \cup U \cup R  // li is released; members of U are used before;
                                  // members of R are released before
     forall j != i:
       RH'[j] = RH[j]    // The other release histories are not changed
     U' = U
     AH' = AH            // == Empty
     L' = L \ {li}       // li is no longer held
   endif
 case a == "(i":  // because of well-nestedness, we know that li \notin L
   // "(i" must represent an "A" symbol in the current prefix of
   // (U* R)^i U* (A U*)^j; however, "(i" may later be identified to be
   // a "U" symbol as longer prefixes are considered
   R' = R
   RH'= RH
   U' = U
   AH'[i] = {li}         // This is an acquisition of li, followed by nothing (yet)
   forall j != i:
     AH'[j] = (AH[j] == \emptyset)
                   ? \emptyset
                   :  AH[j] \cup {li} // {li} is added to AH[j] because the acquire
                                      //  of li happens after the acquire of lj
   L' = L \cup {li}  // li is now held
   */

/**
 * (1) LOCKS   = number of locks in the program.
 * (2) PLYVARS = number of BDD vars in a ply.
 * (3) NUMVARS = number of used BDD vars (== PLYVARS*3).
 *
 * Each ply contains the following information.
 * [R] : 
 *    set of initially held locks that are released along a path.
 *    Size == |LOCKS|.
 *
 * [RH_1 ... RH_{LOCKS}] : 
 *    Relase histories for each lock initially held and released.
 *    Size(RH_i) == |LOCKS|.
 *
 * [U] :
 *    Set of locks used along a path. 
 *    Are the "U" symbols in the pattern (U* R)^i U* (A U*)^j
 *    Size == |LOCKS|.
 *
 * [AH_1 ... AH_{LOCKS}] :
 *    Acqusition histories for each held lock.
 *    Size(AH_i) == |LOCKS|.
 *
 * [L]
 *    Set of locks held at end of the path.
 *    Size == |LOCKS|
 *
 * Thus, PLYVARS =
 *   3 * LOCKS ([R], [U], and [L])        +
 *   2 * (LOCKS * LOCKS) [RH and AH]      
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

#include "wali/domains/eah/EAH.hpp"

using namespace wali::domains::eah;
using wali::waliErr;
using std::cout;
using std::endl;

const int EAH::MAX_LOCKS = 4;
int EAH::BASE            = -1;
int EAH::LOCKS           = -1;
int EAH::PLYVARS         = -1;
int EAH::NUMVARS         = -1;
bddPair* EAH::shiftright = 0;
bddPair* EAH::shiftleft  = 0;
bddPair* EAH::shift32    = 0;
bdd*  EAH::pply1         = 0;
bdd*  EAH::pply2         = 0;
bdd*  EAH::pply3         = 0;
bdd*  EAH::R_acquires    = 0;
bdd*  EAH::R_releases    = 0;
bdd*  EAH::R_checker     = 0;


bool EAH::is_initialized()
{
  return (LOCKS != -1);
}

static void my_error_handler(int errcode)
{

  *wali::waliErr << "[BuDDy ERROR] " << bdd_errstring(errcode) << std::endl;
  throw errcode;
}

bool EAH::allocate( int num_locks )
{
  if (is_initialized())
  {
    *waliErr << "[WARNING] EAH is already initialized." << endl;
    return false;
  }
  if (num_locks < 0)
  {
    *waliErr << "[ERROR] Must be a positive number of locks." << endl;
    assert(0);
  }
  else if (num_locks > MAX_LOCKS)
  {
    *waliErr << "[ERROR] Can only have " << MAX_LOCKS << " locks." << endl;
    assert(0);
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
    bdd_error_hook( my_error_handler );
  }
  else {
    *waliErr << "[WARNING] BuDDy already initialized." << endl;
  }

  // End initialize BuDDy
  // ///////////////////////

  LOCKS    = num_locks;                 // Set number of locks;
  PLYVARS  = 3*LOCKS + 2*(LOCKS*LOCKS); // @see description above
  NUMVARS  = PLYVARS * 3;               // Total number of vars
  cout << "[INFO] EAH.        " << LOCKS << " lock(s);";
  cout << "  #NUMVARS = " << NUMVARS << ";";
  cout << "  #PLYVARS = " << PLYVARS << endl;
  BASE     = bdd_extvarnum(NUMVARS);
  return true;
}

bool EAH::init_vars()
{

  const int Ply1   = BASE;
  const int Ply2   = BASE + PLYVARS;
  const int Ply3   = BASE + (2*PLYVARS);
  // ////////////////////////
  // Creat shiftright/shiftleft pairs
  {
    shiftright = bdd_newpair();
    shiftleft  = bdd_newpair();
    for (int i=0; i < PLYVARS; i++)
    {
      // PLY_1 -> PLY_2
      bdd_setpair( shiftright , Ply1 + i , Ply2 + i);
      // PLY_2 -> PLY_3
      bdd_setpair( shiftright , Ply2 + i , Ply3 + i );

      // PLY_2 -> PLY_1
      bdd_setpair( shiftleft  , Ply2 + i , Ply1 + i );
      // PLY_3 -> PLY_2
      bdd_setpair( shiftleft  , Ply3 + i , Ply2 + i );
    }
  }

  // ////////////////////////
  // Creat shift32 pair
  {
    shift32  = bdd_newpair();
    for (int i=0; i < PLYVARS; i++)
    {
      // PLY_3 -> PLY_2
      bdd_setpair(shift32 , Ply3 + i , Ply2 + i);
    }
  }

  // ////////////////////////
  // create var sets for ply 1-3
  {
    bdd p1 = bddtrue;
    bdd p2 = bddtrue;
    bdd p3 = bddtrue;
    for (int i=0; i < PLYVARS; i++)
    {
      p1 = p1 & bdd_ithvar( Ply1 + i );
      p2 = p2 & bdd_ithvar( Ply2 + i );
      p3 = p3 & bdd_ithvar( Ply3 + i );
    }
    pply1 = new bdd(p1);
    pply2 = new bdd(p2);
    pply3 = new bdd(p3);
  }

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

  // //////////////////////////////////////////
  // create the "checker" BDD 
  {
    bdd tmp_checker = bddfalse;
    for (int i=0; i < LOCKS ; i++)
    {
      // Disjoint sets of currently held locks
      int i_in_L  = vn(0,get_lock_in_L(i));
      int i_in_Lp = vn(1,get_lock_in_L(i));
      tmp_checker = tmp_checker | (bdd_ithvar(i_in_L) & bdd_ithvar(i_in_Lp));

      // For P1, let I1 = { li | li \in L1 & AH[i] = {} }
      // Check that I1 \cap U2 = {}
      // Vice versa for P2/P1. 
      bdd held_by_P1_used_by_P2 = 
        bdd_ithvar(i_in_L) &                      // li \in L1
        bdd_nithvar( vn(0,get_ah_start(i)+i) ) &  // li \notin AH_1[i]
        bdd_ithvar( vn(1,get_lock_in_U(i)) );     // li \in U2

      tmp_checker = tmp_checker | held_by_P1_used_by_P2;

      // Vice versa
      bdd held_by_P2_used_by_P1 =
        bdd_ithvar(i_in_Lp) &                     // li \in L2
        bdd_nithvar( vn(1,get_ah_start(i)+i) ) &  // li \notin AH_2[i]
        bdd_ithvar( vn(0,get_lock_in_U(i)) );     // li \in U1

      tmp_checker = tmp_checker | held_by_P2_used_by_P1;

      for (int j=0; j < LOCKS ; j++)
      {
        // ///////////////////////////
        //  Check AH sets
        // Select lock j in AH_i
        int i_in_Aj = BASE + get_ah_start(i) + j;

        // Select lock i in AH'_j
        int j_in_Api = BASE + (PLYVARS + (get_ah_start(j) + i));
        tmp_checker = tmp_checker | (bdd_ithvar(i_in_Aj) & bdd_ithvar(j_in_Api));

        // ///////////////////////////
        //  Check RH sets
        // Select lock j in RH_i
        int i_in_Rj = BASE + get_rh_start(i) + j;

        // Select lock i in AH'_j
        int j_in_Rpi = BASE + (PLYVARS + (get_rh_start(j) + i));
        tmp_checker = tmp_checker | (bdd_ithvar(i_in_Rj) & bdd_ithvar(j_in_Rpi));
      }
    }
    R_checker = new bdd(bdd_not(tmp_checker));
    //printEAH(*R_checker, "Checker");
  }
  return true;
}

bool EAH::initialize( int num_locks )
{
  assert( EAH::allocate(num_locks) );
  assert( EAH::init_vars() );
  return true;
}

EAH EAH::Empty()
{
  static bdd R = empty();
  return EAH( R );
}

EAH EAH::Id()
{
  static bdd R = identity();
  return EAH(R);
}

EAH EAH::Null()
{
  return EAH(bddfalse);
}

EAH EAH::Acquire(int lock)
{
  assert(check_lock(lock));
  return EAH(R_acquires[lock]);
}

EAH EAH::Release(int lock)
{
  assert(check_lock(lock));
  return EAH(R_releases[lock]);
}

EAH::EAH( const EAH& that )
  : R(that.R)
{
}

EAH::~EAH()
{
}

std::ostream& EAH::print( std::ostream& o ) const
{
  printEAH(o,R);
  o << endl;
  return o;
}

std::ostream& EAH::prettyPrint(std::ostream& o, const std::string& name, bool subID) const
{
  // Header
  std::stringstream ss;
  ss << "[" << name;
  o << std::left << std::setw(20) << ss.str() << "] : \n";
  printEAH(o,R,subID);
  o << endl;
  return o;
}

bool EAH::is_equal( const EAH& that) const
{
  return (R == that.R);
}

bool EAH::is_id() const
{
  return is_equal(Id());
}

bool EAH::is_empty() const
{
  return is_equal(Empty());
}

bool EAH::is_null() const
{
  return is_equal(Null());
}

/** @return composition of [this] and [that] */
EAH EAH::Compose( const EAH& that ) const
{
  bdd X = compose(R,that.R);
  return EAH(X);
}

/** @return intersection of [this] and [that] */
EAH EAH::Intersect( const EAH& that ) const
{
  return EAH( (R & that.R) );
}

EAH EAH::Transition() const
{
  static EAH T(transition());
  EAH ID = Id();
  // [this] = (R,RH,U,AH,L) --> (R',RH',U',AH',L')
  // Want   : (R',RH',U',AH',L') --> (0,0^k,0,0^k,L')

  // (1) Quantify out ply 1
  bdd exist_ply1 = bdd_exist(R, *pply1);
  // (2) Shift to the left
  bdd shift = bdd_replace( exist_ply1,shiftleft);
  // (3) Create (R',RH',U',AH',L') -> (R',RH',U',AH',L')
  EAH matched = exist_ply1 & shift & ID.R;
  // (4) Compose with T
  return (matched * T);

}

/** @return union of [this] and [that] */
EAH EAH::Union( const EAH& that ) const
{
  return EAH( (R | that.R) );
}

/** @return true if Compatible([this],[that]) */
bool EAH::Compatible( const EAH& that )
{
  EAH path1 = Empty().Compose(*this);
  //path1.prettyPrint(std::cout,"Path 1");

  EAH path2 = Empty().Compose(that);
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
  EAH quantify_ply_1_for_path1 = bdd_exist( path1.R , *pply1 );
  EAH quantify_ply_1_for_path2 = bdd_exist( path2.R , *pply1 );

  EAH path1_shiftleft = bdd_replace(quantify_ply_1_for_path1.R,shiftleft);
  //path1_shiftleft.prettyPrint(std::cout,"SHIFT< Path 1");

  EAH p1_then_p2 = path1_shiftleft & quantify_ply_1_for_path2;
  //p1_then_p2.prettyPrint(std::cout,"P1 :> P2");

  EAH step2 = p1_then_p2 & EAH(*R_checker);
  //step2.prettyPrint(std::cout, "Step 2");
  return (step2.R != bddfalse);
}

EAH EAH::operator|(const EAH& that) const
{
  return this->Union(that);
}

EAH EAH::operator*(const EAH& that) const
{
  return this->Compose(that);
}

EAH EAH::operator&(const EAH& that) const
{
  return this->Intersect(that);
}

bool EAH::operator==(const EAH& that) const
{
  return R == that.R;
}

EAH::EAH( bdd R ) : R(R)
{
}

bool EAH::check_lock(int lock)
{
  if (!is_initialized())
  {
    *waliErr << "[ERROR] EAH not initialized" << endl;
    return false;
  }
  else if ((lock < 0) || (lock >= LOCKS))
  {
    *waliErr << "[ERROR] Invalid lock " << lock << endl;
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& o, const EAH& eah)
{
  eah.print(o);
  return o;
}

int EAH::vn(int ply, int v)
{
  assert((0<=ply) && (ply < 3));
  assert((0<=v)   && (v < PLYVARS));
  return BASE + (ply * PLYVARS) + v;
}

/** @return the relation {} -> {} */
bdd EAH::empty()
{
  assert( EAH::is_initialized() );
  bdd R = bdd_nithvar(BASE);
  int max = BASE + PLYVARS * 2;
  for (int i = BASE + 1 ; i < max ; i++)
    R = R & bdd_nithvar(i);
  return R;
}

/** @return the relation forall S -> S. I.e., \lam S.S */
bdd EAH::identity()
{
  assert( EAH::is_initialized() );
  bdd R = bddtrue;
  for (int v=0; v < PLYVARS; v++)
  {
    R = R & 
      bdd_biimp(
          bdd_ithvar( vn(0,v) ),
          bdd_ithvar( vn(1,v) ) );
  }
  return R;
}

/** @return relation (R,RH,U,AH,L) -> (\empty,\empty^k,\empty,\empty^k,L) */
bdd EAH::transition()
{
  assert( EAH::is_initialized() );
  EAH ID = Id();
  bdd R = ID.R;
  // Unset all vars except the last set of locks L
  for (int v = 0 ; v < PLYVARS-LOCKS ; v++)
  {
    R = unset( R , 1 , v );
  }
#if 0
  // This is a slower but more methodical way of doing the above
  for (int li = 0; li < LOCKS ; li++)
  {
    // (1) l \notin R'
    bdd li_notin_R = unset( ID.R , 1 , get_l_in_R(li) );
    R = compose(R,li_notin_R);
    // (2) l \notin U'
    bdd li_notinU = unset( ID.R , 1 , get_l_in_U(li) );
    R = compose(R,li_notinU);
    // (3) AH'[i] = \emptyset
    //     RH'[i] = \emptyset
    for (int lj=0 ; lj < LOCKS ; lj++)
    {
      bdd unset_lj_in_AHi = unset( ID.R , 1 , get_ah_start(li)+lj );
      R = compose( R , unset_lj_in_AHi );
      bdd unset_lj_in_RHi = unset( ID.R, 1 , get_rh_start(li)+lj );
      R = compose( R , unset_lj_in_RHi );
    }
  }
#endif
  return R;
}

/* Return relation R[true/(b \in ply)] */
bdd EAH::set(bdd R, int ply, int b)
{
  int var = vn(ply,b);
  bdd x = bdd_ithvar(var);
  return bdd_exist(R,x) & x;
}

/* Return relation R[false/(b \in ply)] */
bdd EAH::unset(bdd R, int ply, int b)
{
  int var = vn(ply,b);
  return bdd_exist(R,bdd_ithvar(var)) & bdd_nithvar(var);
}

/* Relation composition of R1 and R2 */
bdd EAH::compose(bdd R1, bdd R2)
{
  bdd p2   = *pply2;
  bdd sh23 = bdd_replace( R2 , shiftright );
  bdd tmp  = bdd_relprod( R1 , sh23 , p2);
  return bdd_replace( tmp , shift32 );
}

// A PLY is I : RH_1...RH_{LOCKS} : L : AH_0 ... AH_{LOCKS}
int EAH::get_rh_start(int lock)
{
  // PLY is (R,RH,U,AH,L)
  return (LOCKS       + // skip R
      (LOCKS * lock));  // Get start of RH[lock]
}

// A PLY is I : RH_1...RH_{LOCKS} : L : AH_0 ... AH_{LOCKS}
int EAH::get_ah_start(int lock)
{
  // PLY is (R,RH,U,AH,L)
  return (LOCKS       + // skip R
      (LOCKS * LOCKS) + // skip RH
      LOCKS           + // skip U
      (LOCKS * lock));  // Get start of AH[lock]
}

int EAH::get_lock_in_L( int lock )
{
  // Skip over I and RH_is, and add lock number
  // PLY is (R,RH,U,AH,L)
  return PLYVARS - LOCKS + lock;
}

int EAH::get_lock_in_R( int lock )
{
  return lock;
}

int EAH::get_lock_in_U( int lock )
{
  return (LOCKS       + // skip over R
      (LOCKS * LOCKS) + // skip over RH
      lock);
}

bdd EAH::acquire_lock( int lock )
{
  assert(check_lock(lock));
  EAH ID = Id();

  // (1) li \notin L
  bdd notinL = ID.R & bdd_nithvar( vn(0,get_lock_in_L(lock)));

  // (2) L' = L \cup {lock}
  bdd set_lock = set( notinL , 1 , get_lock_in_L(lock) );

  // (3) l \in L' --> [lock] \in AH_l
  bdd R = set_lock;
  for (int l = 0 ; l < LOCKS ; l++)
  {
    bdd if_l_in_L = bdd_ithvar( vn( 0 , get_lock_in_L(l) ) );

    bdd lock_in_AH_l = set( ID.R , 1 , get_ah_start(l)+lock );

    bdd add_lock_to_AH_l = 
      bdd_ite( 
          if_l_in_L , 
          lock_in_AH_l , 
          ID.R);
    R = compose( R , add_lock_to_AH_l );
  }
  return R;
}

bdd EAH::release_lock( int li )
{
  assert(check_lock(li));
  EAH ID = Id();
  // case 1: li \in AH[i]
  //   R'        = R
  //   RH'       = RH
  //   U'        = U \cup {li}
  //   AH'[lock] = \emptyset
  //   forall j != i : 
  //      AH'[j] = AH[j]
  //   L' = L \ {li}
  //
  // (1) li \in AH[i]
  bdd inAHi = ID.R & bdd_ithvar( vn(0,get_ah_start(li)+li));
  // (2) U' = U \cup {li}
  bdd setU = set( inAHi, 1 , get_lock_in_U(li) );
  // (3) AH[i] == \emptyset
  bdd T = setU;
  for (int l=0 ; l < LOCKS ; l++)
  {
    bdd unset_l_in_AHi = unset( ID.R , 1 , get_ah_start(li)+l );

    T = compose( T , unset_l_in_AHi );
  }
  // (4) L' = L \ {li}
  bdd unset_li_in_L = unset( ID.R , 1 , get_lock_in_L(li) );
  T = compose( T , unset_li_in_L );

  // case 2: li \notin AH[i]
  //  R' = R \cup {li}
  //  RH'[i] = {li} \cup U \cup R
  //  forall j != i :
  //    RH'[j] = RH[j]
  //  U' = U
  //  AH' = AH
  //  L' = L \ {li}
  //
  // (1) li \notin AH[i]
  bdd notinAHi = ID.R & bdd_nithvar( vn(0,get_ah_start(li)+li));
  // (2) R' = R \cup {li}
  bdd set_li_in_R = set(notinAHi, 1, get_lock_in_R(li));
  // (3) RH'[i] = {li} \cup U \cup R
  bdd r3 = set_li_in_R;
  for (int l = 0 ; l < LOCKS ; l++)
  {
    bdd if_l_in_U = bdd_ithvar( vn( 0 , get_lock_in_U(l) ) );
    bdd if_l_in_R = bdd_ithvar( vn( 0 , get_lock_in_R(l) ) );
    bdd l_in_RHi = set( ID.R , 1 , get_rh_start(li)+l );
    bdd add_l_to_AHi = 
      bdd_ite( 
          if_l_in_U | if_l_in_R, 
          l_in_RHi , 
          ID.R);
    r3 = compose( r3 , add_l_to_AHi );
  }
  // (4) L' = L \ {li}
  unset_li_in_L = unset( ID.R , 1 , get_lock_in_L(li) );
  bdd F = compose( r3 , unset_li_in_L );
  
  // Now do the switch.
  bdd if_li_in_AHi = bdd_ithvar( vn(0,get_ah_start(li)+li));
  bdd R = bdd_ite( if_li_in_AHi, T, F );
  return R;

}

void EAH::printPly(char* v, int size, int ply)
{
  using namespace std;
  std::stringstream ss;
  assert(size == NUMVARS);
  assert((0<=ply) && (ply < 3));
  // for R, RH_1 , ... , RH_{LOCKS} , U , AH_1 , ... , AH_{LOCKS}, L
  int plybegin = BASE + (ply * PLYVARS);
  // A ply has (3+2*|LOCKS|) sets of vars of size |LOCKS|
  for (int i = 0 ; i < (3+(2*LOCKS)) ; i++)
  {
    // for lock l \in [0,LOCKS)
    bool first = true;
    if ( (i == 1) || (i==(LOCKS+2)) )
    {
      ss << "[{";
    }
    else
    {
      ss << "{";
    }
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
    if ( (i == (LOCKS)) || (i == (2*LOCKS+1)) )
    {
      ss << "}] ";
    }
    else
    {
      ss << "} ";
    }
  }
  cout << left << setw(30) << ss.str();
}

void EAH::printHandler(char* v, int size)
{
  cout << "  ";
  printPly(v,size,0);
  cout << "  ==>  ";
  printPly(v,size,1);
  cout << endl;
}

void EAH::printEAH(std::ostream& o , bdd R, bool subID)
{
  EAH x(R);
  if (x.is_id())
    o << "  ID"    << endl;
  else if (x.is_null())
    o << "  NULL"  << endl;
  else if (x.is_empty())
    o << "  EMPTY" << endl;
  else {
    for ( int l = 0 ; l < LOCKS ; l++ )
    {
      if (x.is_equal(Acquire(l)))
      {
        o << "  Acq(" << (char)('a'+l) << ")" << endl;
        return;
      }
      else if (x.is_equal(Release(l)))
      {
        o << "  Rel(" << (char)('a'+l) << ")" << endl;
        return;
      }
    }
    if (subID)
      bdd_allsat((R - identity()),printHandler);
    else
      bdd_allsat(R,printHandler);
  }
}

