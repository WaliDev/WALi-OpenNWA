#include "wali/domains/matrix/Matrix_template.hpp"
#include "wali/domains/matrix/Matrix_template.hxx"

#include "wali/domains/matrix/MinPlus.hpp"
#include "wali/domains/matrix/SemElemWrapper.hpp"

namespace wali {
  namespace domains {

    template class Matrix<bool>;
    template class Matrix<details::MinPlus<int> >;
    template class Matrix<details::SemElemWrapper>;

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
