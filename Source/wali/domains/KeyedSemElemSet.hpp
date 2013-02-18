#ifndef WALI_KEYED_SEMELEMSET_HPP
#define WALI_KEYED_SEMELEMSET_HPP

#include "wali/SemElem.hpp"

#include <boost/optional.hpp>
#include <tr1/unordered_map>
#include <limits>

namespace wali
{
  namespace domains
  {

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
      boost::optional<PositionType> pre_, post_;

    public:
      // Constructs our representation of zero.
      PositionKey() {}

      PositionKey(PositionType const & pre, PositionType const & post)
        : pre_(pre)
        , post_(post)
      {}

      PositionType pre() const {
        assert(!isZero());
        return *pre_;
      }

      PositionType post() const {
        assert(!isZero());
        return *post_;
      }

      bool isZero() const {
        assert((pre_ && post_) || (!pre_ && !post_));
        return !pre_;
      }

      ///
      /// Guaranteed to abort!
      sem_elem_t one() const {
        abort();
        return NULL; // MSVS
      }

      ///
      /// This actually returns zero
      sem_elem_t zero() const {
        static sem_elem_t z = new PositionKey();
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
        if (*this->post_ != *that->pre_) {
          return zero();
        }
        return new PositionKey(*this->pre_, *that->post_);
      }

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
        return (*this->pre_ == *that->pre_)
          && (*this->post_ == *that->post_);
      }

      ///
      /// This actually prints the guard
      std::ostream& print( std::ostream & os ) const {
        if (isZero()) {
          os << "[ZERO]";
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
