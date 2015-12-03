#ifndef WALI_DOMAINS_MATRIX_MATRIX_TEMPLATE_HPP
#define WALI_DOMAINS_MATRIX_MATRIX_TEMPLATE_HPP

#include <vector>
#include <iosfwd>

// We use boost::container::vector because of vector<bool>. Boost's
// version isn't dumb.
#include <boost/container/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <wali/SemElem.hpp>

namespace wali {
  namespace domains {

    template<typename ElementType>
    class Matrix
      : public SemElem
    {
    public:
      typedef ElementType value_type;
      typedef boost::numeric::ublas::matrix<value_type, 
                                            boost::numeric::ublas::row_major,
                                            boost::container::vector<value_type> >
              BackingMatrix;

      Matrix(BackingMatrix const & mat);

      BackingMatrix const &
      matrix() const;

      Matrix *
      zero_raw() const;

      Matrix *
      one_raw() const;

      Matrix *
      extend_raw(Matrix * rhs) const;

      Matrix *
      combine_raw(Matrix * rhs) const;

      bool
      equal(Matrix * rhs) const;

      std::ostream &
      print(std::ostream & stream) const;


      // Here are the "normal" SemElem functions that wrap those above
      virtual sem_elem_t one() const;
      virtual sem_elem_t zero() const;
      virtual sem_elem_t extend(SemElem * se);
      virtual sem_elem_t combine(SemElem * se);
      virtual bool equal(SemElem * se) const;

    private:
      Matrix* down(SemElem* se) const;

      BackingMatrix m_matrix;
    };


    typedef Matrix<bool> BoolMatrix;

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
