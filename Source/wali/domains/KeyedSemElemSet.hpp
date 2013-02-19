#ifndef WALI_KEYED_SEMELEMSET_HPP
#define WALI_KEYED_SEMELEMSET_HPP

#include "wali/SemElem.hpp"

#include <boost/optional.hpp>
#include <tr1/unordered_map>
#include <limits>

#include <iostream>

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
    ///   o  combine does something like set union but combining elements with
    ///              matching keys
    ///   o   extend extends every mapping of the first with every mapping of
    ///              the second, doing extend of both key and weight: if the
    ///              extend of the key is 0, then that pair is dropped and the
    ///              extend of the weights is not computed
    ///
    /// The key is treated as a sem_elem_t, but it really doesn't have to be
    /// one -- in particular, keys are never combined.
    ///
    /// Another way that the keys aren't like guards (i.e. what the word
    /// 'guard' suggests) is that keys have to be disjoint. The combine
    ///     { <k1, w1> } + { <k2, w2> }
    /// will always be either
    ///     { <k1, w1+w2> }  or  { <k1, w1>,  <k2, w2> }
    /// depending on whether k1==k2 (in the first case). So if two keys are
    /// different, there is no mixing of their associated values.
    class KeyedSemElemSet
      : public wali::SemElem
    {
    public:
      typedef std::tr1::unordered_multimap<sem_elem_t, sem_elem_t,
                                           SemElemRefPtrHash, SemElemRefPtrEqual>
              BackingMap;

      typedef BackingMap::const_iterator const_iterator;

      KeyedSemElemSet(BackingMap const & m)
        : map_(m)
      {
        assert(m.size() > 0u);
        one_key_ = m.begin()->first->one();
        one_value_ = m.begin()->second->one();
      }
      
      KeyedSemElemSet(BackingMap const & m,
                      sem_elem_t example_key,
                      sem_elem_t example_value)
        : map_(m)
        , one_key_(example_key->one())
        , one_value_(example_value->one())
      {}

      std::pair<const_iterator, const_iterator>
      equal_range(sem_elem_t key) const {
        return map_.equal_range(key);
      }

      size_t size() const {
        return map_.size();
      }
      
      sem_elem_t one() const {
        BackingMap m;
        m.insert(std::make_pair(one_key_, one_value_));
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

        for (BackingMap::const_iterator this_element = this->map_.begin();
             this_element != this->map_.end(); ++this_element)
        {
          for (BackingMap::const_iterator that_element = that->map_.begin();
               that_element != that->map_.end(); ++that_element)
          {
            sem_elem_t
              this_guard = this_element->first,
              that_guard = that_element->first,
              this_weight = this_element->second,
              that_weight = that_element->second;

            sem_elem_t new_guard = this_guard->extend(that_guard);

            if (!new_guard->equal(new_guard->zero())) {
              sem_elem_t new_weight = this_weight->extend(that_weight);
              m.insert(std::make_pair(new_guard, new_weight));
            }
          }
        }
        
        return new KeyedSemElemSet(m, one_key_, one_value_);
      }
      
      sem_elem_t combine(SemElem * UNUSED_PARAMETER(se)) { abort(); }

      bool equal(SemElem * se) const {
        KeyedSemElemSet * that = dynamic_cast<KeyedSemElemSet*>(se);
        assert(that);

        if (this->map_.size() != that->map_.size()
            || !this->one_key_->equal(that->one_key_)
            || !this->one_value_->equal(that->one_value_))
        {
          return false;
        }

        for (BackingMap::const_iterator this_element = this->map_.begin();
             this_element != this->map_.end(); ++this_element)
        {
          std::pair<BackingMap::const_iterator, BackingMap::const_iterator>
            range = that->map_.equal_range(this_element->first);
          bool found = false;
          for ( ; range.first != range.second; ++range.first) {
            if (range.first->second->equal(this_element->second)) {
              found = true;
            }
          }
          if (!found) {
            return false;
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
          }
          element->first->print(o) << " ";
          element->second->print(o);
          first = false;
        }
        o << "}";
        return o;
      }


      // Pull in the sem_elem_t overloads
      using SemElem::extend;
      using SemElem::combine;
      using SemElem::equal;

    private:      
      BackingMap map_;
      sem_elem_t one_key_, one_value_;
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
        if (isZero()) {
          os << "[ZERO]";
        }
        if (isOne()) {
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
