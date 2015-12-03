#ifndef ADDONS_DOMAINS_MATRIX_FIXTURES_HPP
#define ADDONS_DOMAINS_MATRIX_FIXTURES_HPP

namespace testing
{
namespace boolmatrix
{

using namespace boost::numeric::ublas;
using namespace wali::domains;

struct RandomMatrix1_3x3
{
    BoolMatrix::BackingMatrix mat;

    RandomMatrix1_3x3()
        : mat(3,3)
    {
        bool m[3][3] = {
            {1, 1, 0},
            {1, 0, 1},
            {0, 0, 1}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j) = m[i][j];
            }
        }
    }
};


struct RandomMatrix2_3x3
{
    BoolMatrix::BackingMatrix mat;

    RandomMatrix2_3x3()
        : mat(3,3)
    {
        bool m[3][3] = {
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 1}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j) = m[i][j];
            }
        }
    }
};


struct ExtendR1R2_3x3
{
    BoolMatrix::BackingMatrix mat;

    ExtendR1R2_3x3()
        : mat(3,3)
    {
        bool m[3][3] = {
            {1, 1, 1},
            {1, 0, 1},
            {0, 0, 1}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j) = m[i][j];
            }
        }
    }
};


struct ExtendR2R1_3x3
{
    BoolMatrix::BackingMatrix mat;

    ExtendR2R1_3x3()
        : mat(3,3)
    {
        bool m[3][3] = {
            {1, 1, 0},
            {1, 1, 1},
            {0, 0, 1}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j) = m[i][j];
            }
        }
    }
};

typedef ExtendR2R1_3x3 CombineR1Id_3x3;


struct ZeroBackingMatrix_3x3
{
    BoolMatrix::BackingMatrix mat;

    ZeroBackingMatrix_3x3()
        : mat(zero_matrix<BoolMatrix::value_type>(3, 3))
    {}
};


struct IdBackingMatrix_3x3
{
    BoolMatrix::BackingMatrix mat;

    IdBackingMatrix_3x3()
        : mat(identity_matrix<BoolMatrix::value_type>(3, 3))
    {}
};


struct MatrixFixtures_3x3
{
    RandomMatrix1_3x3 r1;
    RandomMatrix2_3x3 r2;
    ExtendR1R2_3x3 ext_r1_r2;
    ExtendR2R1_3x3 ext_r2_r1;
    // no CombineR1Id because it is equal to ext_r2_r1
    IdBackingMatrix_3x3 id;
    ZeroBackingMatrix_3x3 zero;
};


}
}

#endif
