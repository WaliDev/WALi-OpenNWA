/**
 * @author Nicholas Kidd
 * @version $Id: SlidingEAH.cpp 612 2009-01-16 06:13:59Z kidd $
 */

#include "wali/Common.hpp"
#include "wali/util/Timer.hpp"
#include "wali/domains/eah/SlidingEAH.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iomanip>


using namespace wali::domains::eah;
using wali::waliErr;
using std::endl;
using std::cout;

bool SlidingEAH::DO_INV_CHECK              = true;
int SlidingEAH::PHASES                     = -1;
int SlidingEAH::NUMPLYS                    = -1;
int SlidingEAH::printKludge                = -1;
bool SlidingEAH::is_compat_initialized     = false;
bddPair* SlidingEAH::big_shift_left        = 0;
bddPair* SlidingEAH::big_shift_right       = 0;
bdd* SlidingEAH::mask_for_phaseX           = 0;
bdd* SlidingEAH::ply_selectors             = 0;
bddPair** SlidingEAH::from_ply_shift_pairs = 0;

///////////////////////////////////////////////////////////
// public static
bool SlidingEAH::is_initialized()
{
  return (EAH::is_initialized() && (PHASES != -1));
}

bool SlidingEAH::initialize( int locks , int phases, int Q )
{
  if (phases < 1) {
    *waliErr << "[ERROR] Must have at least one phase." << endl << endl;
    assert(false);
    return false;
  }
  assert( EAH::allocate(locks,Q) );
  PHASES = phases;
  // Set total number of plys to be 3 + (PHASES-1)
  // This allows for (PHASES-1) phase changes.
  bdd_extvarnum(EAH::PLYVARS * (PHASES-1));
  NUMPLYS = 2 + PHASES;
  // A bdd is of the form : {(PHASES-1)*S}DRS
  //   e.g., 3 phases has SSDRS
  // Set BASE so that actually have SSDRS
  EAH::NUMVARS = EAH::PLYVARS * NUMPLYS;
  EAH::BASE    = EAH::PLYVARS * (NUMPLYS - 3);

  // Now that EAH::BASE has been updated,
  // tell EAH to initialize the static vars.
  assert( EAH::init_vars() );
  cout << "[INFO] SlidingEAH. " << PHASES << " phases ;";
  cout << "  #NUMVARS = " << EAH::NUMVARS << ";";
  cout << "  #PLYVARS = " << EAH::PLYVARS << ";";
  cout << "  #NUMPLYS = " << NUMPLYS << endl;

  { // Create the big shift left pair
    bddPair* p = bdd_newpair();
    // first and last ply do not get shifted
    for (int ply = 1 ; ply < NUMPLYS ; ply++)
    {
      const int Ply_n = EAH::PLYVARS * ply;
      const int Ply_m = EAH::PLYVARS * (ply-1);
      for (int i=0 ; i < EAH::PLYVARS ; i++)
      {
        // PLY_n -> PLY_{m}
        //bdd_setpair( p  , Ply_n + i , Ply_m + i );
        bdd_setpair( p  , Ply_n + i , Ply_m + i );
      }
    }
    big_shift_left = p;
  }
  { // Create the big shift right pair
    bddPair* p = bdd_newpair();
    // last ply does not get shifted
    for (int ply = (NUMPLYS-1) ; ply > 0 ; ply--)
    {
      const int Ply_m = EAH::PLYVARS * (ply-1); // -1 for 0-based indexing
      const int Ply_n = EAH::PLYVARS * ply;
      for (int v = 0 ; v < EAH::PLYVARS ; v++)
      {
        // Ply_m -> Ply_n
        bdd_setpair( p , Ply_m + v , Ply_n + v );
      }
    }
    big_shift_right = p;
  }

  { // Create the mask_for_phaseX bdd
    bdd R = bddtrue;
    // Do not mask off the last 3 plys
    for (int ply = 0 ; ply < NUMPLYS-3 ; ply++)
    {
      const int Ply_i = ply * EAH::PLYVARS;
      for (int v = 0 ; v < EAH::PLYVARS; v++)
      {
        R = R & bdd_ithvar( Ply_i + v );
      }
    }
    mask_for_phaseX = new bdd(R);
  }
  { // Create the ply selectors
    ply_selectors = new bdd[NUMPLYS];
    for (int ply = 0 ; ply < NUMPLYS ; ply++)
    {
      const int Ply_i = EAH::PLYVARS * ply;
      bdd tmp = bddtrue;
      for (int v = 0 ; v < EAH::PLYVARS ; v++)
      {
        tmp = tmp & bdd_ithvar(Ply_i + v);
      }
      ply_selectors[ply] = tmp;
    }
  }
  { // Create the from_shift selectors
    from_ply_shift_pairs = new bddPair*[NUMPLYS]; 
    // cannot start from ply 0
    from_ply_shift_pairs[0] = NULL;
    for (int idx = 1 ; idx < NUMPLYS ; idx++)
    {
      bddPair* p = bdd_newpair();
      for (int ply = idx ; ply < NUMPLYS ; ply++)
      {
        const int Ply_n = ply * EAH::PLYVARS;     // from offset
        const int Ply_m = (ply-1) * EAH::PLYVARS; // to offset
        for (int v = 0 ; v < EAH::PLYVARS; v++)
        {
          bdd_setpair( p , Ply_n+v , Ply_m+v );
        }
      }
      from_ply_shift_pairs[idx] = p;
    }
  }
  return true;
}

SlidingEAH SlidingEAH::Empty()
{
  static SlidingEAH P( EAH::Empty().R , 0 );
  return P;
}

SlidingEAH SlidingEAH::Id()
{
  static SlidingEAH P( EAH::Id().R , 0 );
  return P;
}

SlidingEAH SlidingEAH::Null()
{
  static SlidingEAH P(  EAH::Null().R , 0 );
  return P;
}

SlidingEAH SlidingEAH::Acquire( int lock , int phase )
{
  // phase doesn't matter b/c of SSSDRS encoding.
  // Simply ensure that it is in a valid phase
  assert( (0 <= phase) && (phase < PHASES) );
  return SlidingEAH( EAH::Acquire(lock).R , 0 );
}

SlidingEAH SlidingEAH::Release( int lock , int phase )
{
  // phase doesn't matter b/c of SSSDRS encoding.
  // Simply ensure that it is in a valid phase
  assert( (0 <= phase) && (phase < PHASES) );
  return SlidingEAH( EAH::Release(lock).R , 0 );
}

SlidingEAH SlidingEAH::Transition( int from , int to )
{
  assert( (0 <= from) && ((from+1) == to) && (to < PHASES));
  // phase doesn't matter b/c of SSSDRS encoding.
  // Simply ensure that it is in a valid phase
  return SlidingEAH( EAH::Id().R , -1 );
}

bdd SlidingEAH::compat_init()
{
  assert( SlidingEAH::is_initialized() );
  // E.g., PHASES = 3 ==> NUMPLYS = 5
  //  o  {this->R} has form xyDRS.
  // We need to "shuffle" two {R} relations.
  // So, if R1 = abcdS \and R2 = wxyzS,
  // then we want: 
  //    aw bx cy dz
  // Thus, we need a total of:
  //    (PLYVARS*2      --- for the pairing (e.g., ab) 
  //     * (NUMPLYS-1)) --- ignore the Scratch plys
  int total_vars = (EAH::PLYVARS*2) * (NUMPLYS-1);
  //*waliErr << "*NEW* total vars : " << total_vars << endl;
  int extra_vars = total_vars - EAH::NUMVARS;
  //*waliErr << "         #extra  : " << extra_vars << endl;
  bdd_extvarnum(extra_vars);
  // checker will be of the form (0 , 0  ,  Compat, Compat, ... ) 
  bdd checker = bddtrue;

  // 1. initialize {\empty,\empty}
  for (int v =0; v < (EAH::PLYVARS*2); v++)
  {
    checker = checker & bdd_nithvar(v);
  }

  // 2. copy {EAH::R_checker} = (Compat,Compat),
  // and move (Compat,Compat) to starting positions (0,PLYVARS).
  // This requires a shift for each possible phase change,
  // which is PHASES - 1.
  bdd EAH_checker = *EAH::R_checker; // E.g., \eq xyCCS
  for (int phaseX = 1 ; phaseX < PHASES ; phaseX++)
  {
    EAH_checker = bdd_replace(EAH_checker,big_shift_left);
  }

  // 3. duplicate to make the C,C,C,C etc. checker.
  // Do this one time for each phase.
  const int PairedSize = EAH::PLYVARS * 2; // Num vars in one compat check.
  const int From_1     = 0;
  const int From_2     = EAH::PLYVARS;
  for (int p = 1 ; p <= PHASES ; p++) // start at 1 b/c of (\empty,\empty) start
  {
    //*waliErr << "PHASE " << p << endl;
    bddPair* pair    = bdd_newpair();
    const int C_1    = PairedSize * p;
    const int C_2    = C_1 + EAH::PLYVARS;
    for (int pos = 0 ; pos < EAH::PLYVARS ; pos++)
    {
      // Move C_1 into position
      //*waliErr << "  C_1 : " << (From_1+pos) << "  -->  " << (C_1+pos) << endl;
      bdd_setpair(pair, From_1 + pos , C_1 + pos);
      // Move C_2 into position
      //*waliErr << "  C_2 : " << (From_2+pos) << "  -->  " << (C_2+pos) << endl;
      bdd_setpair(pair, From_2 + pos , C_2 + pos); 
    }
    checker = checker & bdd_replace(EAH_checker,pair);
    bdd_freepair(pair);
  }
  return checker;
}

// TODO : initial ply can be removed b/c it must be \emptyset.
//        Would need to be reflected in compat_init()
bool SlidingEAH::Compatible( SlidingEAH t1, SlidingEAH t2 )
{
  // "static" enforces that compat_init() invoked only 1 time.
  // shuffle_checker = (0 , 0  ,  Compat, Compat, ... ) 
  static bdd shuffle_checker = compat_init();

  // abcdS___ -> a_b_c_d_
  bdd t1_shift = t1.R;
  // wxyzS___ -> _w_x_y_z
  bdd t2_shift = t2.R;
  const int PairedSize = EAH::PLYVARS * 2; // Num vars in one compat check.
  // Must shift PHASES+1 vars.
  // Think of it as init,p1,p2,p3 -- for 3 phases.

  {
    bddPair* t1pair = bdd_newpair();
    bddPair* t2pair = bdd_newpair();
    for (int p = PHASES ; p >= 0 ; p--)
    {
      //*waliErr << "PHASE " << p << endl;
      const int t1_base = EAH::PLYVARS * p;
      const int t1_tgt  = PairedSize   * p;

      const int t2_base = EAH::PLYVARS * p;
      const int t2_tgt  = (PairedSize   * p) + EAH::PLYVARS;
      for (int pos = 0 ; pos < EAH::PLYVARS ; pos++)
      {
        // Don't shift "from" of phase 0
        if (p != 0)
        {
          //*waliErr << "..T1  from : " << (t1_base+pos) << "  -->  " << (t1_tgt+pos) << endl;
          bdd_setpair( t1pair , t1_base+pos , t1_tgt+pos );
        }
        //*waliErr << "..T2  from : " << (t2_base+pos) << "  -->  " << (t2_tgt+pos) << endl;
        bdd_setpair( t2pair , t2_base+pos , t2_tgt+pos );
      }
    }
    t1_shift = bdd_replace( t1_shift, t1pair );
    t2_shift = bdd_replace( t2_shift, t2pair );
    bdd_freepair(t1pair);
    bdd_freepair(t2pair);
  }
  // for t1 = xyz...
  //     t2 = abc...
  // shuffle = (x , a  ,  y , b , z , c , ... )
  // 
  // shuffle_checker = (0 , 0  ,  Compat, Compat, ... ) 
  bdd result = (t1_shift & t2_shift) & shuffle_checker;
  return result != bddfalse;
}

bool SlidingEAH::Compatible( std::vector< SlidingEAH >& v )
{
  assert( SlidingEAH::is_initialized() );
  const int N = v.size();
  const int Q = PHASES;
  std::stringstream message;
  message << "SlidingEAH::Compatible(vector< SlidingEAH >)";
  message << " {N=" << N << " , Q=" << Q << "}.";
  *waliErr << "[INFO] " << message.str() << endl;
  //wali::util::Timer t(message.str());
  // E.g., Q = 3, N = 3
  //  o v[0].R has form xyDR_
  // Want to shuffle the SlidingEAH for each P_i
  // to get:
  //   1_1 -> ... _> 1_n -> 2_1...2_n -> 3_1 -> ... -> 3_n
  //
  // Total number of vars then is N * Q * EAH::PLYVARS
  int total_vars = N * Q * EAH::PLYVARS;
  *waliErr << "  #Total vars : " << total_vars << " , ";
  int extra_vars = total_vars - EAH::NUMVARS;
  *waliErr << "#extra  : " << extra_vars << endl;
  bdd_extvarnum(extra_vars);
  // checker will be of the form (C_11...C1n, ... , Cq1...Cqn )
  // where Cij comes from the EAH::Compatible relation.
  bdd checker = bddtrue;

  // 1. copy {EAH::R_checker} = (C,C),
  // and move (C,C) to starting positions (0,PLYVARS).
  // This requires a shift for each possible phase change,
  // which is PHASES - 1.
  bdd EAH_checker = *EAH::R_checker; // E.g., \eq __CCS
  for (int phaseX = 1 ; phaseX < PHASES ; phaseX++)
  {
    EAH_checker = bdd_replace(EAH_checker,big_shift_left);
  }

  // 3. Make the (C_11...C1n, ... , Cq1...Cqn )
  // 3a. Make the relation (C_11,...C_1n,__ DONT CARE )
  //     to check phase 1 for each process Pi,Pj
  bdd a3 = EAH_checker;
  for (int i = 0 ; i < N ; i++)
  {
    const int C_i = EAH::PLYVARS * i;
    // First shift C1j into position
    for (int j = i+1 ; j < N ; j++)
    {
      //*waliErr << "  -- Making (" << i << " , " << j << ")" << endl;
      if (i == 0 && j == 1) // (0,1) case defined by EAH_checker
        continue;
      const int C_j = EAH::PLYVARS * j; 
      bddPair* pair    = bdd_newpair();
      for (int pos=0 ; pos < EAH::PLYVARS ; pos++)
      {
        bdd_setpair(pair, EAH::PLYVARS + pos , C_j + pos);
        bdd_setpair(pair, pos , C_i + pos);
      }
      a3 = a3 & bdd_replace(EAH_checker,pair);
      bdd_freepair(pair);
    }
  }
  // 3b. Copy (C_11,...,C_1n, into the other phase positions
  const int PhaseCompatSize = EAH::PLYVARS * N;
  bdd b3 = a3;
  for (int phase = 1 ; phase < Q ; phase++)
  {
    bddPair* pair    = bdd_newpair();
    const int PhaseStart = PhaseCompatSize * phase;
    for (int pos = 0 ; pos < PhaseCompatSize ; pos++)
    {
      bdd_setpair(pair, pos , PhaseStart + pos);
    }
    b3 = b3 & bdd_replace(a3,pair);
    bdd_freepair(pair);
  }
  bdd rCompatible = b3;
  
  // 4. Interleave the vector of SlidingEAH's
  std::vector<bdd> fromStart;
  for (size_t idx = 0 ; idx < v.size() ; idx++)
  {
    SlidingEAH startEmpty = SlidingEAH::Empty() * v[idx];
    bdd quant_ply_1 = bdd_exist(startEmpty.R,ply_selectors[0]);
    { // DEBUGGING
      invariant_check( quant_ply_1, Q-1 );
    }
    fromStart.push_back( quant_ply_1 );
  }
  bdd shuffle = bddtrue;
  // E.g., Q = 3, N = 3
  //  proc = 0
  //      E1 _ _ E2 _ _ E3 _ _
  //  proc = 1
  //      _ E1 _ _ E2 _ _ E3 _
  //  proc = 1
  //      _ _ E1 _ _ E2 _ _ E3
  for (int proc = 0 ; proc < N ; proc++)
  {
    const int StartInSlot = proc * EAH::PLYVARS;
    //*waliErr << "  -- Shuffling proc P_" << proc << endl;
    bdd r_proc = fromStart[proc];
    bddPair* pair = bdd_newpair();
    for (int q = 1 ; q <= Q ; q++)
    {
      // Ignore E0 b/c it is {}
      const int TargetPhaseSlot = (q-1) * PhaseCompatSize;
      const int to_E_q          = TargetPhaseSlot + StartInSlot;
      const int from_E_q        = EAH::PLYVARS * q; 
      for (int pos = 0 ; pos < EAH::PLYVARS ; pos++)
      {
        const int From = from_E_q + pos;
        const int To   = to_E_q   + pos;
        if ( From != To ) // Happen for proc=1, q = 1
        { 
          //*waliErr << "    o " <<  From << " ~> " << To << endl;
          bdd_setpair(pair, From , To);
        }
      }
    }
    r_proc = bdd_replace( fromStart[proc], pair );
    bdd_freepair(pair);
    shuffle = shuffle & r_proc;
  }
  bdd Verify = rCompatible & shuffle;
  return (Verify != bddfalse);
}

///////////////////////////////////////////////////////////
// public member
SlidingEAH::SlidingEAH( const SlidingEAH& x ) : 
  R(x.R), fPhase(x.fPhase)
{
}

SlidingEAH::~SlidingEAH()
{
}

bool SlidingEAH::is_equal( const SlidingEAH& that ) const
{
  if (fPhase == that.fPhase)
  {
    return R == that.R;
  }
  else
  {
    return false;
  }
}

bool SlidingEAH::is_empty() const
{
  return is_equal(Empty());
}

bool SlidingEAH::is_id()  const
{
  return is_equal(Id());
}

bool SlidingEAH::is_null() const
{
  return (R == Null().R);
}

SlidingEAH SlidingEAH::operator|( const SlidingEAH& that) const
{
  if (fPhase != that.fPhase)
  {
    std::cout << "[ Error in SlidingEAH::operator| ]\n";
    print( std::cout << "THIS---\n" ) << std::endl;
    that.print( std::cout << "THAT---\n") << std::endl;
    assert( false );
  }
  bdd X = R | that.R;
  return SlidingEAH( X , fPhase );
}

SlidingEAH SlidingEAH::operator*( const SlidingEAH& that) const
{
  if (fPhase == -1)
  {
    *waliErr << "[ERROR] Phase transition on left of compose?" << std::endl;
    assert( false );
  }
  else if (that.fPhase == -1)
  {
    // Can have at most (PHASES-1) phase changes.
    assert( (fPhase + 1) < PHASES );
#if 0
    { // DEBUGGING
      *waliErr << "[INFO] Transition." << std::endl;
      *waliErr << "      - Phases = " << PHASES << std::endl;
      *waliErr << "      - Plys   = " << NUMPLYS << std::endl;
      *waliErr << "      - this X = " << fPhase << std::endl;
      *waliErr << "      - that X = " << that.fPhase << std::endl;
      assert( invariant_check( R , fPhase ) );
    }
#endif
    // E.g., from _rDRS -> rDRS_
    bdd go_left = bdd_replace( R , big_shift_left );
    { // DEBUGGING CODE
      if (DO_INV_CHECK) 
      { 
        assert( invariant_check( go_left , (fPhase + 1) ) );
        bdd check_last = bdd_exist(go_left,ply_selectors[NUMPLYS-1]);
        assert(check_last == go_left);
        bdd check_next_last = bdd_exist( go_left , ply_selectors[NUMPLYS-2] );
        assert( check_next_last == go_left );
      }
    }

    // E.g., from _rDRS -> __DRS
    bdd masked  = bdd_exist( R , *mask_for_phaseX );
    { // DEBUGGING CODE
      assert( invariant_check( masked , 0 ) );
    }

    // E.g., from __DRS -> __RTS
    EAH for_phaseX(masked);
    EAH phaseX = for_phaseX.Transition(1,2); // TODO NEW
    { // DEBUGGING CODE
      assert( invariant_check( phaseX.R , 0 ) );
    }

    // E.g., from rDRS_ & __RTS -> rDRTS
    bdd finish = go_left & phaseX.R;
    { // DEBUGGING CODE
      assert( invariant_check( finish , (fPhase+1) ) );
    }
    return SlidingEAH( finish , fPhase+1);
  }
  else if (that.fPhase == 0)
  {
    { // DEBUGGING CODE
      assert( invariant_check( that.R , 0 ) );
    }
    { // DEBUGGING CODE
      assert( invariant_check( R , fPhase ) );
    }
    // special case {that} has not seen a phase change
    bdd X = EAH::compose(R,that.R);
    { // DEBUGGING CODE
      assert( invariant_check( X , fPhase ) );
    }
    return SlidingEAH( X , fPhase );
  }
  else
  {
    // Can have at most (PHASES-1) phase changes.
    if ( (fPhase + that.fPhase) >= PHASES)
    {
      *wali::waliErr << "\nTOO MANY PHASE CHANGES FOR EXTEND!\n" << std::endl;
      assert( false );
    }
#if 0
    { // DEBUGGING
      *waliErr << "[INFO] Unbalanced mismatched path abstractions!" << std::endl;
      *waliErr << "      - Phases = " << PHASES << std::endl;
      *waliErr << "      - Plys   = " << NUMPLYS << std::endl;
      *waliErr << "      - this X = " << fPhase << std::endl;
      *waliErr << "      - that X = " << that.fPhase << std::endl;
      assert( invariant_check( R , fPhase ) );
      assert( invariant_check( that.R , that.fPhase ) );
    }
#endif

    // 1. Shift {that} one position
    //    E.g., _xyzS -> __xyz
    bdd shiftR_that = bddfalse;
    try {
      shiftR_that = bdd_replace(that.R,big_shift_right);
      { // DEBUGGING : Assert that [that] is correct after shifting right.
        for (int ply=0 ; ply < (NUMPLYS-3-that.fPhase+1) ; ply++)
        {
          bdd check = bdd_exist( shiftR_that , ply_selectors[ply] );
          assert( check == shiftR_that );
        }
      }
    }
    catch( int errcode )
    {
      *waliErr << __FILE__ << ":" << __LINE__ << " -- BAD!" << std::endl;
      *waliErr << "  this.fPhase == " << fPhase << std::endl;
      *waliErr << "  that.fPhase == " << that.fPhase << std::endl;
      that.prettyPrint(*waliErr << "  that? ") << std::endl;
      assert( false );
    }

    // 2. Shift {this} number of phase changes of {that}
    //    E.g., _abcS -> abc__
    bdd shiftL_this = R;
    for (int left = 0 ; left < that.fPhase; left++)
    {
      try {
        { // DEBUGGING CODE
          if (DO_INV_CHECK)
          {
            int fromPly = NUMPLYS - 3 - fPhase - left;
            assert( fromPly > 0 );
            for (int ply=0; ply < fromPly ; ply++)
            {
              bdd e = bdd_exist(shiftL_this , ply_selectors[ply]);
              if (e != shiftL_this)
              {
                *waliErr << __FILE__ << ":" << __LINE__ << " Not empty left of Slider.\n";
                *waliErr << "  # shifts    == " << left << std::endl;
                *waliErr << "  bad ply     == " << ply << std::endl;
                assert(false);
              }
            }
          }
        }
        shiftL_this = bdd_replace( shiftL_this , big_shift_left );
      } 
      catch (int errcode)
      {
        *waliErr << __FILE__ << ":" << __LINE__ << "[ERROR] BAD SHIFT LEFT!" << std::endl;
        *waliErr << "  this.fPhase == " << fPhase << std::endl;
        *waliErr << "  that.fPhase == " << that.fPhase << std::endl;
        *waliErr << "  # shifts    == " << left << std::endl;
        //prettyPrint(*waliErr << "  this? ") << std::endl;
        SlidingEAH bad(shiftL_this, left + fPhase );
        bad.prettyPrint(*waliErr << "bad?" ) << std::endl;
        assert(false);
      }
    }

    // 3. Logically & the shifted relations
    //    E.g., abc__ & __xyz -> abcy'z'
    //   -- Invariant: for abc__ and __xyz, c \subseteq x.
    //       o How to (should be) enforce(d)?
    //       o Should hold as paths start with ID relation!
    bdd anded = shiftL_this & shiftR_that;
    { // DEBUGGING CODE
      if (DO_INV_CHECK)
      { 
        for (int ply=0 ; ply < (NUMPLYS-3-fPhase-that.fPhase); ply++)
        {
          bdd check = bdd_exist( anded , ply_selectors[ply] );
          assert( check == anded );
        }
      }
    }

    // 4. Quantify out \anded relation.
    //    Ply to be quantified out is the first populated ply
    //    of {that} after it has be shifted right (i.e, {shiftR_that}).
    //    E.g., abcy'z' quantifies ply 2 --- 0-based indexing
    //    Is equal to ply "NUMPLYS - 2 - that.fPhase".
    //      o {NUMPLYS - 2} gets the "D" ply for {that}.
    //      o {- that.fPhase} to account for phase changes from {that}.
    const int cPly = (NUMPLYS - 2 - that.fPhase);
    { // DEBUGGING CODE
      assert ( (cPly > 0) && (cPly < (NUMPLYS-2)) );
    }
    bdd quantified = bdd_exist( anded, ply_selectors[cPly] );

    // 5. Shift right of quantification over.
    //    E.g., ab_y'z' -> aby'z'_
    bdd result = bdd_replace( quantified , from_ply_shift_pairs[cPly+1] );

    { // DEBUGGING CODE
      assert( invariant_check( result, (fPhase + that.fPhase) ) );
    }
    SlidingEAH slider( result , (fPhase + that.fPhase) );
    return slider;
  }

}

SlidingEAH SlidingEAH::operator&( const SlidingEAH& that) const
{
  assert( fPhase == that.fPhase );
  return SlidingEAH( (R & that.R), fPhase );
}

bool SlidingEAH::operator==( const SlidingEAH& that) const
{
  return is_equal(that);
}


void SlidingEAH::printHandler(char* v, int size)
{
  using namespace std;
  std::stringstream ss;
  assert(size == EAH::NUMVARS);

  // E.g., for xyDRS, EAH::BASE points to D
  // For each phase change, we need to back up
  // one set of plyvars.
  const int start = EAH::BASE - (EAH::PLYVARS * printKludge);
  assert( start >= 0 );
  // For each phase that has a value, print the ply
  // E.g., if printKludge = 1, then we have _yDRS,
  // and we need to print yDR. Thus, printKludge
  // is the number of *extra* plys that must be printed.
  std::string skip = "  ";
  for (int ply = 0 ; ply < (printKludge+2) ; ply++ )
  {
    // for R, RH_1 , ... , RH_{LOCKS} , U , AH_1 , ... , AH_{LOCKS} , L
    int plybegin = start + (ply * EAH::PLYVARS);
    ss << skip;
    // A ply has (3+2*|LOCKS|) sets of vars of size |LOCKS|
    for (int i = 0 ; i < (3+(2*EAH::LOCKS)) ; i++)
    {
      // for lock l \in [0,LOCKS)
      bool first = true;
      if ( (i == 1) || (i==(EAH::LOCKS+2)) )
      {
        ss << "[{";
      }
      else
      {
        ss << "{";
      }
      for (int l = 0; l < EAH::LOCKS ; l++)
      {
        int offset = plybegin + ((i * EAH::LOCKS) + l);
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
      if ( (i == (EAH::LOCKS)) || (i == (2*EAH::LOCKS+1)) )
      {
        ss << "}] ";
      }
      else
      {
        ss << "} ";
      }
    }
    ss << " -->\n";
    skip = skip + "  ";
  }
  cout << ss.str();
  cout << "   ----\n";
}

std::ostream& SlidingEAH::prettyPrint( std::ostream& o ) const
{
  assert( fPhase < PHASES );
  o << "SlidingEAH[ " << fPhase << " ] ";
  if (fPhase == -1)
  {
    o << "Phase Change";
  }
  else if (R == Id().R)
  {
    o << "Id";
  }
  else if (R == Empty().R)
  {
    o << "Empty";
  }
  else if (R == Null().R)
  {
    o << "Null";
  }
  else
  {
    bool found = false;
    for (int l = 0 ; l < EAH::LOCKS ; l++)
    {
      if (R == Acquire(l,0).R)
      {
        found = true;
        o << "  Acq(" << (char)('a'+l) << ")";
        break;
      }
      else if (R == Release(l,0).R)
      {
        found = true;
        o << "  Rel(" << (char)('a'+l) << ")";
        break;
      }
    }
    if (!found)
    {
      o << "\n";
      printKludge = fPhase;
      bdd_allsat(R,printHandler);
      printKludge = -1;
    }
  }
  o << std::endl;
  return o;
}

std::ostream& SlidingEAH::print( std::ostream& o ) const
{
  assert( fPhase < PHASES );
  o << fPhase << "-";
  if (fPhase == -1)
  {
    o << "X";
  }
  else if (R == Id().R)
  {
    o << "I";
  }
  else if (R == Empty().R)
  {
    o << "E";
  }
  else if (R == Null().R)
  {
    o << "N";
  }
  else
  {
    bool found = false;
    for (int l = 0 ; l < EAH::LOCKS ; l++)
    {
      if (R == Acquire(l,0).R)
      {
        found = true;
        o << "  A_" << (char)('a'+l);
        break;
      }
      else if (R == Release(l,0).R)
      {
        found = true;
        o << "  R_" << (char)('a'+l);
        break;
      }
    }
    if (!found)
    {
      o << "O";
    }
  }
  return o;
}

///////////////////////////////////////////////////////////
// private
bool SlidingEAH::invariant_check( bdd X , int phaseX )
{
  if (DO_INV_CHECK)
  {
    for (int ply=0 ; ply < (NUMPLYS - 3 - phaseX) ; ply++)
    {
      bdd check = bdd_exist( X , ply_selectors[ply] );
      if (X != check )
        return false;
    }
    bdd check = bdd_exist( X , ply_selectors[NUMPLYS-1] );
    return (check == X) ? true : false;
  }
  else
  {
    return true;
  }
}

SlidingEAH::SlidingEAH( bdd R , int p ) :
  R(R), fPhase(p)
{
  assert( fPhase < PHASES || fPhase == -1 );
  { // DEBUGGING
    invariant_check( R , fPhase );
  }
}

