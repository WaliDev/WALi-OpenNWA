#ifndef WALI_KEYED_SEMELEMSET_HPP
#define WALI_KEYED_SEMELEMSET_HPP

#include "wali/SemElem.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include "wali/util/unordered_map.hpp"
#include "wali/util/unordered_set.hpp"
#include <limits>
#include <typeinfo>

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
      typedef wali::util::unordered_map<sem_elem_t, sem_elem_t,
                                        SemElemRefPtrHash, SemElemRefPtrEqual>
              BackingMap;

      typedef BackingMap::const_iterator const_iterator;


      static
      size_t
      get_hash(BackingMap const & m) {
        size_t xor_part = 0u;
        size_t sum_part = 0u;

        for (BackingMap::const_iterator it = m.begin();
             it != m.end(); ++it)
        {
          size_t elem_hash = it->first->hash() + 17*it->second->hash();
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
          sem_elem_t weight = it->second;

          if (guard->equal(guard->zero())
              || weight->equal(weight->zero()))
          {
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
        one_value_ = m.begin()->second->one();
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
        return map_.equal_range(key);
      }

      const_iterator
      begin() const {
        return map_.begin();
      }

      const_iterator
      end() const {
        return map_.end();
      }

      size_t size() const {
        return map_.size();
      }
      
      sem_elem_t one() const {
        BackingMap m;
        m[one_key_] = one_value_;
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
              sem_elem_t new_weight =
                this_guard->second->extend(that_guard->second);

              BackingMap::iterator loc = m.find(new_guard);
              if (loc == m.end()) {
                m.insert(loc, std::make_pair(new_guard, new_weight));
              }
              else {
                new_weight = new_weight->combine(loc->second);
                loc->second = new_weight;
              }
            }
          }
        }
        
        return new KeyedSemElemSet(m, one_key_, one_value_);
      }
      
      sem_elem_t combine(SemElem * se) {
        KeyedSemElemSet * that = dynamic_cast<KeyedSemElemSet*>(se);
        assert(that);

        BackingMap m(this->map_);
        for (BackingMap::const_iterator that_guard = that->map_.begin();
             that_guard != that->map_.end(); ++that_guard)
        {
          BackingMap::iterator new_loc = m.find(that_guard->first);

          if (new_loc == m.end()) {
            m.insert(new_loc, *that_guard);
          }
          else {
            sem_elem_t new_weight = new_loc->second->combine(that_guard->second);
            new_loc->second = new_weight;
          }
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
          BackingMap::const_iterator that_loc = that->map_.find(this_guard->first);

          if (that_loc == that->map_.end()
              || !that_loc->second->equal(this_guard->second))
          {
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
          first = false;          
          element->first->print(o) << ": ";
          element->second->print(o);
        }
        o << "}";
        return o;
      }


      virtual size_t hash() const { return hash_; }

      virtual
      std::ostream &
      print_typename(std::ostream & os) const {
        os << "KeyedSemElemSet<";
        if (size() == 0u) {
          os << "(empty)";
        }
        else {
          os << begin()->first->print_typename(os) << " -> ";
          os << begin()->second->print_typename(os);
        }
        return os << ">";
      }


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

      virtual
      std::ostream &
      print_typename(std::ostream & os) const {
        os << "PositionKey<" << typeid(PositionType).name() << ">";
        return os;
      }

      
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
        wali::util::hash<PositionType> hasher;

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
