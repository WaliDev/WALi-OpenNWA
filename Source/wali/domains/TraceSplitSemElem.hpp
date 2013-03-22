#ifndef WALI_TRACESPLIT_SEMELEMSET_HPP
#define WALI_TRACESPLIT_SEMELEMSET_HPP

#include <limits>
#include <boost/optional.hpp>
#include <boost/function.hpp>

#include "wali/util/unordered_map.hpp"
#include "wali/SemElem.hpp"

namespace wali
{
  namespace domains
  {

    class Guard
      : public wali::Countable
      , public wali::Printable
    {
    public:
      typedef ref_ptr<Guard> Ptr;

      virtual size_t hash() const = 0;
      virtual bool equal(Ptr other) const = 0;
      virtual bool isFalse() const = 0;
    };

    struct GuardRefPtrHash
    {
      size_t
      operator() (Guard::Ptr se) const {
        return se->hash();
      }
    };

    struct GuardRefPtrEqual
    {
      bool
      operator() (Guard::Ptr left, Guard::Ptr right) const {
        return left->equal(right);
      }
    };

    template <typename ElementType>
    class LiteralGuard
      : public Guard
    {
    public:
      typedef ElementType value_type;

      LiteralGuard(value_type const & value)
        : _value(value)
      {}

      // creates false
      LiteralGuard()
        : _special(SpecialFalse)
      {}

      LiteralGuard(LiteralGuard const & other)
        : _value(other._value)
        , _special(other._special)
      {}

      virtual size_t hash() const {
        if (_special) {
          return std::numeric_limits<size_t>::max();
        }
        else {
          wali::util::hash<value_type> hasher;
          return hasher(*_value);
        }
      }

      virtual bool equal(Ptr other) const {
        LiteralGuard * that = dynamic_cast<LiteralGuard*>(other.get_ptr());
        if (this->_special && that->_special) {
          // currently there is only one special value
          assert (*this->_special == *that->_special);
          return *this->_special == *that->_special;
        }
        if (!this->_special && !that->_special) {
          return *this->_value == *that->_value;
        }
        return false;
      }

      virtual bool isFalse() const {
        assert (!_special || *_special == SpecialFalse);
        return _special;
      }

      virtual std::ostream& print(std::ostream & os) const {
        if (isFalse()) {
          os << "[FALSE]";
        }
        else {
          os << "[" << *_value << "]";
        }
        return os;
      }

    private:
      bool valid() const {
        return (_special && !_value) || (!_special && _value);
      }

      enum Special { SpecialFalse };
      boost::optional<value_type> _value;
      boost::optional<Special> _special;
    };
    

    /// Represents a domain from a key to a weight.
    ///
    /// The "key" acts a bit like a guard, except that keys that are
    /// not present act like a specific default value ("most of the
    /// time", this will be 0/bottom/concretizes to nothing).
    ///
    /// This class differs from KeyedSemElemSet in the semiring
    /// definition: here extend (resp. combine) does key-wise extend
    /// (combine) instead of an all-pairs extend (union). This also
    /// stores the default value.
    ///
    /// Thus, if I denote an element by <k,w> where k is the key and w the
    /// weight:
    ///
    ///   o  zero is the empty set {} with default 0
    ///   o  one is the empty set {} with default 1
    ///   o  combine does key-wise combine
    ///   o  extend does key-wise extend
    ///
    /// The key is treated as a sem_elem_t, but it really doesn't have to be
    /// one -- in particular, keys are never either extended or combined.
    class TraceSplitSemElem
      : public wali::SemElem
    {
    public:
      typedef wali::util::unordered_map<Guard::Ptr, sem_elem_t,
                                        GuardRefPtrHash, GuardRefPtrEqual>
              BackingMap;

      typedef BackingMap::const_iterator const_iterator;

      typedef boost::function<void (Guard::Ptr &, sem_elem_t &)> ReduceFunction;

      static
      void
      identity_reducer(Guard::Ptr & UNUSED_PARAMETER(key),
                       sem_elem_t & UNUSED_PARAMETER(value))
      {
      }


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
      normalize(ReduceFunction reducer, sem_elem_t default_value, BackingMap m)
      {
        for (BackingMap::const_iterator it = m.begin();
             it != m.end(); /*nothing*/ )
        {
          Guard::Ptr guard = it->first;
          sem_elem_t weight = it->second;

          reducer(guard, weight);

          if (guard->isFalse() || weight->equal(default_value)) {
            it = m.erase(it);
          }
          else {
            ++it;
          }
        }
        return m;
      }
      

      TraceSplitSemElem(ReduceFunction reducer,
                        BackingMap const & m,
                        sem_elem_t default_value)
        : reducer_(reducer)
        , map_(normalize(reducer, default_value, m))
        , default_(default_value)
        , hash_(get_hash(map_))
      {}

      sem_elem_t
      getWeight(Guard::Ptr key) const {
        const_iterator loc = map_.find(key);
        if (loc == map_.end()) {
          return default_;
        }
        else {
          return loc->second;
        }
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
        return new TraceSplitSemElem(reducer_, m, default_->one());
      }
      
      sem_elem_t zero() const {
        BackingMap m;
        return new TraceSplitSemElem(reducer_, m, default_->zero());
      }

      sem_elem_t extend(SemElem * se) {
        abort(); // this function is not currently used and I don't want to
                 // write tests for it at the moment
        TraceSplitSemElem * that = dynamic_cast<TraceSplitSemElem*>(se);
        assert(that);
        BackingMap m;
        
        for (BackingMap::const_iterator this_pair = this->map_.begin();
             this_pair != this->map_.end(); ++this_pair)
        {
          Guard::Ptr guard = this_pair->first;
          sem_elem_t this_weight = this_pair->second;
          sem_elem_t that_weight = that->getWeight(guard);
          m[guard] = this_weight->extend(that_weight);
        }

        for (BackingMap::const_iterator that_pair = that->map_.begin();
             that_pair != that->map_.end(); ++that_pair)
        {
          Guard::Ptr guard = that_pair->first;
          sem_elem_t that_weight = that_pair->second;
          if (map_.count(guard) == 0) {
            sem_elem_t this_weight = this->default_;
            m[guard] = this_weight->extend(that_weight);
          }
        }

        sem_elem_t new_default = this->default_->extend(that->default_);

        return new TraceSplitSemElem(reducer_, map_, new_default);
      }
      
      sem_elem_t combine(SemElem * se) {
        abort(); // this function is not currently used and I don't want to
                 // write tests for it at the moment
        TraceSplitSemElem * that = dynamic_cast<TraceSplitSemElem*>(se);
        assert(that);
        BackingMap m;
        
        for (BackingMap::const_iterator this_pair = this->map_.begin();
             this_pair != this->map_.end(); ++this_pair)
        {
          Guard::Ptr guard = this_pair->first;
          sem_elem_t this_weight = this_pair->second;
          sem_elem_t that_weight = that->getWeight(guard);
          m[guard] = this_weight->combine(that_weight);
        }

        for (BackingMap::const_iterator that_pair = that->map_.begin();
             that_pair != that->map_.end(); ++that_pair)
        {
          Guard::Ptr guard = that_pair->first;
          sem_elem_t that_weight = that_pair->second;
          if (map_.count(guard) == 0) {
            sem_elem_t this_weight = this->default_;
            m[guard] = this_weight->combine(that_weight);
          }
        }

        sem_elem_t new_default = this->default_->combine(that->default_);

        return new TraceSplitSemElem(reducer_, map_, new_default);
      }


      bool equal(SemElem * se) const {
        TraceSplitSemElem * that = dynamic_cast<TraceSplitSemElem*>(se);
        assert(that);

        if (this->map_.size() != that->map_.size()
            || !this->default_->equal(that->default_))
        {
          return false;
        }

        for (BackingMap::const_iterator this_pair = this->map_.begin();
             this_pair != this->map_.end(); ++this_pair)
        {
          Guard::Ptr guard = this_pair->first;
          sem_elem_t this_weight = this_pair->second;
          sem_elem_t that_weight = that->getWeight(guard);
          if (!this_weight->equal(that_weight)) {
            return false;
          }
        }

        for (BackingMap::const_iterator that_pair = that->map_.begin();
             that_pair != that->map_.end(); ++that_pair)
        {
          Guard::Ptr guard = that_pair->first;
          sem_elem_t that_weight = that_pair->second;
          if (map_.count(guard) == 0) {
            sem_elem_t this_weight = this->default_;
            if (!this_weight->equal(that_weight)) {
              return false;
            }
          }
          // (else, we checked it above)
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
          os << "(some guard) -> ";
          os << begin()->second->print_typename(os);
        }
        return os << ">";
      }

      // Pull in the sem_elem_t overloads
      using SemElem::extend;
      using SemElem::combine;
      using SemElem::equal;

    private:      
      ReduceFunction reducer_;
      BackingMap map_;
      sem_elem_t default_;
      size_t hash_;
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

