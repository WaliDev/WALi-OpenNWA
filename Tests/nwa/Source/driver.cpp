#include <iostream>
#include <sstream>
#include <vector>

#if defined(_MSC_VER) && defined(_M_X64)
#  include <boost/test/included/unit_test.hpp>
#else
#  include <boost/test/unit_test.hpp>
#endif

#include "opennwa/Nwa.hpp"

#include "arbitrary.hpp"

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;
using Arbitrary::suite_of_random_tests;

typedef opennwa::NWA<opennwa::ClientInfo> Nwa;
typedef wali::ref_ptr<Nwa> NwaRefPtr;


namespace Arbitrary {
  void arbitrary(NwaRefPtr & nwa, int n)
  {
    assert(nwa == 0);

    int num_states;
    arbitrary(num_states, n);
    num_states = abs(num_states);
    std::vector<wali::Key> state_keys(num_states);
    nwa = new Nwa(wali::getKey("stuck"));
    for (int i=0; i<num_states; ++i) {
      std::stringstream ss;
      ss << "state #" << i;
      wali::Key key = wali::getKey(ss.str());
      nwa->addState(key);
      state_keys[i] = key;
    }

    int num_syms;
    arbitrary(num_syms, n/2);
    num_syms = abs(num_syms);
    std::vector<wali::Key> sym_keys(num_syms);
    for (int i=0; i<num_syms; ++i) {
      std::stringstream ss;
      ss << "symbol #" << i;
      wali::Key key = wali::getKey(ss.str());
      nwa->addSymbol(key);
      sym_keys[i] = key;
    }

    // Internals
    for (int i=0; i<num_states; ++i) {
      for (int j=0; j<num_states; ++j) {
        for (int k=0; k<num_syms; ++k) {
          bool inc;
          arbitrary(inc, 0);

          if (inc) {
            nwa->addInternalTrans(state_keys[i], sym_keys[k], state_keys[j]);
          }
        }
      }
    }

    // Calls
    for (int i=0; i<num_states; ++i) {
      for (int j=0; j<num_states; ++j) {
        for (int k=0; k<num_syms; ++k) {
          bool inc;
          arbitrary(inc, 0);

          if (inc) {
            nwa->addCallTrans(state_keys[i], sym_keys[k], state_keys[j]);
          }
        }
      }
    }

    // Returns
    for (int i=0; i<num_states; ++i) {
      for (int j=0; j<num_states; ++j) {
        for (int k=0; k<num_states; ++k) {
          for (int l=0; l<num_syms; ++l) {
            bool inc;
            arbitrary(inc, 0);

            if (inc) {
              nwa->addReturnTrans(state_keys[i], state_keys[j], sym_keys[l], state_keys[k]);
            }
          }
        }
      }
    }
  }
}


/// Tests whether the argument is positive. Obviously this will fail a
/// lot of the time.
void test_positive(int i)
{
  BOOST_CHECK_LT(0, i);
}


void test_nwa(NwaRefPtr p)
{
  BOOST_CHECK_EQUAL(p->sizeStates(), p->num_nwa_states());
}


void test_intersection(NwaRefPtr left, NwaRefPtr right, NwaRefPtr intersection, Nwa::NestedWord const & word)
{
  bool inLeft = left->isMemberNondet(word);
  bool inRight = right->isMemberNondet(word);
  bool inInter = intersection->isMemberNondet(word);

  if (inLeft && inRight) {
    BOOST_CHECK(inInter);
  }
  else {
    BOOST_CHECK(!inInter);
  }
}


template<typename Function>
test_suite* binop_test_suite_given_one_nwa(Function func, int n, int size, NwaRefPtr left)
{
  NwaRefPtr right;
  arbitrary(right, size);

  NwaRefPtr intersection = Nwa::intersect(left, right);

  return suite_of_random_tests(boost::bind(test_intersection, left, right, intersection, _1),
                               n, size);
}


/// This is the equivalent of "main"
test_suite* init_unit_test_suite(int, char** const)
{
  return suite_of_random_tests(test_nwa, 20, 10);
}
