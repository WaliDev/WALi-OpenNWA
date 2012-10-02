#include "wali/LongestSaturatingPathSemiring.hpp"

namespace wali {

  namespace {
    unsigned int plus_saturate(unsigned int v1, unsigned int v2, unsigned biggest) {
      if(v1 == (unsigned int)(-1) || v2 == (unsigned int)(-1)) {
        return (unsigned int)(-1);
      }
      if (v1 + v2 < v1 || v1 + v2 < v2 || v1 + v2 > biggest) {
        return biggest;
      }
      return (v1+v2);
    }

    unsigned int max(unsigned int v1, unsigned int v2) {
      return (v1 < v2) ? v1 : v2;
    }
  }

  
  sem_elem_t LongestSaturatingPathSemiring::one() const
  {
    //static sem_elem_t elem = new LongestSaturatingPathSemiring(0);
    return new LongestSaturatingPathSemiring(0, this->biggest);
  }

  sem_elem_t LongestSaturatingPathSemiring::zero() const
  {
    //static sem_elem_t z = new LongestSaturatingPathSemiring((unsigned int)(-1));
    return new LongestSaturatingPathSemiring((unsigned int)(-1), this->biggest);
  }


  unsigned int LongestSaturatingPathSemiring::getNum() const
  {
    return v;
  }


  //---------------------------------
  // semiring operations
  //---------------------------------
  sem_elem_t LongestSaturatingPathSemiring::extend( SemElem* rhs )
  {
    assert(dynamic_cast<LongestSaturatingPathSemiring*>(rhs));
    
    LongestSaturatingPathSemiring* that = static_cast< LongestSaturatingPathSemiring* >(rhs);
    assert(this->biggest == that->biggest);
    return new LongestSaturatingPathSemiring(plus_saturate(v,that->v,biggest), biggest);
  }

  sem_elem_t LongestSaturatingPathSemiring::combine( SemElem* rhs )
  {
    assert(dynamic_cast<LongestSaturatingPathSemiring*>(rhs));
    
    LongestSaturatingPathSemiring* that = static_cast< LongestSaturatingPathSemiring* >(rhs);
    assert(this->biggest == that->biggest);    
    return new LongestSaturatingPathSemiring(max(v,that->v), biggest);
  }


  bool LongestSaturatingPathSemiring::equal(SemElem *rhs) const
  {
    assert(dynamic_cast<LongestSaturatingPathSemiring*>(rhs));

    LongestSaturatingPathSemiring* that = static_cast< LongestSaturatingPathSemiring* >(rhs);
    assert(this->biggest == that->biggest);    
    return v == that->v;
  }

  
  //------------------------------------
  // output
  //------------------------------------
  std::ostream & LongestSaturatingPathSemiring::print(std::ostream &out) const
  {
    out << "LongestSaturatingPathSemiring(" << v << ")";
    return out;
  }

}
