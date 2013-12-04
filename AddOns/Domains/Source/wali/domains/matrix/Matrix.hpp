#ifndef WALI_DOMAINS_MATRIX_MATRIX_HPP
#define WALI_DOMAINS_MATRIX_MATRIX_HPP

#include <vector>
#include <iosfwd>

// We use boost::container::vector because of vector<bool>. Boost's
// version isn't dumb.
#include <boost/container/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <wali/SemElem.hpp>

namespace wali {
  namespace domains {

    class BoolMatrix
      : public SemElem
    {
    public:
      typedef bool value_type;
      typedef boost::numeric::ublas::matrix<value_type, 
                                            boost::numeric::ublas::row_major,
                                            boost::container::vector<value_type> >
              BackingMatrix;

      BoolMatrix(BackingMatrix const & mat);

      BackingMatrix const &
      matrix() const;

      BoolMatrix *
      zero_raw() const;

      BoolMatrix *
      one_raw() const;

      BoolMatrix *
      extend_raw(BoolMatrix * rhs) const;

      BoolMatrix *
      combine_raw(BoolMatrix * rhs) const;

      bool
      equal(BoolMatrix * rhs) const;

      std::ostream &
      print(std::ostream & stream) const;


      // Here are the "normal" SemElem functions that wrap those above
      virtual sem_elem_t one() const;
      virtual sem_elem_t zero() const;
      virtual sem_elem_t extend(SemElem * se);
      virtual sem_elem_t combine(SemElem * se);
      virtual bool equal(SemElem * se) const;

    private:
      BoolMatrix* down(SemElem* se) const;

      BackingMatrix m_matrix;
    };

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
