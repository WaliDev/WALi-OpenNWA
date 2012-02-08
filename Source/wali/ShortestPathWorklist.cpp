/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ShortestPathWorklist.hpp"
#include "wali/ShortestPathSemiring.hpp"
#include "wali/wfa/Trans.hpp"
#include <climits>
#include <cstdlib>
#include <iostream>
#include <typeinfo>

namespace wali
{
  ShortestPathWorklist::ShortestPathWorklist() : RankedWorklist()
  {
  }

  ShortestPathWorklist::~ShortestPathWorklist()
  {
  }

  int ShortestPathWorklist::doRankOf( sem_elem_t a ) const
  {
    ShortestPathSemiring * p = dynamic_cast<ShortestPathSemiring*>(a.get_ptr());
    if (p == NULL) {
      std::cout << "Error: weight not a shortestpathsemiring. It is: ";
      if (a != NULL) {
        std::cout << typeid(*a).name() << "\n";
      }
      else {
        std::cout << "null\n";
      }
      std::exit(1);
    }
    
    unsigned int rank = p->getNum();
    if (rank > INT_MAX) {
      rank = INT_MAX;
    }
    return rank;
  }

}

