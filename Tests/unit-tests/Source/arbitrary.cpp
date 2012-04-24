#include <cstdlib>

#include "arbitrary.hpp"

namespace Arbitrary {

void arbitrary(bool & out, int n)
{
  int x;
  do {
    arbitrary(x, 1000);
  } while (x == 0);

  out = (x > 0);
}

// Return an int in the range [-n, n]
void arbitrary(int & out, int n)
{
  double x = static_cast<double>(std::rand()) / RAND_MAX * 2 * n  + 1.0 - n;
  out = static_cast<int>(x);
}

}