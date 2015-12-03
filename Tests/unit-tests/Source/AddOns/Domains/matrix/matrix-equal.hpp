#ifndef WALI_TESTS_DOMAIN_MATRIX_EQUAL_HPP
#define WALI_TESTS_DOMAIN_MATRIX_EQUAL_HPP

namespace boost {
namespace numeric {
namespace ublas {

template<typename T, typename O, typename S>
bool operator==(matrix<T,O,S> const & m, matrix<T,O,S> const & n)
{
    if (m.size1() != n.size1()
        || m.size2() != n.size2())
    {
        return false;
    }

    for (size_t i=0; i<m.size1(); ++i) {
        for (size_t j=0; j<m.size2(); ++j) {
            if (m(i,j) != n(i,j))
                return false;
        }
    }

    return true;
}

}
}
}

#endif /* WALI_TESTS_DOMAIN_MATRIX_EQUAL_HPP */

