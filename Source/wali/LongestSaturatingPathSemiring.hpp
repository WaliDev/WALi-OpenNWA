#ifndef WALI_LONGESTSATURATING_PATH_SEMIRING_HPP
#define WALI_LONGESTSATURATING_PATH_SEMIRING_HPP

#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/Key.hpp"
#include <set>


namespace wali {


  /// This is a funny domain. But maybe it'll be useful? I just use it for
  /// testing.
  class LongestSaturatingPathSemiring : public wali::SemElem
  {
  public:

    //-----------------------------
    // semiring one and zero
    //-----------------------------
    sem_elem_t one() const;

    sem_elem_t zero() const;

    //---------------------------------
    // semiring operations
    //---------------------------------
    sem_elem_t extend( SemElem* rhs );

    sem_elem_t combine( SemElem* rhs );

    bool equal(SemElem *rhs) const;

    //------------------------------------
    // output
    //------------------------------------
    std::ostream & print(std::ostream &out) const;

    unsigned int getNum() const;

  private:
    
    unsigned int v;
    unsigned int biggest;

  public:
    //---------------------
    // Constructors 
    //---------------------
    LongestSaturatingPathSemiring(unsigned int big) : v(0), biggest(big) { }
    LongestSaturatingPathSemiring(unsigned int _v, unsigned int big) : v(_v), biggest(big) {}
  };



}

#endif // REACH_SEMIRING
