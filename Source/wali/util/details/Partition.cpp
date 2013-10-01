#include "Partition.hpp"


namespace wali
{
namespace util
{
namespace details
{
  
  template<class T>
  void unordered_set_intersection(const boost::unordered_set<T> &first, 
				  const boost::unordered_set<T> &second,
				  boost::unordered_set<T> &out)
  {
    if (second.size() < first.size()) {
      unordered_set_intersection(second, first, out);
      return;
    }
    out.clear();

    for (typename boost::unordered_set<T>::const_iterator it = first.begin(); it != first.end(); ++it) {
      if (second.find(*it) != second.end()) {
	out.insert(*it);
      }
    }
    return;
  } 


  // Return true iff first and second partition the elements in the same manner.
  bool equal_partitions(const Partition &first, const Partition &second)
  {
    // Quick check: partitions talk about different number of elements
    if (first.NumElements() != second.NumElements()) {
      return false;
    }

    // Slightly more expensive check: see if the symmetric difference is non-empty.
    boost::unordered_set<int> both;
    unordered_set_intersection<int>(first.Elements(), second.Elements(), both);
    if (first.NumElements() != both.size() || second.NumElements() != both.size()) {
      return false; // the partittions talk about different set of elements
    }

    // Full check
    first.Normalize();
    second.Normalize();

    for (boost::unordered_set<int>::const_iterator it=both.begin(); it!=both.end(); ++it) {
      if (first.FindSet(*it) != second.FindSet(*it)) {
	return false;
      }
    }

    return true;
  }


}
}
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
