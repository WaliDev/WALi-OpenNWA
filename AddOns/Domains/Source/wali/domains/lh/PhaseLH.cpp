/**
 * @author Nicholas Kidd
 * @version $Id: PhaseLH.cpp 617 2009-01-16 23:23:40Z kidd $
 */

/**
 * Transformers implement post so that summarization of paths
 * computes the "eta" tranformer. These have no effect on q.
  
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
#define BDDMEMSIZE (FIVE_CENT_MB*3)

#else
#define BDDMEMSIZE 10000000
#endif

#define INTERLEAVE 1

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "wali/domains/lh/LH.hpp"
#include "wali/domains/lh/PhaseLH.hpp"

using namespace wali::domains::lh;
using wali::waliErr;
using std::cout;
using std::endl;

const int PhaseLH::MAX_LOCKS     = 4;
bool PhaseLH::DO_INV_CHECK       = false;
int PhaseLH::BASE                = -1;
int PhaseLH::LOCKS               = -1;
int PhaseLH::PHASES              = -1;
int PhaseLH::Q                   = -1;
int PhaseLH::QVARS               = -1;
int PhaseLH::NUMPLYS             = -1;
int PhaseLH::PLYVARS             = -1;
int PhaseLH::NUMVARS             = -1;
bddPair* PhaseLH::shiftright     = 0;
bddPair* PhaseLH::shiftleft      = 0;
bdd* PhaseLH::ply_selectors      = 0;
bddPair** PhaseLH::left_from_ply = 0;
int PhaseLH::kludge              = -1;
int PhaseLH::N                   = -1;


int PhaseLH::getln(int v)
{
  int acc = 0;
  int orig = v;
  while( v != 1)
  {
    acc++;
    v = v/2;
  }
  return (orig&0x1) ? (acc+1) : acc;
}

bool PhaseLH::is_initialized()
{
  return (LOCKS != -1);
}

static void my_error_handler(int errcode)
{

  *wali::waliErr << "[BuDDy ERROR] In PhaseLH : " << bdd_errstring(errcode) << std::endl;
  throw errcode;
}

bool PhaseLH::allocate( int num_locks, int phases , int theQ )
{
  if (is_initialized())
  {
    *waliErr << "[WARNING] PhaseLH is already initialized." << endl;
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

  LOCKS    = num_locks;                 // Set number of locks
  PHASES   = phases;                    // Set number of phases
  Q        = theQ;                      // Set Q
  QVARS    = getln(Q);                  // Set number of QVars
  PLYVARS  = QVARS + 3*LOCKS + 2*(LOCKS*LOCKS); // @see description above
  NUMPLYS  = PHASES + 2;                // @see description above
  NUMVARS  = PLYVARS * NUMPLYS;      // Total number of vars
  cout << "[INFO] PhaseLH.\n";
  cout << "  #LOCKS   = " << LOCKS << ";\n"; 
  cout << "  #PHASES  = " << PHASES << ";\n";
  cout << "  #Q       = " << Q << ";\n";
  cout << "  #QVARS   = " << QVARS << ";\n";
  cout << "  #PLYVARS = " << PLYVARS << endl;
  cout << "  #NUMPLYS = " << NUMPLYS << ";\n";
  cout << "  #NUMVARS = " << NUMVARS << ";\n";
  BASE     = bdd_extvarnum(NUMVARS);
  // initialize LH vars
  LH::BASE    = BASE;
  LH::PLYVARS = PLYVARS;
  LH::NUMPLYS = NUMPLYS;
  LH::NUMVARS = PLYVARS * 3;
  LH::LOCKS   = LOCKS;
  LH::Q       = Q;
  LH::QVARS   = QVARS;
  LH::init_vars();
  return true;
}

bool PhaseLH::init_vars()
{

  // ////////////////////////
  // Create shiftright/shiftleft pairs
  {
    shiftright = bdd_newpair();
    shiftleft  = bdd_newpair();
    for (int ply = 0 ; ply < (NUMPLYS-1) ; ply++)
    {
      //*waliErr << " ply " << ply << " -> " << (ply+1) << endl;
      //*waliErr << " ply " << (ply+1) << " <- " << ply << endl;
      for (int v=0; v < PLYVARS; v++)
      {
        // PLY_i -> PLY_{i+1}
        bdd_setpair( shiftright , vidx(ply,v)   , vidx(ply+1,v) ); //ply_i    -> ply{i+i}
        bdd_setpair( shiftleft  , vidx(ply+1,v) , vidx(ply,v) );   //ply{i+1} -> ply_i
      }
    }
  }

  // ////////////////////////
  { // Create the ply selectors
    ply_selectors = new bdd[NUMPLYS];
    for (int ply = 0 ; ply < NUMPLYS ; ply++)
    {
      bdd tmp = bddtrue;
      for (int v = 0 ; v < PLYVARS ; v++)
      {
        tmp = tmp & var(ply,v);
      }
      ply_selectors[ply] = tmp;
    }
  }

  // ////////////////////////
  { // Create the shift left from ply pairs.
    left_from_ply = new bddPair*[NUMPLYS]; 
    // Must start from ply 2 or greater.
    // Ply 1 always has vars.
    // Least ply quantified out is 1.
    left_from_ply[0] = NULL;
    left_from_ply[1] = NULL;
    for (int idx = 2 ; idx < NUMPLYS ; idx++)
    {
      bddPair* p = bdd_newpair();
      //*waliErr << "Left from ply " << idx << endl;
      for (int ply = idx ; ply < NUMPLYS ; ply++)
      {
        //*waliErr << "     ply " << ply << " -> " << ply -1 << endl;
        for (int v = 0 ; v < PLYVARS; v++)
        {
          //*waliErr << "          " << ply << ":" << v << " -> " << (ply-1) << ":" << v << endl;
          bdd_setpair( p , vidx(ply,v), vidx(ply-1,v) );
        }
      }
      //*waliErr << endl;
      left_from_ply[idx] = p;
    }
  }
   
  return true;
}

bool PhaseLH::initialize( int num_locks, int phases, int the_Q )
{
  if (the_Q == -1)
    the_Q = phases;
  assert( PhaseLH::allocate(num_locks,phases,the_Q) );
  assert( PhaseLH::init_vars() );
  return true;
}

PhaseLH::PhaseLH( const PhaseLH& that )
  : R(that.R), phaseX(that.phaseX)
{
  invariant_check(R,phaseX);
}


PhaseLH::~PhaseLH()
{
}

PhaseLH PhaseLH::Empty()
{
  static bdd R = LH::Empty().R;
  return PhaseLH( R,0 );
}

PhaseLH PhaseLH::Id()
{
  static bdd R = LH::Id().R;
  return PhaseLH(R,0);
}

PhaseLH PhaseLH::Null()
{
  return PhaseLH(bddfalse,0);
}

PhaseLH PhaseLH::Acquire(int lock, int i ATTR_UNUSED)
{
  assert(check_lock(lock));
  return PhaseLH(LH::R_acquires[lock],0);
}

PhaseLH PhaseLH::Release(int lock, int i ATTR_UNUSED)
{
  assert(check_lock(lock));
  return PhaseLH(LH::R_releases[lock],0);
}

PhaseLH PhaseLH::Transition( int q1 , int q2 )
{
  // (1) Start in ply 0 and ply 1
  // (q1,R,RH,U,AH,L) -> (q1,R',RH',U',AH',L')
  LH id = LH::Id();

  // (2) Transition the LH to get
  //    (q1,R',RH',U',AH',L') --> (q2,0,0^k,0,0^k,L')
  LH t = id.Transition(q1,q2);

  // (3) Shift t to the right
  bdd r3 = bdd_replace( t.R , shiftright );

  // (4) logically and id and r3
  bdd r4 = id.R & r3;

  // (5) Return PhaseLH with 1 transition
  return PhaseLH(r4,1);
}

std::ostream& PhaseLH::prettyPrint( std::ostream& o ) const
{
  return print(o);
}

std::ostream& PhaseLH::print( std::ostream& o ) const
{
  assert( kludge == -1 );
  kludge = phaseX;
  printPhaseLH(o,R);
  o << endl;
  kludge = -1;
  return o;
}

bool PhaseLH::is_equal( const PhaseLH& that) const
{
  return (R == that.R);
}

bool PhaseLH::is_id() const
{
  return is_equal(Id());
}

bool PhaseLH::is_empty() const
{
  return is_equal(Empty());
}

bool PhaseLH::is_null() const
{
  return is_equal(Null());
}

/** @return composition of [this] and [that] */
PhaseLH PhaseLH::operator*( const PhaseLH& that ) const
{
  //bdd X = compose(R,that.R);
  //return PhaseLH(X);
  int pX = phaseX + that.phaseX;
  if (pX >= PHASES)
  {
    *wali::waliErr << "\nTOO MANY PHASE CHANGES FOR EXTEND!\n" << std::endl;
    assert( false );
  }
  // The ply that needs to be joined on
  int target_ply = phaseX + 1;
  //*waliErr << "[PEH:op*] target ply : " << target_ply << endl;

  // (1) Shift that.R right # of phase changes.
  bdd r1 = that.R;
  int accum = that.phaseX;
  for (int x = 0; x < target_ply ; x++)
  {
    r1 = bdd_replace(r1,shiftright);
    accum++;
    PhaseLH tmp(r1,accum);
  }
  //PhaseLH t8(r1,accum);
  //t8.print(*waliErr << "R1 ~~>\n") << endl;

  // (2) Do the composition
  bdd r2 = bdd_relprod(R,r1,ply_selectors[target_ply]);
  //PhaseLH tmp(r2,phaseX+that.phaseX+1);
  //tmp.print(*waliErr<< "r2:\n");
  //*waliErr << "---------------\n";

  // (3) Shift back out now.
  //     Quantified out ply target_ply.
  //     Do shift from target_ply+1
  //*waliErr << "*Shift back left from " << (target_ply+1) << endl;
  bdd r3 = bdd_replace(r2,left_from_ply[ (target_ply+1) ] );

  //*waliErr << "New phaseX = " << phaseX + that.phaseX << endl;
  PhaseLH p3( r3, (phaseX+that.phaseX) );
  //p3.print(*waliErr<< "p3:\n");
  //*waliErr << "---------------\n";
  return p3;
}

/** @return intersection of [this] and [that] */
PhaseLH PhaseLH::operator&( const PhaseLH& that ) const
{
  if (phaseX != that.phaseX)
  {
    std::cout << "[ Error in PhaseLH::operator& ] Mismatched phase changes.\n";
    print( std::cout << "THIS---\n" ) << std::endl;
    that.print( std::cout << "THAT---\n") << std::endl;
    assert( false );
  }
  return PhaseLH( (R & that.R),phaseX );
}

/** @return union of [this] and [that] */
PhaseLH PhaseLH::operator|( const PhaseLH& that ) const
{
  if (phaseX != that.phaseX)
  {
    std::cout << "[ Error in PhaseLH::operator| ]\n";
    print( std::cout << "THIS---\n" ) << std::endl;
    that.print( std::cout << "THAT---\n") << std::endl;
    assert( false );
  }
  return PhaseLH( (R | that.R),phaseX );
}

bool PhaseLH::operator==(const PhaseLH& that) const
{
  return R == that.R;
}

bool PhaseLH::operator!=(const PhaseLH& that) const
{
  return R != that.R;
}

/** @return true if Compatible([this],[that]) */
bool PhaseLH::Compatible( PhaseLH t1, PhaseLH t2 )
{
  std::vector<PhaseLH> v;
  v.push_back(t1);
  v.push_back(t2);
  return Compatible(v);
}

bool PhaseLH::Compatible( std::vector< PhaseLH >& v )
{
#if 0
  {
    assert( PhaseLH::is_initialized() );
    N = v.size();
    const int Q = PHASES;
    std::stringstream message;
    message << "PhaseLH::Compatible(vector< PhaseLH >)";
    message << " {N=" << N << " , Q=" << Q << "}.";
    //*waliErr << "[INFO] " << message.str() << endl;
    //wali::util::Timer t(message.str());
    // E.g., Q = 3, N = 3
    //  o v[0].R has form xyDR_
    // Want to shuffle the PhaseLH for each P_i
    // to get:
    //   1_1 -> ... _> 1_n -> 2_1...2_n -> 3_1 -> ... -> 3_n
    //
    // Total number of vars then is N * Q * LH::PLYVARS
    int total_vars = N * PHASES * PLYVARS;
    //*waliErr << "[INFO] Have #vars " << NUMVARS << endl;
    //*waliErr << "[INFO] Need #vars " << total_vars << endl;
    int extra_vars = total_vars - NUMVARS;
    //*waliErr << "[INFO] Diff #vars " << extra_vars << endl;
    if (extra_vars > 0)
      bdd_extvarnum(extra_vars);
    // checker will be of the form (C_11...C1n, ... , Cq1...Cqn )
    // where Cij comes from the LH::Compatible relation.
    bdd checker = bddtrue;

    // (1) copy {LH::R_checker} = {(C,C) , __DONT CARE__ }
    bdd LH_checker = *LH::R_checker; // E.g., \eq __CCS

    // (2) Reorder the vars in the LH_checker for multiple procs
    //     using cidx.
    //int PhaseLH::cidx(int N, int proc , int phase , int v)
    //*waliErr << "#######  Making PA_checker.\n";
    bdd PA_checker = LH_checker;
    {
      bddPair* p = bdd_newpair();
      for (int v = 0 ; v < PLYVARS ; v++)
      {
        int from = vidx(0,v);
        int to   = cidx(N,0,0,v);
        //*waliErr << "    -- P0 : " << from << "  ->  " << to << endl;
        if (from != to)
          bdd_setpair(p, from, to );//vidx(0,v), cidx(N, 0, 0, v) );
        from = vidx(1,v);
        to   = cidx(N,1,0,v);
        //*waliErr << "    -- P1 : " << from << "  ->  " << to << endl;
        bdd_setpair(p, from, to );//vidx(1,v), cidx(N, 1, 0, v) );
      }
      PA_checker = bdd_replace(LH_checker,p);
      bdd_freepair(p);
    }
    //bdd_allsat(PA_checker,print_r_compat);


    // (3) Make the (C_11...C1n, ... , Cq1...Cqn )
    // 3a. Make the relation (C_11,...C_1n,__ DONT CARE )
    //     to check phase 1 for each process Pi,Pj
    //int PhaseLH::cidx(int N, int proc , int phase , int v)
    //*waliErr << "#######  Extending PA_checker to 'N' processes.\n";
    bdd a3 = PA_checker;
    for (int i = 0 ; i < N ; i++)
    {
      // First shift C1j into position
      for (int j = i+1 ; j < N ; j++)
      {
        if (i == 0 && j == 1) // (0,1) case defined by PA_checker
          continue;
        //*waliErr << "  -- Making (" << i << " , " << j << ")" << endl;
        bddPair* pair    = bdd_newpair();
        for (int pos=0 ; pos < LH::PLYVARS ; pos++)
        {
          // ply 0 -> ply i
          int from = cidx(N,0,0,pos);
          int to   = cidx(N,i,0,pos);
          if (i != 0)
          {
            //*waliErr << "    -- 0~>i : " << from << "  ->  " << to << endl;
            bdd_setpair(pair, from, to );
          }
          // ply 1 -> ply j
          from = cidx(N,1,0,pos);
          to = cidx(N,j,0,pos);
          //*waliErr << "    -- 1~>j : " << from << "  ->  " << to << endl;
          bdd_setpair(pair, from, to );
        }
        a3 = a3 & bdd_replace(PA_checker,pair);
        bdd_freepair(pair);
      }
    }
    //*waliErr << "here: " << __FILE__ << ":" << __LINE__ << endl;
    { // DEBUGGING
      bdd verif = bddtrue;
      for (int phase = 1; phase < PHASES; phase++)
      {
        for (int proc = 0; proc < N ; proc++)
        {
          for (int pos = 0; pos < PLYVARS; pos++)
          {
            int var = cidx(N,proc,phase,pos);
            verif = verif & bdd_ithvar(var);
          }
        }
      }
      bdd exist = bdd_exist(a3, verif);
      assert( a3 == exist );  
    }

    //int PhaseLH::cidx(int N, int proc , int phase , int v)
    // 3b. Copy (C_11,...,C_1n, into the other phase positions
    //*waliErr << "#######  Duplicating PA_checker.\n";
    bdd b3 = a3;
    for (int phase = 1 ; phase < PHASES ; phase++)
    {
      //*waliErr << endl;
      bdd Rphase = bddtrue;
      bddPair* pair    = bdd_newpair();
      for ( int proc = 0; proc < N ; proc++)
      {
        for (int v = 0 ; v < PLYVARS ; v++)
        {
          int from = cidx(N,proc,0,v);
          int to   = cidx(N,proc,phase,v);
          //*waliErr << "    -- " << from << "  ->  " << to << endl;
          bdd_setpair(pair, from, to ); //cidx(N,proc,0,v), cidx(N,proc,phase,v) );
        }
      }
      Rphase = Rphase & bdd_replace(a3,pair);
      bdd_freepair(pair);
      { // DEBUGGING
        bdd verif = bddtrue;
        for (int ph = 0; ph < PHASES; ph++)
        {
          if (ph == phase)
            continue;
          for (int proc = 0; proc < N ; proc++)
          {
            for (int pos = 0; pos < PLYVARS; pos++)
            {
              int var = cidx(N,proc,ph,pos);
              verif = verif & bdd_ithvar(var);
            }
          }
        }
        bdd exist = bdd_exist(Rphase, verif);
        assert( Rphase == exist );  
      }
      b3 = b3 & Rphase;
      //*waliErr << endl;
    }
    bdd rCompatible = b3;
    //bdd_allsat(rCompatible,print_r_compat);

    // 4. Interleave the vector of PhaseLH's's
    std::vector<bdd> fromStart;
    for (size_t idx = 0 ; idx < v.size() ; idx++)
    {
      PhaseLH startEmpty = PhaseLH::Empty() * v[idx];
      bdd quant_ply_1 = bdd_exist(startEmpty.R,ply_selectors[0]);
      { // DEBUGGING
        invariant_check( quant_ply_1, PHASES-1 );
      }
      fromStart.push_back( quant_ply_1 );
    }
    bdd shuffle = bddtrue;
    // E.g., PHASES = 3, N = 3
    //  proc = 0
    //      E1 _ _ E2 _ _ E3 _ _
    //  proc = 1
    //      _ E1 _ _ E2 _ _ E3 _
    //  proc = 1
    //      _ _ E1 _ _ E2 _ _ E3
    for (int proc = 0 ; proc < N ; proc++)
    {
      //*waliErr << "  -- Shuffling proc P_" << proc << endl;
      bdd r_proc = fromStart[proc];
      bddPair* pair = bdd_newpair();
      // Ignore E0 b/c it is {}
      for (int ply = 1 ; ply <= PHASES ; ply++)
      {
        //int PhaseLH::cidx(int N, int proc , int phase , int v)
        for (int pos = 0 ; pos < PLYVARS ; pos++)
        {
          const int From = vidx(ply,pos);
          const int To   = cidx(N,proc,ply-1,pos);
          //*waliErr << "    o " <<  From << " ~> " << To << endl;
          bdd_setpair( pair , From, To );
        }
      }
      r_proc = bdd_replace( r_proc, pair );
      bdd_freepair(pair);
      shuffle = shuffle & r_proc;
    }
    bdd Verify = rCompatible & shuffle;
    return (Verify != bddfalse);
  }
#else
  {
    assert( v.size() >= 2 );
    PhaseLH plh = Summarize(v[0],v[1]);
    for (size_t idx = 2 ; idx < v.size() ; idx++)
    {
      plh = Summarize(plh,v[idx]);
    }
    return (plh != Null());
  }
#endif
}

#define DBG_SUM_JOIN 0
#define DBG_SUM_COMP 0
PhaseLH PhaseLH::Summarize( const PhaseLH& a , const PhaseLH& b)
{
  assert( PhaseLH::is_initialized() );
  static bdd all_a_b = alloc_summarize_vars();
  static bddPair* pair_unshift = make_unshift();
  static bdd* checkers = make_phsum_checks();
  static bddPair* pairA = NULL;
  static bddPair* pairB = NULL;
  bdd A = bdd_exist((PhaseLH::Empty() * a).R,ply_selectors[0]);
  bdd B = bdd_exist((PhaseLH::Empty() * b).R,ply_selectors[0]);
  // Shuffle the plies around
  if (pairA == NULL)
  {
    assert (pairB == NULL);
    pairA = bdd_newpair();
    pairB = bdd_newpair();
    for ( int p = 1 ; p <= PHASES ; p++ )
    {
#if DBG_SUM_JOIN
      *waliErr << "  Phase " << p << endl;
#endif
      const int ToPlyA = (p-1)*3;
      const int ToPlyB = ToPlyA + 1;
      for (int v = 0 ; v < PLYVARS ; v++)
      {

        const int From = vidx(p,v);
        const int ToA  = sidx(ToPlyA,v);
        const int ToB  = sidx(ToPlyB,v);
        if (From != ToA)
        {
#if DBG_SUM_JOIN
          *waliErr << "    -a- " <<  From << " ~> " << ToA << endl;
#endif
          bdd_setpair( pairA , From, ToA );
        }
        else
        {
          assert( false );
        }
        if (From != ToB)
        {
#if DBG_SUM_JOIN
          *waliErr << "    -b- " <<  From << " ~> " << ToB << endl;
#endif
          bdd_setpair( pairB , From, ToB );
        }
      }
    }
  }
  else
  {
    assert (pairB != NULL);
  }
  // (1) Shuffle [a] into place
  bdd R1 = bdd_replace( A , pairA );
  // (2) Shuffle [b] into place
  bdd R2 = bdd_replace( B , pairB );

  // (3) Join shuffled [a] and [b]
  bdd R3 = R1 & R2;
  { // DEBUGGING 
#if DBG_SUM_JOIN
    *waliErr << "--- (PhaseLH) R3 ---\n";
    bdd_allsat(R3,print_sum);
#endif
  }


  // (4) Do compatibility check and summarize
  bdd R4 = R3;
  for (int phase = 0 ; phase < PHASES ; phase++)
  {
    R4 = R4 & checkers[phase];
    { // DEBUGGING 
#if DBG_SUM_COMP
      *waliErr << "--- (PhaseLH.Compat) R4 (" << phase << ") ---\n";
      bdd_allsat(R4,print_sum);
      if (R4 == bddfalse)
      {
        *waliErr << " NULL created @" << phase << endl;
      }
#endif
    }
  }

  // (5) Quantify out A and B from CompatAndSum
  bdd R5 = bdd_exist( R4 , all_a_b );
  // (6) Shift summarized bdd back into place
  bdd R6 = bdd_replace( R5 , pair_unshift );
  { // DEBUGGING 
#if DBG_SUM_COMP
    PhaseLH dbg6( R6, a.phaseX );
    dbg6.prettyPrint(*waliErr << "(PhaseLH) R6\n");
#endif
  }

  // Could pin ply 1 to be "\LH_0", but not necessary.
  //*waliErr << ((R6 != bddfalse) ?
  //    "Paths are compatible." :
  //    "Paths are *IN*compatible.") << std::endl;
  return PhaseLH( R6 , a.phaseX );
}

PhaseLH::PhaseLH( bdd the_R,int the_phaseX ) : R(R),phaseX(the_phaseX)
{
  invariant_check(the_R,the_phaseX);
}

bool PhaseLH::check_lock(int lock)
{
  if (!is_initialized())
  {
    *waliErr << "[ERROR] PhaseLH not initialized" << endl;
    return false;
  }
  else if ((lock < 0) || (lock >= LOCKS))
  {
    *waliErr << "[ERROR] Invalid lock " << lock << endl;
    return false;
  }
  return true;
}

bool PhaseLH::check_q( int q )
{
  if (!is_initialized())
  {
    *waliErr << "[ERROR] PhaseLH not initialized" << endl;
    return false;
  }
  else if ((q < 0) || (q >= Q))
  {
    *waliErr << "[ERROR] Invalid q " << q << endl;
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& o, const PhaseLH& plh)
{
  plh.print(o);
  return o;
}

int PhaseLH::vidx_base(int base, int ply, int v)
{
  assert((0<=ply) && (ply < NUMPLYS));
  assert((0<=v)   && (v < PLYVARS));
#if INTERLEAVE
  return base + (NUMPLYS*v) + ply;
#else
  return base + (ply * PLYVARS) + v;
#endif
}

int PhaseLH::vidx(int ply, int v)
{
  return vidx_base(BASE,ply,v);
}

bdd PhaseLH::var(int ply, int v)
{
  int idx = vidx(ply,v);
  return bdd_ithvar(idx);
}

bdd PhaseLH::nvar(int ply, int v)
{
  int idx = vidx(ply,v);
  return bdd_nithvar(idx);
}

// When doing summarization, there are more
// plys so the assertions in vidx are bypassed
int PhaseLH::sidx(int ply, int v)
{
  assert( is_initialized() );
  const int SUMPLYS = 3 * PHASES;
  assert((0<=ply) && (ply < SUMPLYS));//(ply < NUMPLYS));
  assert((0<=v)   && (v < PLYVARS));
#if INTERLEAVE
  return BASE + (SUMPLYS*v) + ply;
#else
  return BASE + (ply * PLYVARS) + v;
#endif
}

bdd PhaseLH::svar(int ply, int v)
{
  int idx = sidx(ply,v);
  return bdd_ithvar(idx);
}

bdd PhaseLH::nsvar(int ply, int v)
{
  int idx = sidx(ply,v);
  return bdd_nithvar(idx);
}

/** @return the relation {} -> {} */
bdd PhaseLH::empty()
{
  assert( PhaseLH::is_initialized() );
  bdd R = bddtrue;
  for (int v = 0; v < PLYVARS ; v++)
  {
    R = R & nvar(0,v) & nvar(1,v);
  }
  return R;
}

/** @return the relation forall S -> S. I.e., \lam S.S */
bdd PhaseLH::identity()
{
  assert( PhaseLH::is_initialized() );
  bdd R = bddtrue;
  for (int v=0; v < PLYVARS; v++)
  {
    R = R & bdd_biimp( var(0,v),var(1,v) );
  }
  return R;
}

/* Return relation R[true/(b \in ply)] */
bdd PhaseLH::set(bdd R, int ply, int b)
{
  bdd x = var(ply,b);
  return bdd_exist(R,x) & x;
}

/* Return relation R[false/(b \in ply)] */
bdd PhaseLH::unset(bdd R, int ply, int b)
{
  return bdd_exist(R,var(ply,b)) & nvar(ply,b);
}

int PhaseLH::get_rh_start(int lock)
{
  return LH::get_rh_start(lock);
}

int PhaseLH::get_ah_start(int lock)
{
  return LH::get_ah_start(lock);
}

int PhaseLH::get_lock_in_L( int lock )
{
  return LH::get_lock_in_L(lock);
}

int PhaseLH::get_lock_in_R( int lock )
{
  return LH::get_lock_in_R(lock);
}

int PhaseLH::get_lock_in_U( int lock )
{
  return LH::get_lock_in_U(lock);
}

void PhaseLH::print_q( int ply, bdd r)
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
bdd PhaseLH::q_in_ply( int q, int ply )
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


void PhaseLH::printHandler(char* v, int size)
{
  std::string indent = "  ";
  for (int ply = 0 ; ply < (kludge+2) ; ply++)
  {
    cout << indent;
    LH::printPly(v,size,ply);
    cout << " -->\n";
    indent = indent + "  ";
  }
  cout << "    ------" << endl;
}

void PhaseLH::printPhaseLH(std::ostream& o , bdd R, bool subID)
{
  PhaseLH x(R,0);
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

bool PhaseLH::invariant_check( bdd X , int phaseX )
{
  if (DO_INV_CHECK)
  {
    // Must skip over phaseX + 2 plys.
    // For E0,E1,E2, there has been 1 phase change.
    // Need to start quantifying at ply 3.
    for (int ply = (phaseX+2); ply < NUMPLYS ; ply++)
    {
      bdd check = bdd_exist( X , ply_selectors[ply] );
      if (X != check)
        return false;
    }
    return true;
  }
  else
    return true;
}


void PhaseLH::print_r_compat(char* v, int size)
{
  std::stringstream ss;
  for (int ph = 0 ; ph < PHASES ; ph++)
  {
    for (int proc = 0; proc < N ; proc++)
    {
      ss << "p" << proc << ".q{";
      for (int bit = 0; bit < QVARS; bit++)
      {
        switch(v[cidx(N,proc,ph,bit)])
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
      ss << "}:";
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
          int offset = cidx(N,proc,ph,vnum);
          assert(offset < size);
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
        if ( (i == (LOCKS)) || (i == (2*LOCKS+1)) )
        {
          ss << "}] ";
        }
        else
        {
          ss << "} ";
        }
      }
      ss << "\n";
    }
  }
  *waliErr << "Compatible:\n-------------------------------------\n";
  *waliErr << ss.str();
  *waliErr << "-------------------------------------" << endl;
}

#define DBG_ALLOC 0
bdd PhaseLH::alloc_summarize_vars()
{
  assert( PhaseLH::is_initialized() );
  static int verify = 0;
  verify = verify + 1;
  assert( verify == 1 );

  int total_vars = 3 * PHASES * PLYVARS;
  *waliErr << "[INFO] Have #vars " << NUMVARS << endl;
  *waliErr << "[INFO] Need #vars " << total_vars << endl;
  int extra_vars = total_vars - NUMVARS;
  *waliErr << "[INFO] Diff #vars " << extra_vars << endl;
  if (extra_vars > 0)
    bdd_extvarnum(extra_vars);

  bdd R = bddtrue;
#if DBG_ALLOC
  *waliErr << "[all_a_b]\n";
#endif
  for (int p = 0 ; p < PHASES ; p++)
  {
    const int PlyA = p*3;
    const int PlyB = PlyA + 1;
#if DBG_ALLOC
    *waliErr << "  Phase : " << p << std::endl;
#endif
    for (int v = 0 ; v < PLYVARS ; v++)
    {
#if DBG_ALLOC
      *waliErr << "    a:" << sidx(PlyA,v);
      *waliErr << " , b:" << sidx(PlyB,v); 
      const int PlyC = PlyA + 2;
      *waliErr << " , _:" << sidx(PlyC,v) << std::endl;
#endif
      bdd Ra = svar(PlyA,v);
      bdd Rb = svar(PlyB,v);
      assert( Ra != Rb );
      R = R & Ra & Rb;
    }
  }
  return R;
}

bddPair* PhaseLH::make_unshift()
{
  assert( PhaseLH::is_initialized() );
  static int verify = 0;
  verify = verify + 1;
  assert( verify == 1 );

  bddPair* pair = bdd_newpair();
  //*waliErr << "[DS]\n";
  for( int p = 1 ; p <= PHASES ; p++)
  {
    //*waliErr << "  Phase : " << p << std::endl;
    const int FromPly = ((p-1)*3)+2;
    const int ToPly = p;
    for (int v = 0 ; v < PLYVARS ; v++)
    {
      const int From = sidx(FromPly,v); 
      const int To   = vidx(ToPly,v);
      //*waliErr << "    " << From << " ~ds~> " << To << std::endl;
      bdd_setpair(pair,From,To);
    }
  }
  return pair;
}

bdd* PhaseLH::make_phsum_checks()
{
  bdd* checks = new bdd[PHASES];

  for (int phase = 0 ; phase < PHASES ; phase++)
  {
    checks[phase] = make_phsum_for_phase(phase);
  }
  return checks;
}

#define DBG_PHSUM 0
bdd PhaseLH::make_phsum_for_phase(int phase)
{
  assert( is_initialized() );

  static bdd LHsum = LH::Summarizer();
  bddPair* pair = bdd_newpair();
  const int ToPly1 = (phase * 3);
  const int ToPly2 = (phase * 3)+1;
  const int ToPly3 = (phase * 3)+2;
  for (int v = 0 ; v < PLYVARS ; v++)
  {
#if DBG_PHSUM
    *waliErr << "    { ";
#endif
    {
      const int F1 = vidx(0,v);
      const int T1 = sidx(ToPly1,v);
      if (F1 != T1)
      {
#if DBG_PHSUM
        *waliErr << " 1:" << F1 << "~>" << T1 << " ; ";
#endif
        bdd_setpair(pair,F1,T1);
      }
    }
    {
      const int F2 = vidx(1,v);
      const int T2 = sidx(ToPly2,v);
      if (F2 != T2)
      {
#if DBG_PHSUM
        *waliErr << " 2:" << F2 << "~>" << T2 << " ; ";
#endif
        bdd_setpair(pair,F2,T2);
      }
    }
    {
      const int F3 = vidx(2,v);
      const int T3 = sidx(ToPly3,v);
      if (F3 != T3)
      {
#if DBG_PHSUM
        *waliErr << " 3:" << F3 << "~>" << T3 << " ; ";
#endif
        bdd_setpair(pair,F3,T3);
      }
    }
#if DBG_PHSUM
    *waliErr << " }\n";
#endif
  }
  bdd plysum = bdd_replace(LHsum,pair);
  bdd_freepair(pair);
  return plysum;

#if 0
  bdd PhSum = bddtrue;
  for (int p = 0 ; p < PHASES ; p++)
  {
    int plya = p*3;
    int plyb = plya+1;
    bdd tmp_checker = bddfalse;
    // Different states == invalid
    for (int qbit=0 ; qbit < QVARS ; qbit++)
    {
      tmp_checker = tmp_checker |
        (svar(plya,qbit) & nsvar(plyb,qbit));
      tmp_checker = tmp_checker |
        (nsvar(plya,qbit) & svar(plyb,qbit));
    }
    for (int i=0; i < LOCKS ; i++)
    {
      // Disjoint sets of currently held locks
      tmp_checker = tmp_checker | 
        (svar(plya,get_lock_in_L(i)) & // i \in L
         svar(plyb,get_lock_in_L(i))); // i \in L'

      // For P1, let I1 = { li | li \in L1 & AH[i] = {} }
      // Check that I1 \cap U2 = {}
      // Vice versa for P2/P1. 
      bdd held_by_P1_used_by_P2 = 
        svar(plya,get_lock_in_L(i))    &  // li \in L1
        nsvar(plya,get_ah_start(i)+i) &  // li \notin AH_1[i]
        svar(plyb,get_lock_in_U(i));     // li \in U2

      tmp_checker = tmp_checker | held_by_P1_used_by_P2;

      // Vice versa
      bdd held_by_P2_used_by_P1 =
        svar(plyb,get_lock_in_L(i))    &  // li \in L2
        nsvar( plyb,get_ah_start(i)+i) &  // li \notin AH_2[i]
        svar( plya,get_lock_in_U(i));     // li \in U1

      tmp_checker = tmp_checker | held_by_P2_used_by_P1;

      for (int j=0; j < LOCKS ; j++)
      {
        // ///////////////////////////
        //  Check AH sets
        // Select lock j in AH_i
        bdd i_in_Aj = svar(plya , get_ah_start(i)+j);

        // Select lock i in AH'_j
        bdd j_in_Api = svar(plyb , get_ah_start(j) + i);
        tmp_checker = tmp_checker | (i_in_Aj & j_in_Api);

        // ///////////////////////////
        //  Check RH sets
        // Select lock j in RH_i
        bdd i_in_Rj = svar(plya , get_rh_start(i) + j);

        // Select lock i in AH'_j
        bdd j_in_Rpi = svar(plyb , get_rh_start(j)+i);
        tmp_checker = tmp_checker | (i_in_Rj & j_in_Rpi);
      }
    }
    bdd sum = bdd_not(tmp_checker);
    // [plysum] is phase 2-PDS compatibility.
    // Now add in summarization.
    //   - [plyc] is the summary of [plya] and [plyb]
    int plyc = plya+2;
    for (int q = 0; q < Q ; q++)
    {
      sum = bdd_ite(
          (q_in_ply(q,plya) & q_in_ply(q,plyb)),
          (sum & q_in_ply(q,plyc)),
          sum);
    }
    for (int l = 0 ; l < LOCKS; l++)
    {
      // (1) l \in L1 & l \notin L2 => l \in L3
      // (2) l \notin L1 &  l \in L2    => l \in L3
      sum = bdd_ite(
          (
           (svar(plya,get_lock_in_L(l)) & nsvar(plyb,get_lock_in_L(l))) |
           (nsvar(plya,get_lock_in_L(l)) & svar(plyb,get_lock_in_L(l)))
          ),
          set(sum, plyc, get_lock_in_L(l)),
          unset(sum, plyc, get_lock_in_L(l)) 
          );

      // (3) r \in R1    &  r \notin R2 => r \in R3
      // (4) r \notin R1 &  r \in R2    => r \in R3
      sum = bdd_ite(
          (
           (svar(plya,get_lock_in_R(l)) & nsvar(plyb,get_lock_in_R(l))) |
           (nsvar(plya,get_lock_in_R(l)) & svar(plyb,get_lock_in_R(l)))
          ),
          set(sum, plyc, get_lock_in_R(l)),
          unset(sum, plyc, get_lock_in_R(l))
          );

      // (5) l \in U1    |  l \in U2    => l \in U3
      sum = bdd_ite(
          (svar(plya,get_lock_in_U(l)) | svar(plyb,get_lock_in_U(l))),
          set(sum, plyc, get_lock_in_U(l)),
          unset(sum, plyc, get_lock_in_U(l)) 
          );
      const int i = l;
      for (int j = 0 ; j < LOCKS ; j++)
      {
        { // (6) j \in AH1[i] | j \in AH2[i] => j \in AH3[j]
          bdd j_in_A1i = svar(plya , get_ah_start(i)+j);
          bdd j_in_A2i = svar(plyb , get_ah_start(i)+j);
          sum = bdd_ite(
              (j_in_A1i | j_in_A2i),
              set(sum, plyc, get_ah_start(i)+j),
              unset(sum, plyc, get_ah_start(i)+j)
              );
        }
        { // (7) j \in RH1[i] | j \in RH2[i] => j \in RH3[j]
          bdd j_in_R1i = svar(plya , get_rh_start(i)+j);
          bdd j_in_R2i = svar(plyb , get_rh_start(i)+j);
          sum = bdd_ite(
              (j_in_R1i | j_in_R2i),
              set(sum, plyc, get_rh_start(i)+j),
              unset(sum, plyc, get_rh_start(i)+j)
              );
        }
      }
      for (int j = 0 ; j < LOCKS ; j++)
      {
        // (8) j \in AH1[i] & h \in AH2[j] => h \in AH3[i]
        // (9) j \in RH1[i] & h \in RH2[j] => h \in RH3[i]
        bdd j_in_A1i = svar(plya,get_ah_start(i)+j);
        bdd j_in_R1i = svar(plya,get_rh_start(i)+j);
        for (int h = 0 ; h < LOCKS ; h++)
        {
          bdd h_in_A2j = svar(plyb,get_ah_start(j)+h);
          sum = bdd_ite(
              (j_in_A1i & h_in_A2j),
              set(sum, plyc, get_ah_start(i)+h),
              sum);
          bdd h_in_R2j = svar(plyb,get_rh_start(j)+h);
          sum = bdd_ite(
              (j_in_R1i & h_in_R2j),
              set(sum, plyc, get_rh_start(i)+h),
              sum);
        }
      }
    }
    PhSum = PhSum & sum;
  }
#endif

}

void PhaseLH::print_sum(char* v, int size)
{
  std::stringstream ss;
  std::string indent = "  ";
  for (int ph = 0 ; ph < PHASES ; ph++)
  {
    const int pbase = 3 *ph;
    for (int slot = 0; slot < 3 ; slot++)
    {
      ss << "p" << slot << ".q{";
      for (int bit = 0; bit < QVARS; bit++)
      {
        switch(v[sidx(pbase+slot,bit)])
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
      ss << "}:";
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
          int offset = sidx(pbase+slot,vnum);
          assert(offset < size);
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
        if ( (i == (LOCKS)) || (i == (2*LOCKS+1)) )
        {
          ss << "}] ";
        }
        else
        {
          ss << "} ";
        }
      }
      ss << "\n";
    }
    indent+= "  ";
  }
  *waliErr << "Summary:\n-------------------------------------\n";
  *waliErr << ss.str();
  *waliErr << "-------------------------------------" << endl;
}

// When doing compat checking, it is like
// having N phases, and Q plys.
// for 
//    q1q2q3 r1r2r3 h1h2h3 u1u2u3 a1a2a3 l1l2l3
//    (q1q2q3 r1r2r3 h1h2h3 u1u2u3 a1a2a3 l1l2l3)'
// checker is
//   q1q1'_ r1r1'_ h1h1'_ u1u1'_ a1a1'_ l1l1'_
// per phase.
// To get to the next phase.
//   phase index  = PLYVARS * N * phase.
//   var   offset = v*N + proc 
int PhaseLH::cidx(int the_N, int proc , int phase , int v)
{
  assert( (0<=v)      && (v < PLYVARS));
  assert( (0 <= proc) && (proc < the_N) );
  assert( (0 <= phase) && (phase < PHASES) );
  return 
    BASE + 
    (PLYVARS*the_N*phase) + // Skip to correct phase
    (the_N*v) + proc;     // handle interleaving
}

