#include "SimpleWeights.hpp"

using wali::ShortestPathSemiring;

namespace testing
{
  /// Various shortest path lengths.
  ///
  /// The exact set of weights present isn't meaningful, just what the tests
  /// I have now have used. (Often I'll do something where along one path
  /// I'll use a single-digit distance and along another path I'll use a
  /// double-digit distance, or I'll extend a single and double digit
  /// distance, to make combinations of them easier.)
  namespace ShortestPathWeights
  {
    wali::sem_elem_t dist0(new ShortestPathSemiring(0)),
      dist1(new ShortestPathSemiring(1)),
      dist2(new ShortestPathSemiring(2)),
      dist3(new ShortestPathSemiring(3)),
      dist6(new ShortestPathSemiring(6)),
      dist10(new ShortestPathSemiring(10)),
      dist11(new ShortestPathSemiring(11)),
      dist12(new ShortestPathSemiring(12)),
      dist20(new ShortestPathSemiring(20)),
      dist21(new ShortestPathSemiring(21)),
      dist22(new ShortestPathSemiring(22)),
      dist30(new ShortestPathSemiring(30)),
      dist31(new ShortestPathSemiring(31)),
      dist40(new ShortestPathSemiring(40)),
      dist50(new ShortestPathSemiring(50)),
      dist60(new ShortestPathSemiring(60)),
      semiring_zero = dist1->zero();
  }

  namespace ReachWeights
  {
    wali::sem_elem_t reach_dummy(new Reach(true)),
      reachOne = reach_dummy->one(),
      reachZero = reach_dummy->zero();
  }
}

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
