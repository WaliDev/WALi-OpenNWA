#include "Matrix.hpp"
#include <wali/Common.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <ostream>

using namespace boost::numeric::ublas;

namespace wali {
  namespace domains {

    BoolMatrix::BoolMatrix(BackingMatrix const & m)
      : m_matrix(m)
    {}


    BoolMatrix::BackingMatrix const &
    BoolMatrix::matrix() const
    {
      return m_matrix;
    }


    BoolMatrix*
    BoolMatrix::zero_raw() const
    {
      zero_matrix<value_type> m(m_matrix.size1(), m_matrix.size2());
      return new BoolMatrix(m);
    }


    BoolMatrix*
    BoolMatrix::one_raw() const
    {
      identity_matrix<value_type> m(m_matrix.size1(), m_matrix.size2());
      return new BoolMatrix(m);
    }


    BoolMatrix*
    BoolMatrix::extend_raw(BoolMatrix * that) const
    {
      return new BoolMatrix(prod(this->matrix(), that->matrix()));
    }


    BoolMatrix*
    BoolMatrix::combine_raw(BoolMatrix * that) const
    {
      return new BoolMatrix(this->matrix() + that->matrix());
    }


    bool
    BoolMatrix::equal(BoolMatrix * that) const
    {
      fast_assert(this->matrix().size1() == that->matrix().size1());
      fast_assert(this->matrix().size2() == that->matrix().size2());

      if (this->matrix().size1() != that->matrix().size1()
          || this->matrix().size2() != that->matrix().size2())
      {
        return false;
      }

      for (size_t row=0; row<matrix().size1(); ++row) {
        for (size_t col=0; col<matrix().size2(); ++col) {
          if (this->matrix()(row, col) != that->matrix()(row, col)) {
            return false;
          }
        }
      }

      return true;
    }


    std::ostream &
    BoolMatrix::print(std::ostream & stream) const
    {
      stream << "Matrix: " << matrix();
      return stream;
    }


    sem_elem_t
    BoolMatrix::one() const
    {
      return one_raw();
    }


    sem_elem_t
    BoolMatrix::zero() const
    {
      return zero_raw();
    }


    sem_elem_t
    BoolMatrix::extend(SemElem * se)
    {
      return extend_raw(down(se));
    }


    sem_elem_t
    BoolMatrix::combine(SemElem * se)
    {
      return combine_raw(down(se));
    }


    bool
    BoolMatrix::equal(SemElem * se) const
    {
      return equal(down(se));
    }

    BoolMatrix*
    BoolMatrix::down(SemElem* se) const
    {
      BoolMatrix* bm = dynamic_cast<BoolMatrix*>(se);
      fast_assert(bm != NULL);
      return bm;
    }

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
