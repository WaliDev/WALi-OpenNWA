#ifndef WALI_DOMAINS_EQUALITIES_HPP
#define WALI_DOMAINS_EQUALITIES_HPP

#include "wali/util/DisjointSets.hpp"
#include "wali/domains/TraceSplitSemElem.hpp"
#include <string>
#include <map>
#include <set>

namespace wali {
  namespace util {

    /// This class tracks sets of equalities and disequalities between
    /// variables (represented as strings).
    class Equalities
      : public wali::domains::Guard
    {
    public:
      typedef std::string Variable;

    private:
      enum Special { SpecialZero };
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

      Equalities(Equalities const & that)
        : _equalities(that._equalities)
        , _disequalities(that._disequalities)
      {}

      Equalities *
      clone() const {
        return new Equalities(*this);
      }

      bool isFalse() const {
        assert(valid());
        return _special && *_special == SpecialZero;
      }

      domains::Guard::Ptr
      zero() const {
        static domains::Guard::Ptr z = new Equalities(SpecialZero);
        return z;
      }

      size_t hash() const {
        abort();
      }

      bool equal(wali::domains::Guard::Ptr se) const {
        Equalities * that = dynamic_cast<Equalities*>(se.get_ptr());
        assert(that != NULL);

        if (this->isFalse() && that->isFalse()) {
          return true;
        }
        if (this->isFalse() || that->isFalse()) {
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
        if (isFalse()) {
          os << "[ZERO]";
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
