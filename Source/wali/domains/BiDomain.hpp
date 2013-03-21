#ifndef WALI_DOMAINS_BIDOMAIN_HPP
#define WALI_DOMAINS_BIDOMAIN_HPP

namespace wali {
  namespace domains {

    template<typename RuleSemElem,
             typename TransSemElem>
    class BiDomain
      : public wali::SemElem
    {
    public:

      typedef RuleSemElem RuleSemElem;
      typedef TransSemElem WfaSemElem;

      typedef boost::function<ref_ptr<WfaSemElem>(TransSemElem *, RuleSemElem *)>
              ExtendFunction;

      BiDomain(ExtendFunction extend, sem_elem_t val)
        : _value(val)
      {}

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
        BiDomain * that = dynamic_cast<BiDomain*>(other);
        assert(that);
        return this->_value->equal(that->_value);
      }

      virtual bool underApproximates(SemElem * that) {
        BiDomain * that = dynamic_cast<BiDomain*>(other);
        assert(that);
        return this->_value->underApproximates(that->_value);
      }

      virtual std::ostream& print( std::ostream & o ) const {
        return _value->print(o);
      }
      
      virtual bool containerLessThan(SemElem * that) {
        BiDomain * that = dynamic_cast<BiDomain*>(other);
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
