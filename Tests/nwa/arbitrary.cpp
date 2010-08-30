#include <cstdlib>


namespace Arbitrary {

// Return an int in the range [-n, n]
void arbitrary(int & out, int n)
{
  double x = static_cast<double>(std::rand()) / RAND_MAX * 2 * n  + 1.0 - n;
  out = static_cast<int>(x);
}

}