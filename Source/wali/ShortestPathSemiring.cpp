#include "wali/ShortestPathSemiring.hpp"

namespace wali {

  namespace {
    unsigned int plus(unsigned int v1, unsigned int v2) {
      if(v1 == (unsigned int)(-1) || v2 == (unsigned int)(-1)) {
        return (unsigned int)(-1);
      }
      return (v1+v2);
    }

    unsigned int min(unsigned int v1, unsigned int v2) {
      return (v1 < v2) ? v1 : v2;
    }
  }

  
  sem_elem_t ShortestPathSemiring::one() const
  {
    static sem_elem_t elem = new ShortestPathSemiring(0);
    return elem;
  }

  sem_elem_t ShortestPathSemiring::zero() const
  {
    static sem_elem_t z = new ShortestPathSemiring((unsigned int)(-1));
    return z;
  }


  unsigned int ShortestPathSemiring::getNum() const
  {
    return v;
  }


  //---------------------------------
  // semiring operations
  //---------------------------------
  sem_elem_t ShortestPathSemiring::extend( SemElem* rhs )
  {
    assert(dynamic_cast<ShortestPathSemiring*>(rhs));
    
    ShortestPathSemiring* that = static_cast< ShortestPathSemiring* >(rhs);
    return new ShortestPathSemiring(plus(v,that->v));
  }

  sem_elem_t ShortestPathSemiring::combine( SemElem* rhs )
  {
    assert(dynamic_cast<ShortestPathSemiring*>(rhs));
    
    ShortestPathSemiring* that = static_cast< ShortestPathSemiring* >(rhs); 
    return new ShortestPathSemiring(min(v,that->v));
  }


  bool ShortestPathSemiring::equal(SemElem *rhs) const
  {
    assert(dynamic_cast<ShortestPathSemiring*>(rhs));

    ShortestPathSemiring* that = static_cast< ShortestPathSemiring* >(rhs);
    return v == that->v;
  }

  
  //------------------------------------
  // output
  //------------------------------------
  std::ostream & ShortestPathSemiring::print(std::ostream &out) const
  {
    out << "ShortestPathSemiring(" << v << ")";
    return out;
  }

}
