#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/front.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>


namespace Arbitrary {
  /// "Returns" arbitrary objects of the first parameter type.
  void arbitrary(int & out, int n);


  using boost::unit_test_framework::test_suite;
  using boost::unit_test_framework::test_case;

  /// Creates and returns a test suite consisting of 'num' arbitrary tests, each of
  /// which calls the (unary) function 'func' with one of the randomly-generated arguments.
  ///
  /// The 'size' of the arbitrary objects is also given.
  template<typename Function>
  test_suite* suite_of_random_tests(Function func, int num = 10, int size = INT_MAX)
  {
    typedef typename boost::function_types::parameter_types<Function> parameters;
    typedef typename boost::mpl::front<parameters>::type argument_type;

    BOOST_STATIC_ASSERT(boost::function_types::function_arity<Function>::value == 1);

    test_suite* suite = BOOST_TEST_SUITE("Random tests");

    for (int i=0; i<num; ++i) {
      argument_type arb;
      arbitrary(arb, size);
      suite->add(BOOST_TEST_CASE(boost::bind(func, arb)));
    }

    return suite;
  }
}