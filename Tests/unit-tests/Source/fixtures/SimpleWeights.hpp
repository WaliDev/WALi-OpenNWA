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
  ///
  /// The "d" stands for "distance" and both distinguishes "distance 0" from
  /// semiring 0 (and distance 1 from semiring 1) and makes it a legal
  /// identifier. :-)
  struct ShortestPathWeights
  {
    wali::sem_elem_t d0, d1, d2, d3, d6,
      d10, d11, d12, d20, d21, d22,
      d30, d31, d40, d50, d60,
      semiring_zero;

    ShortestPathWeights()
      : d0 (new wali::ShortestPathSemiring(0))
      , d1 (new wali::ShortestPathSemiring(1))
      , d2 (new wali::ShortestPathSemiring(2))
      , d3 (new wali::ShortestPathSemiring(3))
      , d6 (new wali::ShortestPathSemiring(6))
      , d10(new wali::ShortestPathSemiring(10))
      , d11(new wali::ShortestPathSemiring(11))
      , d12(new wali::ShortestPathSemiring(12))
      , d20(new wali::ShortestPathSemiring(20))
      , d21(new wali::ShortestPathSemiring(21))
      , d22(new wali::ShortestPathSemiring(22))
      , d30(new wali::ShortestPathSemiring(30))
      , d31(new wali::ShortestPathSemiring(31))
      , d40(new wali::ShortestPathSemiring(40))
      , d50(new wali::ShortestPathSemiring(50))
      , d60(new wali::ShortestPathSemiring(60))
      , semiring_zero(d0->zero())
    {}
  };

}

// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
