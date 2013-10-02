#ifndef WALI_DOMAINS_REVERSED_SEM_ELEM_HPP
#define WALI_DOMAINS_REVERSED_SEM_ELEM_HPP

#include <limits>
#include <boost/function.hpp>
#include "wali/SemElem.hpp"

namespace wali
{
  namespace domains
  {

    class ReversedSemElem
      : public SemElem
    {
    private:
      sem_elem_t backing_elem_;

    public:
      ReversedSemElem(sem_elem_t backing)
        : backing_elem_(backing)
      {}

      ReversedSemElem(ReversedSemElem const & other)
        : backing_elem_(other.backing_elem_)
      {}

      void
      operator= (ReversedSemElem const & other)
      {
        backing_elem_ = other.backing_elem_;
      }

      sem_elem_t backingSemElem() const
      {
        return backing_elem_;
      }

      sem_elem_t one() const
      {
        return new ReversedSemElem(backing_elem_->one());
      }

      sem_elem_t zero() const
      {
        return new ReversedSemElem(backing_elem_->zero());
      }

      sem_elem_t extend(SemElem * se)
      {
        ReversedSemElem * that = dynamic_cast<ReversedSemElem*>(se);
        assert(that);
        
        return new ReversedSemElem(that->backing_elem_->extend(this->backing_elem_));
      }

      sem_elem_t combine( SemElem * se )
      {
        ReversedSemElem * that = dynamic_cast<ReversedSemElem*>(se);
        assert(that);

        return new ReversedSemElem(this->backing_elem_->combine(that->backing_elem_));
      }

      bool equal( SemElem * se ) const
      {
        ReversedSemElem * that = dynamic_cast<ReversedSemElem*>(se);
        assert(that);

        return this->backing_elem_->equal(that->backing_elem_);
      }

      bool underApproximates(SemElem * se)
      {
        ReversedSemElem * that = dynamic_cast<ReversedSemElem*>(se);
        assert(that);

        return this->backing_elem_->underApproximates(that->backing_elem_);
      }

      std::ostream& print( std::ostream & o ) const
      {
        return backing_elem_->print(o << "R[") << "]";
      }

      std::ostream& marshall( std::ostream& o ) const
      {
        return backing_elem_->marshall(o);
      }

      sem_elem_t diff( SemElem * se )
      {
        ReversedSemElem * that = dynamic_cast<ReversedSemElem*>(se);
        assert(that);

        return new ReversedSemElem(this->backing_elem_->diff(that->backing_elem_));
      }

      sem_elem_t quasi_one() const
      {
        return new ReversedSemElem(backing_elem_->quasi_one());
      }

      std::pair<sem_elem_t,sem_elem_t> delta( SemElem * se )
      {
        ReversedSemElem * that = dynamic_cast<ReversedSemElem*>(se);
        assert(that);

        std::pair<sem_elem_t, sem_elem_t> ans = this->backing_elem_->delta(that->backing_elem_);

        return std::pair<sem_elem_t, sem_elem_t>(new ReversedSemElem(ans.first),
                                                 new ReversedSemElem(ans.second));
      }

      sem_elem_t star()
      {
        return new ReversedSemElem(backing_elem_->star());
      }

      sem_elem_t
      extendAndDiff(sem_elem_t next, sem_elem_t subtrahend)
      {
        ReversedSemElem * next_down = dynamic_cast<ReversedSemElem*>(next.get_ptr());
        assert(next_down);

        ReversedSemElem * next_sub = dynamic_cast<ReversedSemElem*>(subtrahend.get_ptr());
        assert(next_sub);

        return new ReversedSemElem(next_down->backing_elem_->extendAndDiff(this->backing_elem_,
                                                                           next_sub->backing_elem_));
      }

      bool
      containerLessThan(SemElem const * other) const
      {
        ReversedSemElem const * that = dynamic_cast<ReversedSemElem const *>(other);
        assert(that);

        return this->backing_elem_->containerLessThan(that->backing_elem_);
      }

      size_t
      hash() const
      {
        return std::numeric_limits<size_t>::max() - backing_elem_->hash();
      }

      std::ostream &
      print_typename(std::ostream & os) const
      {
        return backing_elem_->print(os << "ReversedSemElem[") << "]";
      }
    };


    inline
    sem_elem_t
    wrapToReversedSemElem(sem_elem_t se)
    {
      return new ReversedSemElem(se);
    }

  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif

