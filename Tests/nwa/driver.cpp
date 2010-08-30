#include <iostream>
#include <boost/test/unit_test.hpp>

#include "wali/nwa/NWA.hpp"

#include "arbitrary.hpp"

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;
using Arbitrary::suite_of_random_tests;


/// Tests whether the argument is positive. Obviously this will fail a
/// lot of the time.
void test_positive(int i)
{
  BOOST_CHECK_LT(0, i);
}


/// This is the equivalent of "main"
test_suite* init_unit_test_suite(int, char** const)
{
  return suite_of_random_tests(test_positive);
}
