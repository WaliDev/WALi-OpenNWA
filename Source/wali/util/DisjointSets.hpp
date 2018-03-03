#ifndef WALI_UTIL_DISJOINT_SETS_HPP
#define WALI_UTIL_DISJOINT_SETS_HPP

#include "wali/Common.hpp"
#include "wali/util/details/Partition.hpp"
#include "wali/util/map_at.hpp"

#include <boost/bimap/bimap.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
#include <sstream>

namespace wali
{
  namespace util
  {

    /// This class implements a disjoint set data structure
    ///
    /// It will store sets of sets of ElementTypes.
    template<typename ElementType,
             typename IdMapping = boost::bimaps::bimap<ElementType, int> >
    class DisjointSets
    {
      typedef int Id;
      
      details::Partition _partition;
      IdMapping _id_mapping;
      Id _next_id;
      mutable boost::shared_ptr<std::set<int> > _cached_roots;

    public:
      typedef ElementType            value_type;
      typedef ElementType            key_type;
      typedef ElementType &          reference;
      typedef ElementType *          pointer;
      typedef ElementType const &    const_reference;
      typedef ElementType const *    const_pointer;
      typedef size_t                 size_type;
      typedef ptrdiff_t              difference_type;
      

      DisjointSets()
        : _next_id(0)
      {}


	  bool
      insert(value_type const & value) {
        invalidate_cache();
        Id id = get_create_id(value);
        bool was_present = _partition.elementExists(id);
        if (!was_present) {
          _partition.Insert(id);
          invalidate_cache();
        }
        return was_present;
      }


      bool
      merge_sets(value_type const & first, value_type const & second) {
        Id
          id_first = get_create_id(first),
          id_second = get_create_id(second);
        // You need to call insert in order to create a singleton partition.
		// merge_sets(a, a) is not the same as insert(a).
		assert(id_first != id_second || _partition.elementExists(id_first));

        bool changed = _partition.SetEquivalent(id_first, id_second);
        if (changed) {
          invalidate_cache();
        }
        return changed;
      }


      void
      merge_all() {
        if (this->begin() == this->end()) {
          return;
        }
        
        while (true) {
          const_iterator
            first = this->begin(),
            second = first;
          ++second;
          
          if (second == this->end()) {
            return;
          }

          merge_sets(*(first->begin()), *(second->begin()));
        }
      }

      /// The DisjointSets is a set of sets of Elements -- the user-visible
      /// view of the second "sets" is provided by this type.
      //
      // In reality, the inner sets are lists of IDs, so this class serves as
      // an adapter from such a list to the actual elements.
      class InnerSet {
        boost::shared_ptr<std::list<int> > _ids;
        IdMapping const & _id_mapping;

        friend class DisjointSets;

        InnerSet(boost::shared_ptr<std::list<int> > ids,
                 IdMapping const & mapping)
          : _ids(ids)
          , _id_mapping(mapping)
        {}


      public:
        /// This iterator iterates over an InnerSetView, so it returns actual
        /// elements.
        //
        // In reality, it's esentially iterating over the list of IDs.
        class const_iterator
          : public boost::iterator_facade<const_iterator,
                                          ElementType const,
                                          boost::forward_traversal_tag>
        {
        public:
          const_iterator()
            : _id_mapping(0)
          {}


        private:
          std::list<int>::const_iterator _current_element;
          boost::shared_ptr<std::list<int> > _ids;
          IdMapping const * _id_mapping;
          friend class InnerSet;
          friend class boost::iterator_core_access;
          

          const_iterator(std::list<int>::const_iterator current,
                         boost::shared_ptr<std::list<int> > ids,
                         IdMapping const & mapping)
            : _current_element(current)
            , _ids(ids)
            , _id_mapping(&mapping)
          {}


          // iterator_facade builds operator++, operator*, etc. from the
          // following functions (and it also provides 
          void
          increment() {
            assert(_current_element != _ids->end());
            ++_current_element;
          }


          bool
          equal(const_iterator const & other) const {
            // Iterators to the "same" element in different set views will be
            // different. Fix this? TODO
            return this->_current_element == other._current_element;
          }


          typename DisjointSets::value_type const &
          dereference() const {
            assert(_id_mapping);
            assert(_current_element != _ids->end());
            return bimap_at(_id_mapping->right, *_current_element);
          }
        }; // end of class InnerSet::const_iterator


        const_iterator
        begin() const {
          return const_iterator(_ids->begin(), _ids, _id_mapping);
        }


        const_iterator
        end() const {
          return const_iterator(_ids->end(), _ids, _id_mapping);
        }
      }; // end of class InnerSet


      /// DisjointSets is a set of sets of elements. This iterator is for
      /// iterating over the outer "sets".
      ///
      /// The value type of this iterator is an InnerSet (const).
      //
      // In reality, we are iterating over the set of roots of the disjoint
      // set. When the iterator is created or modified, we create a new
      // InnerSet instance.
      class const_iterator
        : public boost::iterator_facade<const_iterator,
                                        InnerSet const,
                                        boost::forward_traversal_tag>
      {
      public:
        const_iterator()
          : _id_mapping(0)
          , _partition(0)
        {}

        
      private:
        std::set<int>::const_iterator _current_root;
        boost::shared_ptr<InnerSet> _current_inner_set;
        boost::shared_ptr<std::set<int> > _roots;
        IdMapping const * _id_mapping;
        details::Partition const * _partition;
        friend class DisjointSets;
        friend class boost::iterator_core_access;
        

        const_iterator(details::Partition const & partition,
                       std::set<int>::const_iterator element,
                       boost::shared_ptr<std::set<int> > roots,
                       IdMapping const & mapping)
          : _current_root(element)
          , _roots(roots)
          , _id_mapping(&mapping)
          , _partition(&partition)
        {
          update_current_inner_set();
        }


        void
        update_current_inner_set() {
          if (_current_root == _roots->end()) {
            _current_inner_set.reset();
          }
          else {
            // We are creating a new InnerSet. We need to know the elements
            // which are in it. Get their ids.
            std::list<int> * ids_list = new std::list<int>(_partition->GetRelatedSet(*_current_root));
            boost::shared_ptr<std::list<int> > ids_in_current_inner_set(ids_list);

            // Now create and cache the InnerSet instance
            InnerSet * newset = new InnerSet(ids_in_current_inner_set, *_id_mapping);
            _current_inner_set.reset(newset);
          }
        }


        void
        increment() {
          assert(_current_root != _roots->end());
          ++_current_root;
          update_current_inner_set();
        }


        bool
        equal(const_iterator const & other) const {
          return this->_current_root == other._current_root;
        }


        InnerSet const &
        dereference() const {
          return *_current_inner_set;
        }
      }; // end of DisjointSets::const_iterator


      const_iterator
      begin() const {
        update_cache();
        return const_iterator(_partition,
                              _cached_roots->begin(),
                              _cached_roots,
                              _id_mapping);
      }
      

      const_iterator
      end() const {
        update_cache();
        return const_iterator(_partition,
                              _cached_roots->end(),
                              _cached_roots,
                              _id_mapping);
      }


      const_iterator
      find(const_reference needle) const {
        typename IdMapping::left_const_iterator iter = _id_mapping.left.find(needle);
        if (iter == _id_mapping.left.end()) {
          return end();
        }

        Id id = iter->second;
        bool was_present = _partition.elementExists(id);
        if (!was_present) {
          fast_assert(false && "Element present in DisjointSets but not contained Partition");
          return end();
        }

        update_cache();
        std::set<Id>::const_iterator root = _cached_roots->find(id);
        if (root == _cached_roots->end()) {
          fast_assert(false && "Element present in DisjointSets and Partition, but not _cached_roots");
          return end();
        }

        return const_iterator(_partition,
                              root,
                              _cached_roots,
                              _id_mapping);
      }


      std::ostream &
      output(std::ostream & os) const {
        std::stringstream ss;
        ss << "{";
        for(const_iterator outer_iter = this->begin();
            outer_iter != this->end(); ++outer_iter)
        {
          if (outer_iter != this->begin()) {
            ss << ", ";
          }
          ss << "{";
          for(typename InnerSet::const_iterator inner_iter = outer_iter->begin();
              inner_iter != outer_iter->end(); ++inner_iter)
          {
            if (inner_iter != outer_iter->begin()) {
              ss << ", ";
            }
            ss << *inner_iter;
          }
          ss << "}";
        }
        ss << "}";
        
        os << ss.rdbuf();
        return os;
      }


      std::string
      to_string() const {
        std::stringstream ss;
        this->output(ss);
        return ss.str();
      }


      bool
      operator==(DisjointSets const & that) const {
        // Number of elements total
        if (this->_id_mapping.left.size() != that._id_mapping.right.size()) {
          return false;
        }
        
        this->update_cache();
        that.update_cache();

        // Number of partitions
        if (this->_cached_roots->size() != that._cached_roots->size()) {
          return false;
        }

        // For each partition in this, check that it is a partition in that.
        for(const_iterator outer_iter = this->begin();
            outer_iter != this->end(); ++outer_iter)
        {
          assert(outer_iter->begin() != outer_iter->end());
          ElementType const & first_element = *outer_iter->begin();

          if (that._id_mapping.left.find(first_element)
              == that._id_mapping.left.end())
          {
            // element isn't present at all
            return false;
          }
          Id first_element_id = that._id_mapping.left.find(first_element)->second;
          Id that_root = that._partition.FindSet(first_element_id);

          for(typename InnerSet::const_iterator inner_iter = outer_iter->begin();
              inner_iter != outer_iter->end(); ++inner_iter)
          {
            ElementType const & element = *inner_iter;
            if (that._id_mapping.left.find(element)
                == that._id_mapping.left.end())
            {
              // element isn't present at all
              return false;
            }
            Id element_id = that._id_mapping.left.find(element)->second;

            if (that_root != that._partition.FindSet(element_id)) {
              // element is in a different partition in the other set
              return false;
            }
          }
        }

        return true;
      }


      bool
      operator!=(DisjointSets const & that) const {
        return !(*this == that);
      }


      const_reference
      representative(const_reference element) const {
        assert(_id_mapping.left.count(element) > 0);
        Id element_id = _id_mapping.left.find(element)->second;
        Id root_id = _partition.FindSet(element_id);
        assert(_id_mapping.right.count(root_id) > 0);
        return _id_mapping.right.find(root_id)->second;
      }

          
    private:

      void
      invalidate_cache() {
        _cached_roots.reset();
      }

      void
      update_cache() const {
        if (!_cached_roots) {
          _cached_roots.reset(new std::set<int>(_partition.GetRoots()));
        }
      }

      
      Id
      get_create_id(value_type const & value) {
        typename IdMapping::left_iterator iter = _id_mapping.left.find(value);
        if (iter == _id_mapping.left.end()) {
          _id_mapping.left.insert(std::make_pair(value, _next_id));
          ++_next_id;
          return _next_id - 1;
        }
        else {
          return iter->second;
        }
      }
          
    };


    template<typename ElementType, typename IdMapping>
    std::ostream &
    operator<< (std::ostream & os, DisjointSets<ElementType, IdMapping> const & sets) {
      os << sets.to_string();
      return os;
    }

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif /* WALI_UTIL_DISJOINT_SETS_HPP */
