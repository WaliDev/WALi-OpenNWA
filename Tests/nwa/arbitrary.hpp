#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/front.hpp>
#include <boost/bind.hpp>


namespace Arbitrary {
  /// "Returns" arbitrary objects of the first parameter type.
  void arbitrary(int & out, int n);
  void arbitrary(bool & out, int /*ignored*/);


  using boost::unit_test_framework::test_suite;
  using boost::unit_test_framework::test_case;

  /// Given the same arguments as 'suite_of_random_tests' and a single arbitrary item,
  /// return a test suite to run. By default, that "suite" is really just a single case.
  template<typename Function, typename Argument>
  test_suite* default_builder(Function func, Argument const & arg)
  {
    test_suite * suite = BOOST_TEST_SUITE("Random");
    suite->add(BOOST_TEST_CASE(boost::bind(func, arg)));
    return suite;
  }


  /// This is a blindingly simple template metaprogram that, given the type of a callable
  /// (function, function pointer, functor), returns the type of the first (and only)
  /// argument.
  template<typename Function>
  struct first_arg {
    typedef typename boost::function_types::parameter_types<Function> parameters;
    typedef typename boost::mpl::front<parameters>::type argument_type;
    BOOST_STATIC_ASSERT(boost::function_types::function_arity<Function>::value == 1);

    typedef argument_type type;
  };

  /// Creates and returns a test suite consisting of 'num' arbitrary tests, each of
  /// which calls the (unary) function 'func' with one of the randomly-generated arguments.
  ///
  /// The 'size' of the arbitrary objects is also given.
  template<typename Function>
  test_suite* suite_of_random_tests(Function func,
                                    int num = 10,
                                    int size = INT_MAX)
  {
    return suite_of_random_tests(func, num, size, default_builder<Function, typename first_arg<Function>::type>);
  }

  template<typename Function, typename TestCaseBuilder>
  test_suite* suite_of_random_tests(Function func,
                                    int num,
                                    int size,
                                    TestCaseBuilder builder)
  {
    typedef typename first_arg<Function>::type argument_type;

    test_suite* suite = BOOST_TEST_SUITE("Random tests");

    for (int i=0; i<num; ++i) {
      argument_type arb;
      arbitrary(arb, size);
      suite->add(builder(func, arb));
    }

    return suite;
  }
}