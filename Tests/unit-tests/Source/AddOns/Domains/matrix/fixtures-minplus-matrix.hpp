#ifndef ADDONS_DOMAINS_MATRIX_MINPLUS_FIXTURES_HPP
#define ADDONS_DOMAINS_MATRIX_MINPLUS_FIXTURES_HPP

namespace testing
{
namespace minplus_matrix
{

using namespace boost::numeric::ublas;
using namespace wali::domains;

const int inf = std::numeric_limits<MinPlusIntMatrix::value_type::value_type>::max();

struct RandomMatrix1_3x3
{
    MinPlusIntMatrix::BackingMatrix mat;

    RandomMatrix1_3x3()
        : mat(3,3)
    {
        int m[3][3] = {
            {   1, inf, inf},
            {  12,   0,   1},
            { inf, inf,  17}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j).set_value(m[i][j]);
            }
        }
    }
};


struct RandomMatrix2_3x3
{
    MinPlusIntMatrix::BackingMatrix mat;

    RandomMatrix2_3x3()
        : mat(3,3)
    {
        int m[3][3] = {
            { inf,   0,  21},
            {   1,  17,   0},
            {   0,   7,  12}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j).set_value(m[i][j]);
            }
        }
    }
};


struct ExtendR1R2_3x3
{
    MinPlusIntMatrix::BackingMatrix mat;

    ExtendR1R2_3x3()
        : mat(3,3)
    {
        int m[3][3] = {
            { inf,   1,  22},
            {   1,   8,   0},
            {  17,  24,  29}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j).set_value(m[i][j]);
            }
        }
    }
};


struct ExtendR2R1_3x3
{
    MinPlusIntMatrix::BackingMatrix mat;

    ExtendR2R1_3x3()
        : mat(3,3)
    {
        int m[3][3] = {
            { 12,   0,   1},
            {  2,  17,  17},
            {  1,   7,   8}
        };

        for (size_t i = 0; i< mat.size1(); ++i) {
            for (size_t j = 0; j < mat.size2(); ++j) {
                mat(i, j).set_value(m[i][j]);
            }
        }
    }
};

typedef ExtendR2R1_3x3 CombineR1Id_3x3;


struct ZeroBackingMatrix_3x3
{
    MinPlusIntMatrix::BackingMatrix mat;

    ZeroBackingMatrix_3x3()
        : mat(zero_matrix<MinPlusIntMatrix::value_type>(3, 3))
    {}
};


struct IdBackingMatrix_3x3
{
    MinPlusIntMatrix::BackingMatrix mat;

    IdBackingMatrix_3x3()
        : mat(identity_matrix<MinPlusIntMatrix::value_type>(3, 3))
    {}
};


struct MatrixFixtures_3x3
{
    RandomMatrix1_3x3 r1;
    RandomMatrix2_3x3 r2;
    ExtendR1R2_3x3 ext_r1_r2;
    ExtendR2R1_3x3 ext_r2_r1;
    IdBackingMatrix_3x3 id;
    ZeroBackingMatrix_3x3 zero;
};

}
}

#endif
