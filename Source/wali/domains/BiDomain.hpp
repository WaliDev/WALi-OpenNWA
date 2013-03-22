#ifndef WALI_DOMAINS_BIDOMAIN_HPP
#define WALI_DOMAINS_BIDOMAIN_HPP

#include <cxxabi.h>
#include <ostream>
#include <boost/function.hpp>
#include "wali/SemElem.hpp"

namespace wali {
  namespace domains {

    inline
    std::string
    demangle(std::string const & mangled) {
      int out;
      char* demangled = abi::__cxa_demangle(mangled.c_str(), NULL, NULL, &out);
      assert(demangled);
      std::string ret = demangled;
      free(demangled);
      return ret;
    }

    template<typename RuleSemElem_,
             typename TransSemElem_>
    class BiDomain
      : public wali::SemElem
    {
    public:

      typedef RuleSemElem_ RuleSemElem;
      typedef TransSemElem_ WfaSemElem;

      typedef boost::function<ref_ptr<WfaSemElem>(WfaSemElem *, RuleSemElem *)>
              ExtendFunction;

      BiDomain(ExtendFunction extend, sem_elem_t val)
        : _value(val)
      {}

      virtual
      std::ostream &
      print_typename(std::ostream & os) const {
        os << "BiDomain<"
           << demangle(typeid(RuleSemElem).name()) << ", "
           << demangle(typeid(WfaSemElem).name()) << "; holding ";
        _value->print_typename(os) << ">";
        return os;
      }
      
      sem_elem_t
      zero() const {
        return _value->zero();
      }

      sem_elem_t
      one() const {
        return _value->one();
      }

      sem_elem_t
      combine(SemElem * other) {
        BiDomain * that = dynamic_cast<BiDomain*>(other);
        assert(that);
        return this->_value->combine(that->_value);
      }

      sem_elem_t
      extend(SemElem * other) {
        BiDomain * that = dynamic_cast<BiDomain*>(other);
        assert(that);
        RuleSemElem * rule = dynamic_cast<RuleSemElem*>(that->_value.get_ptr());
        WfaSemElem * weight = dynamic_cast<WfaSemElem*>(this->_value.get_ptr());
        return _extend(weight, rule);
      }

      virtual bool equal( SemElem * se ) const {
        BiDomain * that = dynamic_cast<BiDomain*>(se);
        assert(that);
        return this->_value->equal(that->_value);
      }

      virtual bool underApproximates(SemElem * se) {
        BiDomain * that = dynamic_cast<BiDomain*>(se);
        assert(that);
        return this->_value->underApproximates(that->_value);
      }

      virtual std::ostream& print( std::ostream & o ) const {
        return _value->print(o);
      }
      
      virtual bool containerLessThan(SemElem * se) {
        BiDomain * that = dynamic_cast<BiDomain*>(se);
        assert(that);
        return this->_value->containerLessThan(that->_value);
      }

      virtual size_t hash() const {
        return _value->hash();
      }

    private:
      ExtendFunction _extend;
      sem_elem_t _value;
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
