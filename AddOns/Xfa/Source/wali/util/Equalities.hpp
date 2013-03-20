#ifndef WALI_DOMAINS_EQUALITIES_HPP
#define WALI_DOMAINS_EQUALITIES_HPP

#include "wali/SemElem.hpp"

#include "wali/util/DisjointSets.hpp"
#include <string>
#include <map>
#include <set>

namespace wali {
  namespace util {

    /// This class tracks sets of equalities and disequalities between
    /// variables (represented as strings).
    class Equalities
      : public wali::SemElem
    {
    public:
      typedef std::string Variable;

    private:
      enum Special { SpecialOne, SpecialZero };
      boost::optional<Special> _special;

      boost::optional<DisjointSets<Variable> > _equalities;
      boost::optional<std::map<Variable, std::set<Variable> > > _disequalities;

      void
      set_to_zero() {
        _special = SpecialZero;
        _equalities.reset();
        _disequalities.reset();
      }

      Equalities(Special s)
        : _special(s)
      {}

      bool valid() const {
        if (_special) {
          return !_equalities && !_disequalities;
        }
        else {
          return _equalities && _disequalities;
        }
      }
      
    public:
      Equalities()
        : _equalities(DisjointSets<Variable>())
        , _disequalities(std::map<Variable, std::set<Variable> >())
      {}

      bool isZero() const {
        assert(valid());
        return _special && *_special == SpecialZero;
      }

      bool isOne() const {
        assert(valid());
        return _special && *_special == SpecialOne;
      }

      sem_elem_t
      zero() const {
        static sem_elem_t z = new Equalities(SpecialZero);
        return z;
      }

      sem_elem_t
      one() const {
        static sem_elem_t o = new Equalities(SpecialOne);
        return o;
      }

      sem_elem_t
      extend(SemElem * other) {
        Equalities * that = dynamic_cast<Equalities*>(other);
        if (this->isOne()) {
          assert(that->count > 0);
          return that;
        }
        if (that->isOne()) {
          assert(this->count > 0);
          return this;
        }
        if (this->equal(that)) {
          assert(this->count > 0);
          return this;
        }
        return zero();
      }

      sem_elem_t
      combine(SemElem * other) {
        Equalities * that = dynamic_cast<Equalities*>(other);
        if (this->isZero()) {
          assert(that->count > 0);
          return that;
        }
        if (that->isZero()) {
          assert(this->count > 0);
          return this;
        }
        if (this->equal(that)) {
          assert(this->count > 0);
          return this;
        }
        return zero();
      }

      bool equal(SemElem * se) const {
        Equalities * that = dynamic_cast<Equalities*>(se);
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

        return (*this->_equalities == *that->_equalities)
          && (*this->_disequalities == *that->_disequalities);
      }

      void
      add_equality(Variable const & left,
		   Variable const & right)
      {
	bool left_inserted = _equalities->insert(left);
	bool right_inserted = _equalities->insert(right);

	if (!left_inserted && !right_inserted) {
	  // Must check for collisions
	  if ((*_disequalities)[left].count(right) > 0) {
	    set_to_zero();
	  }
	}

	_equalities->merge_sets(left, right);
      }

      void
      add_disequality(Variable const & left,
		      Variable const & right)
      {
	bool left_inserted = _equalities->insert(left);
	bool right_inserted = _equalities->insert(right);

	if (!left_inserted && !right_inserted) {
	  // Must check for collisions
	  if (_equalities->representative(left)
	      == _equalities->representative(right))
          {
            set_to_zero();
          }
	}

        (*_disequalities)[left].insert(right);
        (*_disequalities)[right].insert(left);
      }

      std::ostream& print( std::ostream & os ) const {
        assert(valid());
        if (isZero()) {
          os << "[ZERO]";
        }
        else if (isOne()) {
          os << "[ONE]";
        }
        else {
          os << "[stuff]";
        }
        return os;
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
