#ifndef WALI_DOMAINS_MATRIX_RANKER_HPP
#define WALI_DOMAINS_MATRIX_RANKER_HPP

#include "opennwa/query/ShortWitnessVisitor.hpp"

namespace wali {
  namespace domains {

    template <typename MatrixDomain>
    class MatrixRanker : public opennwa::query::WitnessRanker
    {
    public:
      typedef typename MatrixDomain::BackingMatrix BackingMatrix;
      typedef typename BackingMatrix::value_type value_type;

      MatrixRanker(size_t size1, size_t size2)
        : m_from(1, size1)
        , m_to(size2, 1)
      {}

      explicit MatrixRanker(size_t square_size)
        : m_from(1, square_size)
        , m_to(square_size, 1)
      {}

      void set_initial(size_t coord, value_type v)
      {
        m_from(0, coord) = v;
      }

      void set_final(size_t coord, value_type v)
      {
        m_to(coord, 0) = v;
      }

      virtual unsigned long
      get_rank(wali::witness::Witness * w) const
      {
        BackingMatrix
          mat = dynamic_cast<MinPlusIntMatrix*>(w->weight().get_ptr())->matrix(),
          p1 = prod(m_from, mat), // ah, boost; need to make a temporary
          p = prod(p1, m_to);

        return p(0,0).get_value();
      }

    private:
      BackingMatrix m_from, m_to;
    };

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif /* WALI_DOMAINS_MATRIX_RANKER_HPP */
