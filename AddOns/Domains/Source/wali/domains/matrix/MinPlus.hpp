#include <algorithm>
#include <limits>
#include <ostream>

#include "wali/Common.hpp"

namespace wali {
  namespace domains {
    namespace details {

      template <typename BackingUInt>
      class MinPlus
      {
      public:
        typedef BackingUInt value_type;

      private:
        value_type m_value;

        struct DummyTag {};
        MinPlus(value_type v, DummyTag _dummy ATTR_UNUSED)
          : m_value(v)
        {}

      public:
        // This is needed by boost::matrix:
        //    template<class T, class ALLOC>
        //    const typename identity_matrix<T, ALLOC>::value_type
        //        identity_matrix<T, ALLOC>::one_ (1);
        //    // ISSUE: need 'one'-traits here
        //
        // The zero element for identity_matrix and zero_matrix
        // are constructed by default parameter, so this should be
        // the only call to this function.
        //
        // Once the boost thing is resolved, this should become
        // private. (Or really I should figure out how to make it
        // a friend.)
        explicit MinPlus(int t)
          : m_value()
        {
          if (t == 0) {
            m_value = infinity();
          }
          else if (t == 1) {
            m_value = 0;
          }
          else {
            abort(); // remove this from official WALi version
            fast_assert(false);
            m_value = infinity();
          }
        }

        static MinPlus make(value_type v) {
          MinPlus ret;
          ret.set_value(v);
          return ret;
        }

        MinPlus()
          : m_value(infinity())
        {}

        MinPlus(MinPlus const & other)
          : m_value(other.m_value)
        {}

        MinPlus &
        operator= (MinPlus const & other) {
          m_value = other.m_value;
          return *this;
        }

#if 0 // is this function a good idea? I tend no..
        MinPlus const &
        operator= (value_type v) {
          fast_assert(v != infinity());
          m_value = v;
          return *this;
        }
#endif

        bool
        operator== (MinPlus const & that) const {
          return this->m_value == that.m_value;
        }

        bool
        operator!= (MinPlus const & that) const {
          return !(*this == that);
        }

        value_type
        infinity() const {
#define INHIBIT_MACRO_EXPANSION
          return std::numeric_limits<value_type>::max INHIBIT_MACRO_EXPANSION ();
#udef INHIBIT_MACRO_EXPANSION
        }

        bool
        is_infinite() const {
          return m_value == infinity();
        }

        value_type
        get_value() const {
          return m_value;
        }

        // The "value" is the actual distance. That is, 0 is a
        // distance of 0, not the semiring 0 (which is distance
        // infinity). 1 is a distance of 1, not the semiring 1
        // (which is distance 0).
        void
        set_value(value_type v) {
          m_value = v;
        }

        MinPlus
        operator+ (MinPlus const & that) const {
          using std::min;
          return make(min(this->m_value, that.m_value));
        }

        void
        operator+= (MinPlus const & that) {
          using std::min;
          this->m_value = min(this->m_value, that.m_value);
        }


        MinPlus
        operator* (MinPlus const & that) const {
          if (this->m_value == infinity() || that.m_value == infinity()) {
            return MinPlus();
          }
          // TODO: check for OF?
          return make(this->m_value + that.m_value);
        }

        void
        operator*= (MinPlus const & that) {
          if (this->m_value == infinity() || that.m_value == infinity()) {
            this->m_value = infinity();
            return;
          }
          // TODO: check for OF?
          this->m_value = this->m_value + that.m_value;
        }
      };


      template <typename BackingInt>
      std::ostream &
      operator<< (std::ostream & os, MinPlus<BackingInt> v)
      {
        if (v.get_value() == v.infinity()) {
          return os << "[infinity]";
        }
        else {
          return os << "[" << v.get_value() << "]";
        }
      }

    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
