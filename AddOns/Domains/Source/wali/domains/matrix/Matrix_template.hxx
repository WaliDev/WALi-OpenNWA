#ifndef MATRIX_TEMPLATE_HXX
#define MATRIX_TEMPLATE_HXX

#include <wali/Common.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <ostream>

using namespace boost::numeric::ublas;

namespace wali {
  namespace domains {

    template<typename ElementType>
    Matrix<ElementType>::Matrix(BackingMatrix const & m)
      : m_matrix(m)
    {}


    template<typename ElementType>
    typename Matrix<ElementType>::BackingMatrix const &
    Matrix<ElementType>::matrix() const
    {
      return m_matrix;
    }


    template<typename ElementType>
    Matrix<ElementType>*
    Matrix<ElementType>::zero_raw() const
    {
      zero_matrix<value_type> m(m_matrix.size1(), m_matrix.size2());
      return new Matrix(m);
    }


    template<typename ElementType>
    Matrix<ElementType>*
    Matrix<ElementType>::one_raw() const
    {
      identity_matrix<value_type> m(m_matrix.size1(), m_matrix.size2());
      return new Matrix(m);
    }


    template<typename ElementType>
    Matrix<ElementType>*
    Matrix<ElementType>::extend_raw(Matrix * that) const
    {
      return new Matrix(prod(this->matrix(), that->matrix()));
    }


    template<typename ElementType>
    Matrix<ElementType>*
    Matrix<ElementType>::combine_raw(Matrix * that) const
    {
      return new Matrix(this->matrix() + that->matrix());
    }


    template<typename ElementType>
    bool
    Matrix<ElementType>::equal(Matrix * that) const
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


    template<typename ElementType>
    std::ostream &
    Matrix<ElementType>::print(std::ostream & stream) const
    {
      stream << "Matrix: " << matrix();
      return stream;
    }


    template<typename ElementType>
    sem_elem_t
    Matrix<ElementType>::one() const
    {
      return one_raw();
    }


    template<typename ElementType>
    sem_elem_t
    Matrix<ElementType>::zero() const
    {
      return zero_raw();
    }


    template<typename ElementType>
    sem_elem_t
    Matrix<ElementType>::extend(SemElem * se)
    {
      return extend_raw(down(se));
    }


    template<typename ElementType>
    sem_elem_t
    Matrix<ElementType>::combine(SemElem * se)
    {
      return combine_raw(down(se));
    }


    template<typename ElementType>
    bool
    Matrix<ElementType>::equal(SemElem * se) const
    {
      return equal(down(se));
    }

    template<typename ElementType>
    Matrix<ElementType>*
    Matrix<ElementType>::down(SemElem* se) const
    {
      Matrix* bm = dynamic_cast<Matrix*>(se);
      fast_assert(bm != NULL);
      return bm;
    }

  }
}

#endif /* MATRIX_TEMPLATE_HXX */

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
