#include "Partition.hpp"
#include "utils.hpp"

DEFINE_bool(linear_partition_intersect, true, "If true, use linear partition intersection algorithm, else use quadratic.");


namespace Duvidha
{



  // a \equiv b in ret iff a \equiv b in first and a \equiv b in second for all a,b
  void intersect_partitions_linear(const Partition &first, const Partition &second, Partition &ret)
  {
    ret.Clear();

    // Only consider elements are mentioned in both partitions.
    boost::unordered_set<int> both;
    unordered_set_intersection<int>(first.Elements(), second.Elements(), both);

    // Linear intersection algorithm.
    pair_int_map_t fingerprintMap;
    for (boost::unordered_set<int>::const_iterator it=both.begin(); it!=both.end(); ++it) {
      // Compute fingerprint.
      std::pair<int, int> pairedRoots(first.FindSet(*it), second.FindSet(*it));
      pair_int_map_t::const_iterator mit = fingerprintMap.find(pairedRoots);
      if ( mit == fingerprintMap.end() ) {
	fingerprintMap[pairedRoots] = *it;
      }
      else {
	assert(*it != mit->second);
	ret.SetEquivalent(*it, mit->second);
      }
    }
    return;
  }

  // a \equiv b in ret iff a \equiv b in first and a \equiv b in second for all a,b
  void intersect_partitions_quadratic(const Partition &first, const Partition &second, Partition &ret)
  {
    ret.Clear();

    // Only consider elements are mentioned in both partitions.
    boost::unordered_set<int> both;
    unordered_set_intersection<int>(first.Elements(), second.Elements(), both);

    // Linear intersection algorithm.
    pair_int_map_t fingerprintMap;
    for (boost::unordered_set<int>::const_iterator fit=both.begin(); fit!=both.end(); ++fit) {
      for (boost::unordered_set<int>::const_iterator sit=both.begin(); sit!=both.end(); ++sit) {
	if (first.AreEquivalent(*fit, *sit) &&
	    second.AreEquivalent(*fit, *sit) ) {
	  ret.SetEquivalent(*fit, *sit);
	}
      }
    }
    return;
  }


  void intersect_partitions(const Partition &first, const Partition &second, Partition &ret)
  {
    if (FLAGS_linear_partition_intersect) {
      intersect_partitions_linear(first, second, ret);
    } else {
      intersect_partitions_quadratic(first, second, ret);
    }
    return;
  }

  // a \equiv b in ret iff a \equiv b in first or a \equiv b in second for all a,b
  void union_partitions(const Partition &first, const Partition &second, Partition &ret)
  {
    if (second.NumElements() < first.NumElements()) {
      union_partitions(second, first, ret);
      return;
    }
    ret.Clear();

    // Simply copy over the second
    ret = second;

    // Iterate over all elems in first
    boost::unordered_set<int> felems = first.Elements();
    for (boost::unordered_set<int>::const_iterator it=felems.begin(); it!=felems.end();++it) {
      ret.SetEquivalent(*it, first.FindSet(*it));
    }

    return;
  }

  // a \equiv b in ret iff a \equiv b in first or a \equiv b in second for all a,b
  void union_partitions(const Partition &first, Partition &in_out)
  {
    // Iterate over all elems in first
    boost::unordered_set<int> felems = first.Elements();
    for (boost::unordered_set<int>::const_iterator it=felems.begin(); it!=felems.end();++it) {
      in_out.SetEquivalent(*it, first.FindSet(*it));
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


} //end namespace Duvidha
