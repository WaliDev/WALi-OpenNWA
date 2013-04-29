#ifndef WALI_DOMAINS_REPRESENTATIVE_STRING_HPP
#define WALI_DOMAINS_REPRESENTATIVE_STRING_HPP

#include <string>
#include <boost/optional.hpp>

#include "wali/SemElem.hpp"

namespace wali {
  namespace domains{

    class RepresentativeString
      : public SemElem
    {
      boost::optional<std::string> _value;

    public:
      //---------------------
      // Constructors 
      //---------------------
      RepresentativeString()
      {}

      RepresentativeString(std::string const & value)
        : _value(value)
      {}

      RepresentativeString(boost::optional<std::string> value)
        : _value(value)
      {}

      //-----------------------------
      // semiring one and zero
      //-----------------------------
      sem_elem_t one() const;

      sem_elem_t zero() const;

      //---------------------------------
      // semiring operations
      //---------------------------------
      sem_elem_t extend( SemElem* rhs );

      sem_elem_t combine( SemElem* rhs );

      bool equal(SemElem *rhs) const;

      bool containerLessThan(SemElem const * rhs) const;

      size_t hash() const;

      //------------------------------------
      // output
      //------------------------------------
      std::ostream & print(std::ostream &out) const;
    };

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif /* WALI_DOMAINS_REPRESENTATIVE_STRING_HPP */
