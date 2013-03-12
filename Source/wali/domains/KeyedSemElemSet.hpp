#ifndef WALI_KEYED_SEMELEMSET_HPP
#define WALI_KEYED_SEMELEMSET_HPP

#include "wali/SemElem.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <limits>

#include <iostream>

namespace {
  std::string
  clamp(std::string const & str)
  {
    const size_t maxlen = 50;
    if (str.size() < maxlen) {
      return str;
    }
    else {
      return str.substr(0, maxlen-3) + "...";
    }
  }
}  

namespace wali
{
  namespace domains
  {


    /// Represents a domain from a key to a weight.
    ///
    /// The "key" acts a bit like a guard, except that keys that are not
    /// present act like 0/bottom (concretizes to nothing) rather than top
    /// (concretizes to the universe).
    ///
    /// Thus, if I denote an element by <k,w> where k is the key and w the
    /// weight:
    ///
    ///   o  zero is the empty set {}
    ///   o  one  is the singleton set { <1,1> }
    ///   o  combine does set union (it's possible to have multiple weights
    ///      with the same key, though not more than one entry with the same
    ///      key AND weight)
    ///   o  extend extends every mapping of the first with every mapping of
    ///      the second, doing extend of both key and weight: if the extend
    ///      of the key is 0, then that pair is dropped and the extend of the
    ///      weights is not computed
    ///
    /// The key is treated as a sem_elem_t, but it really doesn't have to be
    /// one -- in particular, keys are never combined.
    ///
    /// Another way that the keys aren't like guards (i.e. what the word
    /// 'guard' suggests) is that there is no mixing of weights between keys.
    class KeyedSemElemSet
      : public wali::SemElem
    {
    public:
      typedef std::tr1::unordered_set<sem_elem_t, SemElemRefPtrHash, SemElemRefPtrEqual>
              InternalSet;
      
      typedef std::tr1::unordered_map<sem_elem_t, InternalSet,
                                      SemElemRefPtrHash, SemElemRefPtrEqual>
              BackingMap;


      /// KeyedSemElemSet stores its elements as a map of sets.
      ///
      /// This stores an inner iterator (InternalSet).
      class const_iterator
        : public boost::iterator_facade<const_iterator,
                                        std::pair<sem_elem_t, sem_elem_t>,
                                        boost::forward_traversal_tag,
                                        std::pair<sem_elem_t, sem_elem_t> >
      {
      public:
        const_iterator() {}
        
      private:
        friend class KeyedSemElemSet;
        friend class boost::iterator_core_access;

        BackingMap::const_iterator outer_iter_current_, outer_iter_end_;
        InternalSet::const_iterator inner_iter_current_, inner_iter_end_;

        const_iterator(BackingMap const & m)
          : outer_iter_current_(m.begin())
          , outer_iter_end_(m.end())
        {
          if (outer_iter_current_ != outer_iter_end_) {
            inner_iter_current_ = outer_iter_current_->second.begin();
            inner_iter_end_ = outer_iter_current_->second.end();
          }
        }

        const_iterator(BackingMap::const_iterator outer_start,
                       BackingMap::const_iterator outer_end)
          : outer_iter_current_(outer_start)
          , outer_iter_end_(outer_end)
        {
          if (outer_iter_current_ != outer_iter_end_) {
            inner_iter_current_ = outer_iter_current_->second.begin();
            inner_iter_end_ = outer_iter_current_->second.end();
          }
        }

        const_iterator(BackingMap::const_iterator outer_start,
                       BackingMap::const_iterator outer_end,
                       InternalSet::const_iterator inner_start,
                       InternalSet::const_iterator inner_end)
          : outer_iter_current_(outer_start)
          , outer_iter_end_(outer_end)
          , inner_iter_current_(inner_start)
          , inner_iter_end_(inner_end)
        {
          if (outer_iter_current_ == outer_iter_end_) {
            inner_iter_current_ = inner_iter_end_ = InternalSet::const_iterator();
          }
          else if (inner_iter_current_ == inner_iter_end_) {
            fixup();
          }
        }

        void
        fixup() {
          // If we are at the end of the current inner set, then advance to
          // the next inner set, and reset the inner iterators.
          if (inner_iter_current_ == inner_iter_end_) {
            ++outer_iter_current_;
            if (outer_iter_current_ != outer_iter_end_) {
              inner_iter_current_ = outer_iter_current_->second.begin();
              inner_iter_end_ = outer_iter_current_->second.end();
            }
            else {
              // Well, unless that's the very last thing.
              inner_iter_current_ = inner_iter_end_ = InternalSet::const_iterator();
            }
          }
        }

        void
        increment() {
          assert(outer_iter_current_ != outer_iter_end_);

          // Advance the inner pointer.
          ++inner_iter_current_;
          fixup();
        }


        bool
        equal(const_iterator const & other) const {
          return this->outer_iter_current_ == other.outer_iter_current_ &&
            this->inner_iter_current_ == other.inner_iter_current_;
        }


        std::pair<sem_elem_t, sem_elem_t>
        dereference() const {
          return std::make_pair(outer_iter_current_->first, *inner_iter_current_);
        }
      }; // end of DisjointSets::const_iterator


      static
      size_t
      get_hash(InternalSet const & s) {
        size_t xor_part = 0u;
        size_t sum_part = 0u;

        for (InternalSet::const_iterator it = s.begin();
             it != s.end(); ++it)
        {
          size_t elem_hash = (*it)->hash();
          xor_part ^= elem_hash;
          sum_part += elem_hash;
        }
        return xor_part + 17*sum_part;
      }
      
      static
      size_t
      get_hash(BackingMap const & m) {
        size_t xor_part = 0u;
        size_t sum_part = 0u;

        for (BackingMap::const_iterator it = m.begin();
             it != m.end(); ++it)
        {
          size_t elem_hash = it->first->hash() + 17*get_hash(it->second);
          xor_part ^= elem_hash;
          sum_part += elem_hash;
        }
        return xor_part + 17*sum_part;
      }


      static
      BackingMap
      remove_zeroes(BackingMap m)
      {
        for (BackingMap::const_iterator it = m.begin();
             it != m.end(); /*nothing*/ )
        {
          sem_elem_t guard = it->first;
          bool all_zero = true;
          for (InternalSet::const_iterator weight = it->second.begin();
               weight != it->second.end(); ++weight)
          {
            if (!(*weight)->equal((*weight)->zero())) {
              all_zero = false;
              break;
            }
          }
          
          if (guard->equal(guard->zero()) || all_zero) {
            it = m.erase(it);
          }
          else {
            ++it;
          }
        }
        return m;
      }
      

      KeyedSemElemSet(BackingMap const & m)
        : map_(remove_zeroes(m))
        , hash_(get_hash(map_))
      {
        assert(m.size() > 0u);
        one_key_ = m.begin()->first->one();
        one_value_ = (*(m.begin()->second.begin()))->one();
      }
      
      KeyedSemElemSet(BackingMap const & m,
                      sem_elem_t example_key,
                      sem_elem_t example_value)
        : map_(remove_zeroes(m))
        , one_key_(example_key->one())
        , one_value_(example_value->one())
        , hash_(get_hash(map_))
      {}

      std::pair<const_iterator, const_iterator>
      equal_range(sem_elem_t key) const {
        BackingMap::const_iterator it = map_.find(key), it2 = it;
        if (it == map_.end()) {
          return std::make_pair(const_iterator(), const_iterator());
        }
        it2++;
        return std::make_pair(const_iterator(it, it2, it->second.begin(), it->second.end()),
                              const_iterator(it, it2, it->second.end(), it->second.end()));
      }

      const_iterator
      begin() const {
        return const_iterator(map_);
      }

      const_iterator
      end() const {
        return const_iterator(map_.end(), map_.end());
      }

      size_t size() const {
        size_t s = 0;
        for (BackingMap::const_iterator values = this->map_.begin();
             values != this->map_.end(); ++values)
        {
          s += values->second.size();
        }
        return s;
      }
      
      sem_elem_t one() const {
        BackingMap m;
        m[one_key_].insert(one_value_);
        return new KeyedSemElemSet(m);
      }
      
      sem_elem_t zero() const {
        BackingMap m;
        return new KeyedSemElemSet(m, one_key_, one_value_);
      }

      sem_elem_t extend(SemElem * se) {
        KeyedSemElemSet * that = dynamic_cast<KeyedSemElemSet*>(se);
        assert(that);

        BackingMap m;

        for (BackingMap::const_iterator this_guard = this->map_.begin();
             this_guard != this->map_.end(); ++this_guard)
        {
          for (BackingMap::const_iterator that_guard = that->map_.begin();
               that_guard != that->map_.end(); ++that_guard)
          {
            sem_elem_t new_guard = this_guard->first->extend(that_guard->first);
            
            if (!new_guard->equal(new_guard->zero())) {
              for (InternalSet::const_iterator this_value = this_guard->second.begin();
                   this_value != this_guard->second.end(); ++this_value)
              {
                for (InternalSet::const_iterator that_value = that_guard->second.begin();
                     that_value != that_guard->second.end(); ++that_value)
                {
                  sem_elem_t new_weight = (*this_value)->extend(*that_value);
                  m[new_guard].insert(new_weight);
                } // for
              } // for
            } // if
          } // for
        } // for
        
        return new KeyedSemElemSet(m, one_key_, one_value_);
      }
      
      sem_elem_t combine(SemElem * se) {
        KeyedSemElemSet * that = dynamic_cast<KeyedSemElemSet*>(se);
        assert(that);

        BackingMap m(this->map_);
        for (BackingMap::const_iterator that_guard = that->map_.begin();
             that_guard != that->map_.end(); ++that_guard)
        {
          m[that_guard->first].insert(that_guard->second.begin(),
                                      that_guard->second.end());
        }

        return new KeyedSemElemSet(m, one_key_, one_value_);
      }

      bool equal(SemElem * se) const {
        KeyedSemElemSet * that = dynamic_cast<KeyedSemElemSet*>(se);
        assert(that);

        if (this->map_.size() != that->map_.size()
            || !this->one_key_->equal(that->one_key_)
            || !this->one_value_->equal(that->one_value_))
        {
          return false;
        }

        for (BackingMap::const_iterator this_guard = this->map_.begin();
             this_guard != this->map_.end(); ++this_guard)
        {
          if (that->map_.count(this_guard->first) == 0u
              || that->map_[this_guard->first].size() != this_guard->second.size())
          {
            return false;
          }

          for (InternalSet::const_iterator this_value = this_guard->second.begin();
               this_value != this_guard->second.end(); ++this_value)
          {
            if (that->map_[this_guard->first].count(*this_value) == 0u) {
              return false;
            }
          }
        }

        return true;
      }

      std::ostream& print( std::ostream & o ) const {
        o << "{ ";
        bool first = true;
        for (BackingMap::const_iterator element = this->map_.begin();
             element != this->map_.end(); ++element)
        {
          if (!first) {
            o << ", ";
            first = false;
          }
          element->first->print(o) << ": {";
          bool internal_first = true;
          for (InternalSet::const_iterator weight = element->second.begin();
               weight != element->second.end(); ++weight)
          {
            if (!internal_first) {
              o << ", ";
              internal_first = false;
            }
            o << clamp((*weight)->toString());
          }
          o << "}";
        }
        o << "}";
        return o;
      }


      virtual size_t hash() const { return hash_; }


      // Pull in the sem_elem_t overloads
      using SemElem::extend;
      using SemElem::combine;
      using SemElem::equal;

    private:      
      BackingMap map_;
      sem_elem_t one_key_, one_value_;
      size_t hash_;
    };
    

    /// Represents a key that might be written [s->s'](w). It applies when
    /// a portion of the current state moves from s to s'. s and s' are some
    /// literal value of type 'PositionType'.
    ///
    /// [s->s'] extend [s'->s''] gives [s->s''];
    ///
    /// The class tracks a distinguished 0 value that will produce 0 when
    /// extended.
    ///
    /// This is NOT an actual semiring, despite what the inheritance
    /// claims. If someone calls combine(), this will abort, as with
    /// one(). (See the comment on KeyedSemElemSet for more details.)
    ///
    /// To be useful in its intended context, PositionType should have an
    /// operator== defined as well as either operator< or a hash<>er, as
    /// appropriate (depending on the setting of KeyedSemElemSet). It should
    /// be copy-constructable. If you call print(), it should have an <<
    /// defined.
    template <typename PositionType>
    class PositionKey
      : public wali::SemElem
    {
      enum Special { SpecialOne, SpecialZero };
      
      boost::optional<PositionType> pre_, post_;
      boost::optional<Special> special_;

      PositionKey(Special s)
        : special_(s)
      {}

      bool valid() const {
        if (special_) {
          return !pre_ && !post_;
        }
        else {
          return pre_ && post_;
        }
      }

    public:
      static PositionKey makeOne() {
        return PositionKey(SpecialOne);
      }

      static PositionKey makeZero() {
        return PositionKey(SpecialZero);
      }
      
      PositionKey(PositionType const & pre, PositionType const & post)
        : pre_(pre)
        , post_(post)
      {}


      PositionType pre() const {
        assert(valid() && !special_);
        return *pre_;
      }

      PositionType post() const {
        assert(valid() && !special_);
        return *post_;
      }

      bool isZero() const {
        assert(valid());
        return special_ && *special_ == SpecialZero;
      }

      bool isOne() const {
        assert(valid());
        return special_ && *special_ == SpecialOne;
      }
      
      ///
      /// Actually returns one
      sem_elem_t one() const {
        static sem_elem_t o = new PositionKey(SpecialOne);
        return o;
      }

      ///
      /// This actually returns zero
      sem_elem_t zero() const {
        static sem_elem_t z = new PositionKey(SpecialZero);
        return z;
      }

      ///
      /// This actually extends, as described above
      sem_elem_t extend(SemElem * se)
      {
        PositionKey * that = dynamic_cast<PositionKey*>(se);
        assert(that != NULL);

        if (this->isZero() || that->isZero()) {
          return zero();
        }

        if (this->isOne()) {
          // Hmmm. I will assume that if the reference count is non-zero then
          // it is in a ref_pointer.
          if (that->count > 0) {
            return that;
          }
          else {
            return new PositionKey(*that);
          }
        }
        if (that->isOne()) {
          if (this->count > 0) {
            return this;
          }
          else {
            return new PositionKey(*this);
          }
        }
        
        if (*this->post_ != *that->pre_) {
          return zero();
        }

        return new PositionKey(*this->pre_, *that->post_);
      }

      using SemElem::extend;

      ///
      /// This aborts
      sem_elem_t combine(SemElem * UNUSED_PARAMETER(se)) {
        abort();
        return NULL;
      }

      ///
      /// This actually tells equality
      bool equal(SemElem * se) const {
        PositionKey * that = dynamic_cast<PositionKey*>(se);
        assert(that != NULL);

        if (this->isZero() && that->isZero()) {
          return true;
        }
        if (this->isZero() || that->isZero()) {
          return false;
        }

        if (this->isOne() && that->isOne()) {
          return true;
        }
        if (this->isOne() || that->isOne()) {
          return false;
        }

        return (*this->pre_ == *that->pre_)
          && (*this->post_ == *that->post_);
      }

      using SemElem::equal;
      using SemElem::combine;

      ///
      /// This actually prints the guard
      std::ostream& print( std::ostream & os ) const {
        assert(valid());
        if (isZero()) {
          os << "[ZERO]";
        }
        else if (isOne()) {
          os << "[ONE]";
        }
        else {
          os << "[" << *pre_ << "->" << *post_ << "]";
        }
        return os;
      }

      ///
      /// This actually does <
      bool containerLessThan(SemElem const * se) const {
        PositionKey const * that = dynamic_cast<PositionKey const *>(se);
        assert(that != NULL);

        if (this->isZero() && that->isZero()) {
          // this == that, so false
          return false;
        }
        if (this->isZero()) {
          // 0 sorts first
          return true;
        }
        if (that->isZero()) {
          // 0 sorts first
          return false;
        }

        if (this->isOne() && that->isOne()) {
          return false;
        }
        if (this->isOne()) {
          // One sorts first except for 0
          return true;
        }
        if (that->isOne()) {
          return false;
        }

        return std::make_pair(*this->pre_, *this->post_)
          < std::make_pair(*that->pre_, *that->post_);
      }

      ///
      /// This actually does hash
      size_t hash() const {
        std::tr1::hash<PositionType> hasher;

        if (isZero()) {
          return std::numeric_limits<size_t>::max();
        }
        if (isOne()) {
          return std::numeric_limits<size_t>::max() - 1;
        }
        
        return hasher(*pre_) + 17*hasher(*post_);
      }
    };
    
  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif
