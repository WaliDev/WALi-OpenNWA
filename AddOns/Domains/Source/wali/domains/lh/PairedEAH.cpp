/**
 * @author Nicholas Kidd
 * @version $Id: PairedEAH.cpp 557 2008-12-17 15:55:34Z kidd $
 */

#include "wali/Common.hpp"
#include "wali/domains/eah/PairedEAH.hpp"
#include <iostream>
#include <cassert>

using namespace wali::domains::eah;
using wali::waliErr;
using std::endl;
using std::cout;

int PairedEAH::PHASES                 = -1;
bool PairedEAH::is_compat_initialized = false;

///////////////////////////////////////////////////////////
// public static
bool PairedEAH::is_initialized()
{
  return (EAH::is_initialized() && (PHASES != -1));
}

bool PairedEAH::initialize( int locks , int phases )
{
  if (phases < 1) {
    *waliErr << "[ERROR] Must have at least one phase." << endl << endl;
    assert(false);
    return false;
  }
  assert( EAH::initialize(locks) );
  PHASES = phases;
  return true;
}

PairedEAH PairedEAH::Empty()
{
  static PairedEAH P( std::vector<EAH>( PHASES , EAH::Empty() ) );
  return P;
}

PairedEAH PairedEAH::Id()
{
  static PairedEAH P(  std::vector<EAH>( PHASES , EAH::Id() ) );
  return P;
}

PairedEAH PairedEAH::Null()
{
  static PairedEAH P(  std::vector<EAH>( PHASES , EAH::Null() ) );
  return P;
}

PairedEAH PairedEAH::Acquire( int lock , int phase )
{
  assert( (0 <= phase) && (phase < PHASES) );
  std::vector<EAH> vec( PHASES , EAH::Id() );
  vec[phase] = EAH::Acquire(lock);
  return PairedEAH( vec , phase , phase );
}

PairedEAH PairedEAH::Release( int lock , int phase )
{
  assert( (0 <= phase) && (phase < PHASES) );
  std::vector<EAH> vec( PHASES , EAH::Id() );
  vec[phase] = EAH::Release(lock);
  return PairedEAH( vec , phase , phase );
}

PairedEAH PairedEAH::Transition( int from , int to )
{
  assert( (0 <= from) && ((from+1) == to) && (to < PHASES));
  std::vector<EAH> vec( PHASES, EAH::Id() );
  return PairedEAH( vec , from , to );
}

bdd PairedEAH::compat_init()
{
  int total_vars = (PHASES+1) * EAH::PLYVARS * 2;
  *waliErr << "TOTAL VARS : " << total_vars << endl;
  int extra_vars = total_vars - EAH::NUMVARS;
  *waliErr << "EXTRA VARS : " << extra_vars << endl;
  bdd_extvarnum(extra_vars);
  // checker= {0 , 0  ,  Compat, Compat, ... } 
  bdd checker     = EAH::Empty().R;
  bdd EAH_checker = *EAH::R_checker;
  for (int p = 1 ; p <= PHASES ; p++)
  {
    //*waliErr << "PHASE " << p << endl;
    bddPair* pair = bdd_newpair();
    int start = EAH::PLYVARS * 2 * p;
    for (int pos = 0 ; pos < EAH::PLYVARS ; pos++)
    {
      //*waliErr << "  from : " << pos << "  -->  " << (start+pos) << endl;
      bdd_setpair(pair, pos                , start+pos);
      //*waliErr << "    to : " << EAH::PLYVARS+pos << "  -->  " << (start+pos+EAH::PLYVARS) << endl;
      bdd_setpair(pair, EAH::PLYVARS + pos , start + pos + EAH::PLYVARS); 
    }
    checker = checker & bdd_replace(EAH_checker,pair);
    bdd_freepair(pair);
  }
  return checker;
}

bool PairedEAH::Compatible( PairedEAH t1, PairedEAH t2 )
{
  assert( PairedEAH::is_initialized() );
  // Enforces that compat_init() invoked only 1 time
  static bdd tensor_checker = compat_init();
  bdd tensor = bddtrue;
  for (int p = 0 ; p < PHASES ; p++)
  {
    //*waliErr << "PHASE " << p << endl;
    {
      bddPair* pair = bdd_newpair();
      int from = EAH::PLYVARS * 2 * p;
      int to   = EAH::PLYVARS * 2 * (p+1);
      for (int pos = 0 ; pos < EAH::PLYVARS ; pos++)
      {
        // Don't shift "from" of phase 0
        if (p != 0)
        {
          //*waliErr << "  from : " << pos << "  -->  " << (from+pos) << endl;
          bdd_setpair( pair , pos         , (from+pos) );
        }
        //*waliErr << "    to : " << EAH::PLYVARS+pos << "  -->  " << (to+pos) << endl;
        bdd_setpair(   pair,  EAH::PLYVARS+pos , (to+pos)   );
      }
      EAH e1 = t1.v[p];
      tensor = tensor & bdd_replace( e1.R, pair );
      bdd_freepair(pair);
    }
    {
      bddPair* pair = bdd_newpair();
      int from = EAH::PLYVARS * ((2 * p)+1);
      int to   = EAH::PLYVARS * ((2 * (p+1))+1);

      for (int pos = 0 ; pos < EAH::PLYVARS ; pos++)
      {
        //*waliErr << "  from : " << pos << "  -->  " << (from+pos) << endl;
        bdd_setpair( pair , pos         , (from+pos) );
        //*waliErr << "    to : " << EAH::PLYVARS+pos << "  -->  " << (to+pos) << endl;
        bdd_setpair(   pair,  EAH::PLYVARS+pos , (to+pos)   );
      }

      EAH e2 = t2.v[p];
      tensor = tensor & bdd_replace( e2.R, pair );
      bdd_freepair(pair);
    }
  }
  // for t1 = {x,y} , {y,z} , ...
  //     t2 = {a,b} , {b,c} , ...
  // tensor = {x , a  ,  y , b , z , c , ... }
  // 
  // tensor_checker= {0 , 0  ,  Compat, Compat, ... } 
  bdd result = tensor & tensor_checker;
  return result != bddfalse;
}

///////////////////////////////////////////////////////////
// public member
PairedEAH::PairedEAH( const PairedEAH& P ) : v(P.v), from(P.from), to(P.to)
{
}

PairedEAH::~PairedEAH()
{
}

bool PairedEAH::is_equal( const PairedEAH& that) const
{
  if ((from == that.from) && (to == that.to))
  {
    for (int p = 0 ; p < PHASES ; p++)
    {
      if (!(v[p] == that.v[p]))
        return false;
    }
    return true;
  }
  else
    return false;
}

bool PairedEAH::is_empty() const
{
  return is_equal(Empty());
}

bool PairedEAH::is_id()  const
{
  return is_equal(Id());
}

bool PairedEAH::is_null() const
{
  return is_equal(Null());
}

PairedEAH PairedEAH::operator|( const PairedEAH& that) const
{
  std::vector<EAH> vec;
  for (int p = 0 ; p < PHASES ; p++)
  {
    vec.push_back( v[p] | that.v[p] );
  }
  return PairedEAH(vec);
}

PairedEAH PairedEAH::operator*( const PairedEAH& that) const
{
  // Base Case = all transitions (-1 for from/to)
  if (from == -1)
  {
    if  (that.from == -1 || (that.from == that.to))
    {
      std::vector<EAH> vec;
      for (int p = 0 ; p < PHASES ; p++)
      {
        vec.push_back( v[p] * that.v[p] );
      }
      return PairedEAH(vec);
    }
    else
    {
      std::vector<EAH> vec(v);
      //cout << "!!! " << that.from << "  -->  " << that.to << endl;
      vec[that.to] = v[that.from].Transition();
      return PairedEAH(vec);
    }
  }
  else 
  {
    if (from == that.from) 
    {
      if (to == that.to) 
      {
        std::vector<EAH> vec;
        for (int p = 0 ; p < PHASES ; p++)
        {
          vec.push_back( v[p] * that.v[p] );
        }
        return PairedEAH(vec);
      }
      else if (to+1 == that.to)
      {
        // can this happen?
        // Yes :: Acquire(l,0) * Transition(0,1)
        std::vector<EAH> vec(v);
        //cout << "!!! " << that.from << "  -->  " << that.to << endl;
        vec[that.to] = v[that.from].Transition();
        return PairedEAH(vec);
        //assert(false);
        //return Null();
      }
      else
      {
        // This cannot this happen?
        assert(false);
        return Null();
      }
    }
    else 
    {
      // TODO - can this happen?
      assert(false);
      return Null();
    }
  }
}

PairedEAH PairedEAH::operator&( const PairedEAH& that) const
{
  std::vector<EAH> vec;
  for (int p = 0 ; p < PHASES ; p++)
  {
    vec.push_back( v[p] & that.v[p] );
  }
  return PairedEAH(vec);
}

bool PairedEAH::operator==( const PairedEAH& that) const
{
  return is_equal(that);
}

std::ostream& PairedEAH::print( std::ostream& o ) const
{
  o << "PairedEAH[ " << from << " , " << to << " ] ";
  for (int p = 0 ; p < PHASES ; p++)
  {
    o << ">> Phase " << p << endl;
    v[p].print(o);
  }
  return o;
}

///////////////////////////////////////////////////////////
// private
PairedEAH::PairedEAH( const std::vector<EAH>& v , int f, int t) : 
  v(v), from(f), to(t)
{
  assert( (from < PHASES) && (to < PHASES));
  // Invariants
  if (from < 0) 
  {
      assert( (from == -1) && (to == -1) );
  }
  else
  {
    assert( (from == to) || (from+1 == to) );
  }
}

