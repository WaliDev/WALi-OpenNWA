#include <iostream>

#include "boost/test/unit_test.hpp"

#include "wali/nwa/NWA.hpp"

using namespace std;
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;


/// This will test stupid things; it illustrates how to write tests,
/// at least a little. (Yes, the second test should fail.)
struct example_test
{
  void does_one_plus_one_equal_two()
  {
    BOOST_CHECK_EQUAL(1+1, 2);
  }

  void does_one_minus_one_equal_one()
  {
    BOOST_CHECK_EQUAL(1-1, 1);
  }
};


/// This is a test suite
struct example_suite : public test_suite
{
  example_suite()
    : test_suite("Sample test suite")
  {
    boost::shared_ptr<example_test> tests(new example_test());

    test_case* good = BOOST_CLASS_TEST_CASE(&example_test::does_one_plus_one_equal_two, tests);
    test_case* bad = BOOST_CLASS_TEST_CASE(&example_test::does_one_minus_one_equal_one, tests);

    add(good);
    add(bad);
  }
};


/// This is the equivalent of "main"
test_suite* init_unit_test_suite(int, char** const)
{
  test_suite* suite = BOOST_TEST_SUITE("Suite");
  
  suite->add(new example_suite());

  return suite;
}
