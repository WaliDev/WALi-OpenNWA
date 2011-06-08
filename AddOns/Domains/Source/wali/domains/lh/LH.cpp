/**
 * @author Nicholas Kidd
 * @version $Id: LH.cpp 611 2009-01-16 06:07:33Z kidd $
 */

/**
 * Transformers implement post so that summarization of paths
 * computes the "eta" tranformer. These have no effect on q.
 * For set of initially-held locks \I, \eta is defined as:
  
            \eta([],\I)   =  (\empytset, \emptyset^k, \empytset, \emptyset^k, \I)
  \eta([r1, ..., rn],\I)  =  post(eta([r1, ..., r_{n-1}],\I), Lab(rn)),


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
 * During a phase change, the q value changes.
 * TODO
 */

/**
 * (1) LOCKS   = number of locks in the program.
 * (2) Q       = number of states in the PA
 * (3) PLYVARS = number of BDD vars in a ply.
 * (4) NUMVARS = number of used BDD vars (== PLYVARS*3).
 *
 * Each ply contains the following information.
 * q   : the current phase
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
 *   getln(Q)                             +
 *   3 * LOCKS ([R], [U], and [L])        +
 *   2 * (LOCKS * LOCKS) [RH and AH]      +
 *   ln(Q)
 */
#define BDDBIGMEM 1
#if BDDBIGMEM

// For large memory machines (i.e., >= 2GB) allocs ~1500 MB
//#define BDDMEMSIZE 75000000
// 25000000 ~~ 500 MB, change the multiple to get what you need.
#define FIVE_CENT_MB 25000000
#define BDDMEMSIZE (FIVE_CENT_MB*1)

#else
#define BDDMEMSIZE 10000000
#endif

#define INTERLEAVE 1

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "wali/domains/lh/LH.hpp"

using namespace wali::domains::lh;
using wali::waliErr;
using std::cout;
using std::endl;

const int LH::MAX_LOCKS = 4;
int LH::BASE            = -1;
int LH::LOCKS           = -1;
int LH::Q               = -1;
int LH::QVARS           = -1;
int LH::PLYVARS         = -1;
int LH::NUMPLYS         = 3;
int LH::NUMVARS         = -1;
bddPair* LH::shiftright = 0;
bddPair* LH::shiftleft  = 0;
bddPair* LH::shift32    = 0;
bdd*  LH::pply1         = 0;
bdd*  LH::pply2         = 0;
bdd*  LH::pply3         = 0;
bdd*  LH::R_acquires    = 0;
bdd*  LH::R_releases    = 0;
bdd*  LH::R_checker     = 0;


int LH::getln(int v)
{
  int acc = 0;
  while( v != 1)
  {
    acc++;
    v = v/2;
  }
  return acc;
}

bool LH::is_initialized()
{
  return (LOCKS != -1);
}

static void my_error_handler(int errcode)
{

  *wali::waliErr << "[BuDDy ERROR] " << bdd_errstring(errcode) << std::endl;
  throw errcode;
}

bool LH::allocate( int num_locks, int theQ )
{
  if (is_initialized())
  {
    *waliErr << "[WARNING] LH is already initialized." << endl;
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

  Q        = theQ;                      // Set Q
  LOCKS    = num_locks;                 // Set number of locks
  QVARS    = getln(Q);                  // Set number of QVars
  PLYVARS  = QVARS + 3*LOCKS + 2*(LOCKS*LOCKS); // @see description above
  NUMVARS  = PLYVARS * NUMPLYS;         // Total number of vars
  cout << "[INFO] LH. " << LOCKS << " lock(s); ";
  cout << "  #Q = " << Q << ";";
  cout << "  #QVARS = " << QVARS << ";";
  cout << "  #NUMVARS = " << NUMVARS << ";";
  cout << "  #PLYVARS = " << PLYVARS << endl;
  BASE     = bdd_extvarnum(NUMVARS);
  return true;
}

bool LH::init_vars()
{

  //const int Ply1   = BASE;
  //const int Ply2   = BASE + PLYVARS;
  //const int Ply3   = BASE + (2*PLYVARS);
  // ////////////////////////
  // Creat shiftright/shiftleft pairs
  {
    shiftright = bdd_newpair();
    shiftleft  = bdd_newpair();
    for (int i=0; i < PLYVARS; i++)
    {
      // PLY_1 -> PLY_2
      bdd_setpair( shiftright , vidx(0,i) , vidx(1,i) ); //Ply1 + i , Ply2 + i);
      // PLY_2 -> PLY_3
      bdd_setpair( shiftright , vidx(1,i) , vidx(2,i) ); //Ply2 + i , Ply3 + i );

      // PLY_2 -> PLY_1
      bdd_setpair( shiftleft  , vidx(1,i) , vidx(0,i) ); //Ply2 + i , Ply1 + i );
      // PLY_3 -> PLY_2
      bdd_setpair( shiftleft  , vidx(2,i) , vidx(1,i) ); //Ply3 + i , Ply2 + i );
    }
  }

  // ////////////////////////
  // Creat shift32 pair
  {
    shift32  = bdd_newpair();
    for (int i=0; i < PLYVARS; i++)
    {
      // PLY_3 -> PLY_2
      bdd_setpair(shift32 , vidx(2,i) , vidx(1,i) );//Ply3 + i , Ply2 + i);
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
      p1 = p1 & var(0,i);
      p2 = p2 & var(1,i);
      p3 = p3 & var(2,i);
    }
    pply1 = new bdd(p1);
    pply2 = new bdd(p2);
    pply3 = new bdd(p3);
  }

  // /////////////////////////
  // create acquisition bdds
  {
    //*waliErr << "[INFO] Building 'Acquire' relations." << endl;
    R_acquires = new bdd[LOCKS]();
    for (int l = 0 ; l < LOCKS ; l++)
      R_acquires[l] = acquire_lock(l);
  }

  // /////////////////////////
  // create release bdds
  {
    //*waliErr << "[INFO] Building 'Release' relations." << endl;
    R_releases = new bdd[LOCKS]();
    for (int l = 0 ; l < LOCKS ; l++)
      R_releases[l] = release_lock(l);
  }

  // //////////////////////////////////////////
  // create the "checker" BDD 
  {
    bdd tmp_checker = bddfalse;
    // Different states == invalid
    for (int qbit=0 ; qbit < QVARS ; qbit++)
    {
      tmp_checker = tmp_checker |
        (var(0,qbit) & nvar(1,qbit));
      tmp_checker = tmp_checker |
        (nvar(0,qbit) & var(1,qbit));
    }
    for (int i=0; i < LOCKS ; i++)
    {
      // Disjoint sets of currently held locks
      tmp_checker = tmp_checker | 
        (var(0,get_lock_in_L(i)) & // i \in L
         var(1,get_lock_in_L(i))); // i \in L'

      // For P1, let I1 = { li | li \in L1 & AH[i] = {} }
      // Check that I1 \cap U2 = {}
      // Vice versa for P2/P1. 
      bdd held_by_P1_used_by_P2 = 
        var(0,get_lock_in_L(i))    &  // li \in L1
        nvar( 0,get_ah_start(i)+i) &  // li \notin AH_1[i]
        var( 1,get_lock_in_U(i));     // li \in U2

      tmp_checker = tmp_checker | held_by_P1_used_by_P2;

      // Vice versa
      bdd held_by_P2_used_by_P1 =
        var(1,get_lock_in_L(i))    &  // li \in L2
        nvar( 1,get_ah_start(i)+i) &  // li \notin AH_2[i]
        var( 0,get_lock_in_U(i));     // li \in U1

      tmp_checker = tmp_checker | held_by_P2_used_by_P1;

      for (int j=0; j < LOCKS ; j++)
      {
        // ///////////////////////////
        //  Check AH sets
        // Select lock j in AH_i
        bdd i_in_Aj = var(0 , get_ah_start(i)+j);

        // Select lock i in AH'_j
        bdd j_in_Api = var(1 , get_ah_start(j) + i);
        tmp_checker = tmp_checker | (i_in_Aj & j_in_Api);

        // ///////////////////////////
        //  Check RH sets
        // Select lock j in RH_i
        bdd i_in_Rj = var(0 , get_rh_start(i) + j);

        // Select lock i in AH'_j
        bdd j_in_Rpi = var(1 , get_rh_start(j)+i);
        tmp_checker = tmp_checker | (i_in_Rj & j_in_Rpi);
      }
    }
    //printLH(*waliErr << "Checker\n", tmp_checker );
    R_checker = new bdd(bdd_not(tmp_checker));
    //printLH(*waliErr << "Checker\n", *R_checker );
  }
  return true;
}

bool LH::initialize( int num_locks, int the_Q )
{
  assert( LH::allocate(num_locks,the_Q) );
  assert( LH::init_vars() );
  return true;
}

LH LH::Empty()
{
  static bdd R = empty();
  return LH( R );
}

LH LH::Id()
{
  static bdd R = identity();
  return LH(R);
}

LH LH::Null()
{
  return LH(bddfalse);
}

LH LH::Acquire(int lock)
{
  assert(check_lock(lock));
  return LH(R_acquires[lock]);
}

LH LH::Release(int lock)
{
  assert(check_lock(lock));
  return LH(R_releases[lock]);
}

LH::LH( const LH& that )
  : R(that.R)
{
}

LH::~LH()
{
}

std::ostream& LH::print( std::ostream& o ) const
{
  printLH(o,R);
  o << endl;
  return o;
}

std::ostream& LH::prettyPrint(std::ostream& o, const std::string& name, bool subID) const
{
  // Header
  std::stringstream ss;
  ss << "[" << name;
  o << std::left << std::setw(20) << ss.str() << "] : \n";
  printLH(o,R,subID);
  o << endl;
  return o;
}

bool LH::is_equal( const LH& that) const
{
  return (R == that.R);
}

bool LH::is_id() const
{
  return is_equal(Id());
}

bool LH::is_empty() const
{
  return is_equal(Empty());
}

bool LH::is_null() const
{
  return is_equal(Null());
}

/** @return composition of [this] and [that] */
LH LH::Compose( const LH& that ) const
{
  bdd X = compose(R,that.R);
  return LH(X);
}

/** @return intersection of [this] and [that] */
LH LH::Intersect( const LH& that ) const
{
  return LH( (R & that.R) );
}

LH LH::Transition(int q1, int q2) const
{
  static LH T(transition());
  LH ID = Id();

  // TODO put back in
  //assert( q1 != q2 );
  // [this] = (q1,R,RH,U,AH,L) --> (q1,R',RH',U',AH',L')
  // Want   : (q1,R',RH',U',AH',L') --> (q2,0,0^k,0,0^k,L')

  // (1) Ensure that this = (q1,...) -> (q1,...)
  bdd Rq1p1 = q_in_ply(q1,0);
  bdd Rq1p2 = q_in_ply(q1,1);
  bdd pinq1 = R & Rq1p1 & Rq1p2;
  // (2) Quantify out ply 1
  bdd exist_ply1 = bdd_exist(pinq1, *pply1);
  // (3) Shift to the left
  bdd shift = bdd_replace( exist_ply1,shiftleft);
  // (4) Create (q1,R',RH',U',AH',L') -> (q1,R',RH',U',AH',L')
  bdd matched = exist_ply1 & shift & ID.R;
  // (5) Compose with T makes
  //   (q1,R',RH',U',AH',L') --> (q1,0,0^k,0,0^k,L')
  bdd R5 = compose(matched, T.R);
  // (6) Quantify out Q in ply 2
  bdd q_in_ply_1 = bddtrue;
  for (int bit = 0; bit < QVARS ; bit++)
  {
    q_in_ply_1 = q_in_ply_1 & var(1,bit); // TODO MOVE Q
  }
  bdd R6 = bdd_exist(R5, q_in_ply_1);
  // (7) insert q2 in ply 1
  bdd Rq2 = q_in_ply(q2,1);
  bdd R7 = R6 & Rq2;
  //printLH( std::cout << "!R7",R7);
  return R7;

}

/** @return union of [this] and [that] */
LH LH::Union( const LH& that ) const
{
  return LH( (R | that.R) );
}

/** @return true if Compatible([this],[that]) */
bool LH::Compatible( const LH& a, const LH& b )
{
  assert( LH::is_initialized() );
  std::vector<LH> v;
  v.push_back(a);
  v.push_back(b);
  return Compatible(v);
}

/** @return true if Compatible(v_1,...,v_N) */
bool LH::Compatible( const std::vector<LH>& v )
{
  assert( LH::is_initialized() );
  assert( v.size() >= 2 );
  LH lh = Summarize(v[0],v[1]);
  for (size_t idx = 2 ; idx < v.size() ; idx++)
  {
    lh = Summarize(lh,v[idx]);
  }
  return (lh != Null());
}


LH LH::operator|(const LH& that) const
{
  return this->Union(that);
}

LH LH::operator*(const LH& that) const
{
  return this->Compose(that);
}

LH LH::operator&(const LH& that) const
{
  return this->Intersect(that);
}

bool LH::operator==(const LH& that) const
{
  return R == that.R;
}

bool LH::operator!=(const LH& that) const
{
  return R != that.R;
}

LH::LH( bdd the_R ) : R(the_R)
{
}

bool LH::check_lock(int lock)
{
  if (!is_initialized())
  {
    *waliErr << "[ERROR] LH not initialized" << endl;
    return false;
  }
  else if ((lock < 0) || (lock >= LOCKS))
  {
    *waliErr << "[ERROR] Invalid lock " << lock << endl;
    return false;
  }
  return true;
}

bool LH::check_q( int q )
{
  if (!is_initialized())
  {
    *waliErr << "[ERROR] LH not initialized" << endl;
    return false;
  }
  else if ((q < 0) || (q >= Q))
  {
    *waliErr << "[ERROR] Invalid q " << q << endl;
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& o, const LH& lh)
{
  lh.print(o);
  return o;
}

int LH::vidx_base(int base, int ply, int v)
{
  assert((0<=v)   && (v < PLYVARS));
#if INTERLEAVE
  return base + (NUMPLYS*v) + ply;
#else
  return base + (ply * PLYVARS) + v;
#endif
}

int LH::vidx(int ply, int v)
{
  return vidx_base(BASE,ply,v);
}

bdd LH::var(int ply, int v)
{
  int idx = vidx(ply,v);
  return bdd_ithvar(idx);
}

bdd LH::nvar(int ply, int v)
{
  int idx = vidx(ply,v);
  return bdd_nithvar(idx);
}

/** @return the relation {} -> {} */
bdd LH::empty()
{
  assert( LH::is_initialized() );
  static int cnt = 0;
  cnt++;
  assert( cnt == 1 );
  bdd R = bddtrue;
  for (int v = 0; v < PLYVARS ; v++)
  {
    R = R & nvar(0,v) & nvar(1,v);
  }
  return R;
}

/** @return the relation forall S -> S. I.e., \lam S.S */
bdd LH::identity()
{
  assert( LH::is_initialized() );
  static int cnt = 0;
  cnt++;
  assert( cnt == 1 );
  //*waliErr << "LH::BASE = " << LH::BASE << endl;
  bdd R = bddtrue;
  for (int v=0; v < PLYVARS; v++)
  {
    R = R & bdd_biimp( var(0,v),var(1,v) );
  }
  return R;
}

/** @return relation (q,R,RH,U,AH,L) -> (q,\empty,\empty^k,\empty,\empty^k,L) */
bdd LH::transition()
{
  assert( LH::is_initialized() );
  LH ID = Id();
  bdd R = ID.R;
#if 0
  // Unset all vars except the last set of locks L
  for (int v = QVARS ; v < PLYVARS-LOCKS ; v++)
  {
    R = unset( R , 1 , v );
  }
#endif
  // This a slower more methodical way of doing the above
  for (int li = 0; li < LOCKS ; li++)
  {
    // (1) l \notin R'
    bdd li_notin_R = unset( ID.R , 1 , get_lock_in_R(li) );
    R = compose(R,li_notin_R);
    // (2) l \notin U'
    bdd li_notinU = unset( ID.R , 1 , get_lock_in_U(li) );
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
  return R;
}

/* Return relation R[true/(b \in ply)] */
bdd LH::set(bdd R, int ply, int b)
{
  bdd x = var(ply,b);
  return bdd_exist(R,x) & x;
}

/* Return relation R[false/(b \in ply)] */
bdd LH::unset(bdd R, int ply, int b)
{
  return bdd_exist(R,var(ply,b)) & nvar(ply,b);
}

/* Relation composition of R1 and R2 */
bdd LH::compose(bdd R1, bdd R2)
{
  bdd p2   = *pply2;
  bdd sh23 = bdd_replace( R2 , shiftright );
  bdd tmp  = bdd_relprod( R1 , sh23 , p2);
  return bdd_replace( tmp , shift32 );
}

int LH::get_rh_start(int lock)
{
  // PLY is (q,R,RH,U,AH,L)
  return (
      QVARS       + // skip q 
      LOCKS       + // skip R
      (LOCKS * lock));  // Get start of RH[lock]
}

int LH::get_ah_start(int lock)
{
  // PLY is (q,R,RH,U,AH,L)
  return (
      QVARS           + // skip q
      LOCKS           + // skip R
      (LOCKS * LOCKS) + // skip RH
      LOCKS           + // skip U
      (LOCKS * lock));  // Get start of AH[lock]
}

int LH::get_lock_in_L( int lock )
{
  // Skip over I and RH_is, and add lock number
  // PLY is (q,R,RH,U,AH,L)
  return (
      QVARS           + // skip q
      LOCKS           + // skip R
      (LOCKS * LOCKS) + // skip RH
      LOCKS           + // skip U
      (LOCKS * LOCKS) + // skip AH
      + lock
      );  
}

int LH::get_lock_in_R( int lock )
{
  return 
    QVARS + // skip q
    lock;
}

int LH::get_lock_in_U( int lock )
{
  return (
      QVARS           + // skip q
      LOCKS           + // skip over R
      (LOCKS * LOCKS) + // skip over RH
      lock);
}

void LH::print_q( int ply, bdd r)
{
  std::cout << "q:";
  for( int bit = 0; bit < QVARS ; bit++)
  {
    bdd yes = var(ply,bit);
    bdd no = nvar(ply,bit);
    if ((r & yes) != bddfalse)
    {
      if ((r&no) != bddfalse)
        std::cout << "*";
      else
        std::cout << "1";
    }
    else if ((r & no) != bddfalse)
      std::cout << "0";
  }
  std::cout << ":";
}

/** @return bdd with bits set to represent q */
bdd LH::q_in_ply( int q, int ply )
{
  assert( check_q(q) );
  bdd r = bddtrue;
  int shift = q;
  for(int bit = 0; bit < QVARS ; bit++)
  {
    if (shift & 0x1)
      r = r & var(ply,bit);
    else
      r = r & nvar(ply,bit);
    shift >>= 1;
  }
  //std::cout << "q" << q << " == ";
  //print_q(ply,r);
  //std::cout << std::endl;
  return r;
}


bdd LH::acquire_lock( int lock )
{
  assert(check_lock(lock));
  static int cnt = 0;
  cnt++;
  assert( cnt <= LOCKS );
  LH ID = Id();

  // (1) li \notin L
  bdd notinL = ID.R & nvar(0,get_lock_in_L(lock));
  // (1') li \notin AH[i]
  notinL = notinL & nvar(0,get_ah_start(lock)+lock);

  // (2) L' = L \cup {lock}
  bdd set_lock = set( notinL , 1 , get_lock_in_L(lock) );

  // (3) AH'[lock] = {lock}
  set_lock = set( set_lock , 1 , get_ah_start(lock)+lock);

  // (3) l \in AH'_l --> [lock] \in AH'_l
  bdd R = set_lock;
  for (int l = 0 ; l < LOCKS ; l++)
  {
    bdd if_l_in_AH2_l = var(1,get_ah_start(l)+l);

    bdd lock_in_AH2_l = set( ID.R , 1 , get_ah_start(l)+lock );

    bdd add_lock_to_AH_l = 
      bdd_ite( 
          if_l_in_AH2_l , 
          lock_in_AH2_l , 
          ID.R);
    R = compose( R , add_lock_to_AH_l );
  }
  return R;
}

bdd LH::release_lock( int li )
{
  assert(check_lock(li));
  static int cnt = 0;
  cnt++;
  assert( cnt <= LOCKS );
  LH ID = Id();
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
  bdd inAHi = ID.R & var(0,get_ah_start(li)+li);
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
  bdd notinAHi = ID.R & nvar(0,get_ah_start(li)+li);
  // (2) R' = R \cup {li}
  bdd set_li_in_R = set(notinAHi, 1, get_lock_in_R(li));
  // (3) RH'[i] = {li} \cup U \cup R
  bdd r3 = set_li_in_R;
  for (int l = 0 ; l < LOCKS ; l++)
  {
    bdd if_l_in_U = var(0,get_lock_in_U(l));
    bdd if_l_in_R = var( 0 , get_lock_in_R(l) );
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
  bdd if_li_in_AHi = var( 0,get_ah_start(li)+li);
  bdd R = bdd_ite( if_li_in_AHi, T, F );
  return R;

}

void LH::printPly(char* v, int size, int ply)
{
  using namespace std;
  std::stringstream ss;
  // for q , R, RH_1 , ... , RH_{LOCKS} , U , AH_1 , ... , AH_{LOCKS}, L
  // A ply has:
  //   1. QVARS
  //   2. (3+2*|LOCKS|) sets of vars of size |LOCKS|
  ss << "q:";
  for (int bit = 0; bit < QVARS; bit++)
  {
    switch(v[vidx(ply,bit)])
    {
      case -1:
        ss << "*";
        break;
      case 1:
        ss << '1';
        break;
      case 0:
        ss << '0';
        break;
    }
  }
  ss << ":";
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
      int vnum = i*LOCKS + l + QVARS;
      int offset = vidx(ply,vnum);
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

void LH::printHandler(char* v, int size)
{
  cout << "  ";
  printPly(v,size,0);
  cout << "\n     ==>  ";
  printPly(v,size,1);
  //cout << "\n       ==>  ";
  //printPly(v,size,2);
  cout << endl;
}

void LH::printLH(std::ostream& o , bdd R, bool subID)
{
  LH x(R);
  if (x.is_id())
    o << "  ID"    << endl;
  else 
    if (x.is_null())
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
      bdd_allsat((R - Id().R),printHandler);
    else
      bdd_allsat(R,printHandler);
  }
}

/** 
 * The relation for Compatible(LH_1,LH_2)
 * is used to compute a yes/no answer, and
 * has no use for the third (scratch) ply.
 * The relation for Summarizer further restricts
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
bdd LH::Summarizer()
{
  assert( LH::is_initialized() );
  bdd sum = *R_checker;
  for (int q = 0; q < Q ; q++)
  {
    sum = bdd_ite(
        (q_in_ply(q,0) & q_in_ply(q,1)),
        (sum & q_in_ply(q,2)),
        sum);
  }
  for (int l = 0 ; l < LOCKS; l++)
  {
    // (1) l \in L1 & l \notin L2 => l \in L3
    // (2) l \notin L1 &  l \in L2    => l \in L3
    sum = bdd_ite(
        (
          (var(0,get_lock_in_L(l)) & nvar(1,get_lock_in_L(l))) |
          (nvar(0,get_lock_in_L(l)) & var(1,get_lock_in_L(l)))
        ),
        set(sum, 2, get_lock_in_L(l)),
        unset(sum, 2, get_lock_in_L(l)) 
        );

    // (3) r \in R1    &  r \notin R2 => r \in R3
    // (4) r \notin R1 &  r \in R2    => r \in R3
    sum = bdd_ite(
        (
         (var(0,get_lock_in_R(l)) & nvar(1,get_lock_in_R(l))) |
         (nvar(0,get_lock_in_R(l)) & var(1,get_lock_in_R(l)))
        ),
        set(sum, 2, get_lock_in_R(l)),
        unset(sum, 2, get_lock_in_R(l))
        );

    // (5) l \in U1    |  l \in U2    => l \in U3
    sum = bdd_ite(
        (var(0,get_lock_in_U(l)) | var(1,get_lock_in_U(l))),
        set(sum, 2, get_lock_in_U(l)),
        unset(sum, 2, get_lock_in_U(l)) 
        );
    const int i = l;
    for (int j = 0 ; j < LOCKS ; j++)
    {
      { // (6) j \in AH1[i] | j \in AH2[i] => j \in AH3[j]
        bdd j_in_A1i = var(0 , get_ah_start(i)+j);
        bdd j_in_A2i = var(1 , get_ah_start(i)+j);
        sum = bdd_ite(
            (j_in_A1i | j_in_A2i),
            set(sum, 2, get_ah_start(i)+j),
            unset(sum, 2, get_ah_start(i)+j)
            );
      }
      { // (7) j \in RH1[i] | j \in RH2[i] => j \in RH3[j]
        bdd j_in_R1i = var(0 , get_rh_start(i)+j);
        bdd j_in_R2i = var(1 , get_rh_start(i)+j);
        sum = bdd_ite(
            (j_in_R1i | j_in_R2i),
            set(sum, 2, get_rh_start(i)+j),
            unset(sum, 2, get_rh_start(i)+j)
            );
      }
    }
    for (int j = 0 ; j < LOCKS ; j++)
    {
      // (8) j \in AH1[i] & h \in AH2[j] => h \in AH3[i]
      // (9) j \in RH1[i] & h \in RH2[j] => h \in RH3[i]
      bdd j_in_A1i = var(0,get_ah_start(i)+j);
      bdd j_in_R1i = var(0,get_rh_start(i)+j);
      for (int h = 0 ; h < LOCKS ; h++)
      {
        bdd h_in_A2j = var(1,get_ah_start(j)+h);
        sum = bdd_ite(
            (j_in_A1i & h_in_A2j),
            set(sum, 2, get_ah_start(i)+h),
            sum);
        bdd h_in_R2j = var(1,get_rh_start(j)+h);
        sum = bdd_ite(
            (j_in_R1i & h_in_R2j),
            set(sum, 2, get_rh_start(i)+h),
            sum);
      }
    }
  }
  return sum;
}

/** @return Summary LH for LHs [a] and [b] */
LH LH::Summarize( const LH& a, const LH& b )
{
  static bdd Rsum = Summarizer();

  LH path1 = Empty().Compose(a);
  //path1.prettyPrint(std::cout,"Path 1");

  LH path2 = Empty().Compose(b);
  //path2.prettyPrint(std::cout,"Path 2");

  // ///////////////////////////////////
  // Perform test
  //    \exists li,lj . li \in AH_j && lj \in AH_i 
  // 
  // Note that L \cap L' not necessary b/c of Peter's mod to definition of 
  // AH and putting lock [l] in [AH_l].
  //
  // (1) existentially quantify out PLY 1 in path1 and path 2
  bdd quantify_ply_1_for_path1 = bdd_exist( path1.R , *pply1 );
  bdd quantify_ply_1_for_path2 = bdd_exist( path2.R , *pply1 );

  // (2) Shift path 1 :: PLY 2 -> PLY1 
  bdd path1_shiftleft = bdd_replace(quantify_ply_1_for_path1,shiftleft);

  // (3) Logically AND path 1 and path 2
  bdd p1_then_p2 = path1_shiftleft & quantify_ply_1_for_path2;
  //LH(p1_then_p2).prettyPrint(cout, "P1 o P2");

  // (4) Check if solution exists.
  //bdd R4 = p1_then_p2 & (*R_checker);
  bdd R4 = p1_then_p2 & Rsum;
  { // DEBUGGING 
    //LH lh4(R4);
    //lh4.prettyPrint(std::cout, "R4");
  }

  // (5) Quantify out plys 1 and 2
  bdd R5 = bdd_exist( bdd_exist( R4,*pply1), *pply2 );

  // (6) And with Empty() relation, and then shift left 1
  bdd R6 = bdd_replace(bdd_exist((Empty().R & R5), *pply1),shiftleft);
  LH lh6(R6);
  { // DEBUGGING 
    lh6.prettyPrint(std::cout, "R6");
  }
  return lh6;
}


