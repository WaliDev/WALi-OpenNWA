#include "wali/ShortestPathSemiring.hpp"

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
    extern wali::sem_elem_t dist0, dist1, dist2, dist3, dist6,
      dist10, dist11, dist12, dist20, dist21, dist22,
      dist30, dist31, dist40, dist50, dist60,
      semiring_zero;
  }
}

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
