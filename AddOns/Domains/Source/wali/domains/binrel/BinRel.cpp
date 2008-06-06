/**
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "wali/domains/binrel/BinRel.hpp"
#include <iostream>
#include <sstream>

using namespace wali::domains::binrel;

// ////////////////////////////
// static
int BinRel::getNumVars() 
{
  return NUMVARS;
}

bool BinRel::initialize(int numVars) 
{
  if (BinRel::NUMVARS == -1) {
    BinRel::NUMVARS = numVars;
    int domains[3] = {NUMVARS,NUMVARS,NUMVARS};
    base = fdd_extdomain( domains,3 );
    if (base < 0) {
      // err msg please
      throw base;
    }
    downOne = bdd_newpair();
    fdd_setpair(downOne, base,base+1);
    fdd_setpair(downOne, base+1,base+2);

    restore = bdd_newpair();
    fdd_setpair(restore, base+2, base+1);
    return true;
  }
  else {
    *wali::waliErr << "[WARNING] class BinRel is already initialized with [" << BinRel::NUMVARS << "] variables.\n";
    return false;
  }
}

binrel_t BinRel::Make(int from, int to) 
{
  if (from >= BinRel::NUMVARS) {
    std::stringstream ss;
    ss << "[ERROR] Out of range [0," << NUMVARS << ")  :  " << from << std::endl; 
    std::cerr << ss.str();
    throw ss.str();
  }
  if (to >= BinRel::NUMVARS) {
    std::stringstream ss;
    ss << "[ERROR] Out of range [0," << NUMVARS << ")  :  " << to << std::endl; 
    std::cerr << ss.str();
    throw ss.str();
  }
  binrel_t brel(new BinRel( fdd_ithvar(base,from) & fdd_ithvar(base+1,to) ));
  return brel;
}

binrel_t BinRel::Make(const std::list< std::pair<int,int> >& input) 
{
  std::list< std::pair<int,int> >::const_iterator it = input.begin();
  binrel_t result = Empty();
  for ( ; it != input.end() ; it++ ) {
    result = result->Union(Make(it->first,it->second));
  }
  return result;
}

binrel_t BinRel::Empty() 
{
  static binrel_t X(new BinRel(bddfalse));
  return X;
}

binrel_t BinRel::Id() 
{
  static binrel_t X( new BinRel(priv_id()) );
  return X;
}

// ////////////////////////////
// Friends
binrel_t operator*(binrel_t a, binrel_t b)
{
  return a->Compose(b);
}

binrel_t operator|(binrel_t a, binrel_t b)
{
  return a->Union(b);
}

binrel_t operator&(binrel_t a, binrel_t b)
{
  return a->Intersect(b);
}

// ////////////////////////////
// Members and Con/Destructors
BinRel::BinRel(const BinRel& that) : wali::SemElem(that),rel(that.rel) {}

BinRel::~BinRel() {}

binrel_t BinRel::Compose( binrel_t that ) 
{
  bdd c = bdd_relprod(rel, bdd_replace(that->rel,downOne),fdd_ithset(base+1));
  return new BinRel(bdd_replace(c,restore));
}

binrel_t BinRel::Union( binrel_t that ) 
{
  return new BinRel(rel | that->rel);
}

binrel_t BinRel::Intersect( binrel_t that )
{
  return new BinRel( rel & that->rel );
}

// Helper function that converts a SemElem
// into a BinRel*
static BinRel* convert(wali::SemElem* se) 
{
  BinRel* br = dynamic_cast<BinRel*>(se);
  if (br == NULL) {
    *wali::waliErr << "[ERROR] Cannot cast to class wali::binrel::BinRel.\n";
    se->print( *wali::waliErr << "    " ) << std::endl;
    assert(false);
  }
  // When done with developement
  // BinRel* br = static_cast<BinRel*>(se)
  return br;
}

wali::sem_elem_t BinRel::combine(wali::SemElem* se) 
{
  binrel_t that( convert(se) );
  return Union(that);
}

wali::sem_elem_t BinRel::extend(wali::SemElem* se) 
{
  binrel_t that( convert(se) );
  return Compose(that);
}

bool BinRel::equal(wali::SemElem* se) const 
{
  BinRel* br = convert(se);
  return rel == br->rel;
}

std::ostream& BinRel::print( std::ostream& o ) const 
{
  o << fddset << rel;
  return o;
}

//******************************************************
// Protected methods
BinRel::BinRel(bdd b) : rel(b) {}

int BinRel::NUMVARS = -1;
int BinRel::base    = -1;
bddPair* BinRel::downOne = NULL;
bddPair* BinRel::restore = NULL;

// BinRel.cpp local method
bdd BinRel::priv_id() 
{
  bdd id;
  for (int i = 0 ; i < BinRel::getNumVars() ; i++) {
    id = id | fdd_ithvar(base,i) & fdd_ithvar(base+1,i);
  }
  return id;
}

