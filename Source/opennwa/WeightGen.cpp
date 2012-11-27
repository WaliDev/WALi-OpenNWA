#include "opennwa/WeightGen.hpp"

namespace opennwa {

  sem_elem_t
  ReachGen::getOne() const
  {
    static const Reach r(true);
    return r.one();
  }


  sem_elem_t
  ShortestPathGen::getOne() const
  {
    static const wali::ShortestPathSemiring r;
    return r.one();
  }
  

  sem_elem_t
  ShortestPathGen::getUnitWeight() const
  {
    static const sem_elem_t r = new wali::ShortestPathSemiring(1);
    return r;
  }


  sem_elem_t
  ShortestWordGen::getOne( ) const
  {
    static const wali::ShortestPathSemiring r;
    return r.one();
  }


  sem_elem_t
  ShortestWordGen::getUnitWeight() const
  {
    static const sem_elem_t r = new wali::ShortestPathSemiring(1);
    return r;
  }
  
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
