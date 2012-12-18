#ifndef WALI_UTIL_DETAILS_PARTITION_HPP
#define WALI_UTIL_DETAILS_PARTITION_HPP

#include <ostream>
#include <set>
#include <map>
#include <sstream>
#include <cassert>
#include <list>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>


namespace wali
{
namespace util
{
namespace details
{
  class Partition;

  extern
  bool
  equal_partitions(const Partition &first, const Partition &second);

  // Implementation of Equivalence partition using disjoint-sets. 
  class Partition
  {
    typedef boost::unordered_map<int, std::size_t> rank_t;
    typedef boost::unordered_map<int, int> parent_t;

    typedef boost::unordered_set<int> elements_t;

    typedef boost::associative_property_map<rank_t> rank_p_t;
    typedef boost::associative_property_map<parent_t> parent_p_t;

    typedef boost::disjoint_sets<rank_p_t, parent_p_t> DisjointSets;

    typedef boost::unordered_map<int, std::list<int> > root_set_map_t;

  private:
    mutable rank_t rank_; // Data structure to store rank.
    mutable parent_t parent_; // Data structure to store the parent info.
    mutable elements_t elements_; // The set of all elements in the partition.
    mutable DisjointSets disjoint_sets_; // The disjoint set data structure initialized with rank_ and parent_.
    mutable root_set_map_t rootSetMap_; // Map from roots of the partitions to the set of elements in that partition.

  public:

    // Constructor
    Partition(): disjoint_sets_(rank_p_t(rank_), 
				parent_p_t(parent_)) 
    {}

    // Copy constructor
    // copy constructor
    Partition(const Partition &other) 
      : rank_(other.rank_)
      , parent_(other.parent_)
      , elements_(other.elements_)
      ,	disjoint_sets_(rank_p_t(rank_), parent_p_t(parent_))
      , rootSetMap_(other.rootSetMap_)
      {
	//disjoint_sets_ = DisjointSets(rank_p_t(rank_), parent_p_t(parent_));
      }

    Partition &operator=(const Partition &rhs) 
    {
      if (this != &rhs) {
	rank_ = rhs.rank_;
	parent_ = rhs.parent_;
	elements_ = rhs.elements_;
	disjoint_sets_ = DisjointSets(rank_p_t(rank_), parent_p_t(parent_));
	rootSetMap_ = rhs.rootSetMap_;
      }
      return *this;
    }

    // Clear the partition
    void Clear() 
    {
      rank_.clear();
      parent_.clear();
      disjoint_sets_ = DisjointSets(rank_p_t(rank_), parent_p_t(parent_));
      elements_.clear();
      rootSetMap_.clear();
    }

    // Return the set of elements in partition
    boost::unordered_set<int> Elements() const
    {
      return elements_;
    }

    // Return the total number of elements in all partitions.
    std::size_t NumElements() const
    {
      return elements_.size();
    }

    // Normalize the partition so that the smallest element
    // is the root.
    // TODO this would be easy to maintain incrementally.
    //      But I don't think boost does not.
    void Normalize() const
    {
      disjoint_sets_.normalize_sets(elements_.begin(), elements_.end());
    }

    // Return true if the elements in the partition are unconstrained
    // that is, each disjoint set contains a single element
    // equivalently, the number of disjoint sets equals the number of elements.
    bool IsUnconstrained() const
    {
      return  CountSets() == elements_.size();
    }

    bool operator==(const Partition &rhs) const
    {
      return equal_partitions(*this, rhs);
    }

    bool operator!=(const Partition &rhs) const
    {
      return !equal_partitions(*this, rhs);
    }

    // The essential functionality.

    // Add the fact that v1 and v2 are equivalent
    // return true if v1 was not already equivalent to v2
    // and false if v1 was already equivalent to v2
    bool SetEquivalent(int v1, int v2)
    {
      bool const already_equiv = false;
      if (v1 == v2) 
	return already_equiv;
      ensureElementExists(v1);
      ensureElementExists(v2);

      
      const int r1 = disjoint_sets_.find_set(v1);
      const int r2 = disjoint_sets_.find_set(v2);
      if (r1 == r2) 
	return already_equiv;
      root_set_map_t::const_iterator it1 = rootSetMap_.find(r1);
      assert(it1 != rootSetMap_.end());
      std::list<int> s1 = it1->second;

      root_set_map_t::const_iterator  it2 = rootSetMap_.find(r2);
      assert(it2 != rootSetMap_.end());
      std::list<int> s2 = it2->second;

      s1.splice(s1.begin(), s2); // union the related sets

      disjoint_sets_.link(r1, r2); // union the disjoint sets

      // associate the combined related set with the new root
      int const new_root = disjoint_sets_.find_set(v1);
      if (new_root != r1) {
	rootSetMap_.erase(it1);
      } else {
	rootSetMap_.erase(it2);
      }
      rootSetMap_[new_root] = s1;
      return !already_equiv;
    }
    
    // Return true if v1 and v2 are equivalent
    bool AreEquivalent(int v1, int v2) const
    {
      if (v1 == v2) {
	return true;
      }

      // The element does not exist, so we do not
      // know any equivalence information,
      // hence, v1 and v2 are not equivalent.
      if (!elementExists(v1) || !elementExists(v2)) {
	return false;
      }

      return disjoint_sets_.find_set(v1) == disjoint_sets_.find_set(v2);
    }


    // Return the representative for the node v.
    int FindSet(int v) const
    {
      // If element does not exist
      if (!elementExists(v)) {
	// Then it is its own root.
	// This avoids adding singleton sets to the partition.
	return v;
      }

      return disjoint_sets_.find_set(v);
    }

    // Return the number of disjoint sets in the partition.
    std::size_t CountSets() const
    {
      return disjoint_sets_.count_sets(elements_.begin(), elements_.end());
    }

    // Return a list of elements that are in the same disjoint set
    // as v.
    std::list<int> GetRelatedSet(int v) const
    {
      std::list<int> ret;
      if (!elementExists(v)) {
	ret.push_back(v);
	return ret;
      }
      int root = disjoint_sets_.find_set(v);
      root_set_map_t::const_iterator it = rootSetMap_.find(root);
      assert(it!=rootSetMap_.end());
      return it->second;
    }

    // Return the current set of roots (or representatives) for 
    // each of the partition
    std::set<int> GetRoots() const
    {
      std::set<int> roots;
      for (elements_t::const_iterator it=elements_.begin(); it!=elements_.end(); ++it) {
	if (FindSet(*it) == *it) {
	  roots.insert(*it);
	}
      }
      return roots;
    }

    // Stringify the data-structure
    std::string ToString() const
    {
      std::stringstream ss;
      std::set<int> roots = GetRoots();
      ss << "{ ";
      for (std::set<int>::const_iterator rit = roots.begin(); 
	                                 rit != roots.end(); ++rit) {
	ss << "[ " << *rit ;
	for (elements_t::const_iterator eit=elements_.begin(); eit!=elements_.end();++eit) {
	  if (*eit != *rit && FindSet(*eit) == *rit) {
	    ss << ", " << *eit;
	  }
	}
	ss << " ] ";
      }
      ss << "}";
      return ss.str();
    }

  private:
    // helper functions

    // If v does not exist, then add it to the disjoint set.
    void ensureElementExists(int v) const
    {
      elements_t::const_iterator it = elements_.find(v);
      if (it == elements_.end()) {
	disjoint_sets_.make_set(v);
	elements_.insert(v);
	
	std::list<int> li;
	li.push_front(v);
	rootSetMap_[v] = li;
      }
      return;
    }

    bool elementExists(int v) const
    {
      elements_t::const_iterator it = elements_.find(v);
      return it != elements_.end();
    }

  };
  


}
}
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

