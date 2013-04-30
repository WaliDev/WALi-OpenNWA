#include "wali/domains/RepresentativeString.hpp"

#include <utility>
#include <boost/functional/hash.hpp>

namespace wali {
  namespace domains {

    namespace {
      typedef boost::optional<std::string> OptString;
    
      OptString
      concat(OptString const & left,
             OptString const & right)
      {
        if (left && right) {
          return *left + *right;
        }
        else {
          // x * 0, 0 * x, or 0 * 0
          return OptString();
        }
      }

      OptString
      min(OptString const & left,
          OptString const & right)
      {
        if (left && right) {
          if (left->size() < right->size()) {
            return left;
          }
          if (left->size() == right->size()) {
            return std::min(*left, *right);
          }
          return right;
        }
        if (left) {
          return left;
        }
        return right;
      }
    }

  
    sem_elem_t RepresentativeString::one() const
    {
      static sem_elem_t elem = new RepresentativeString("");
      return elem;
    }

    sem_elem_t RepresentativeString::zero() const
    {
      static sem_elem_t z = new RepresentativeString();
      return z;
    }


    //---------------------------------
    // semiring operations
    //---------------------------------
    sem_elem_t RepresentativeString::extend( SemElem* rhs )
    {
      assert(dynamic_cast<RepresentativeString*>(rhs));
      RepresentativeString* that = static_cast< RepresentativeString* >(rhs);

      return new RepresentativeString(concat(this->_value, that->_value));
    }

    sem_elem_t RepresentativeString::combine( SemElem* rhs )
    {
      assert(dynamic_cast<RepresentativeString*>(rhs));
      RepresentativeString* that = static_cast< RepresentativeString* >(rhs); 

      return new RepresentativeString(min(this->_value, that->_value));
    }


    bool RepresentativeString::equal(SemElem *rhs) const
    {
      assert(dynamic_cast<RepresentativeString*>(rhs));
      RepresentativeString* that = static_cast< RepresentativeString* >(rhs);

      if (!this->_value && !that->_value) {
        // both zero
        return true;
      }
      if (this->_value && that->_value) {
        // both nonzero
        return *this->_value == *that->_value;
      }
      return false;
    }

  
    bool RepresentativeString::containerLessThan(SemElem const *rhs) const
    {
      assert(dynamic_cast<RepresentativeString const *>(rhs));
      RepresentativeString const * that = static_cast< RepresentativeString const * >(rhs);

      return this->_value < that->_value;
    }


    size_t RepresentativeString::hash() const
    {
      if (_value) {
        boost::hash<std::string> hasher;
        return hasher(*_value);
      }
      // A random number. (Not kidding: I got it from random.org!)
      return 239023451;
    }

  
    //------------------------------------
    // output
    //------------------------------------
    std::ostream & RepresentativeString::print(std::ostream &out) const
    {
      if (_value) {
        out << "RepresentativeString(" << *_value << ")";
      }
      else {
        out << "RepresentativeString::zero";
      }
      return out;
    }

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
