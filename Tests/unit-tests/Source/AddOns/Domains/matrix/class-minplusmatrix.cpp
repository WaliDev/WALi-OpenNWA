#include "gtest/gtest.h"

#include <sstream>
#include <boost/scoped_ptr.hpp>

#include "wali/domains/matrix/Matrix.hpp"

#include "fixtures-minplus-matrix.hpp"
#include "matrix-equal.hpp"

using namespace testing::minplus_matrix;

namespace wali {
namespace domains {

TEST(wali$domains$matrix$MinPlusIntMatrix$$constructorAndMatrix, basicTest3x3)
{
    RandomMatrix1_3x3 f;
    MinPlusIntMatrix m(f.mat);

    EXPECT_EQ(m.matrix(), f.mat);
}


#define NUM_ELEMENTS(arr) ((sizeof arr)/(sizeof arr[0]))

TEST(wali$domains$matrix$MinPlusIntMatrix$$equalAndIsZeroAndIsOne, battery)
{
    MatrixFixtures_3x3 f;
    MinPlusIntMatrix mats[] = {
        MinPlusIntMatrix(f.zero.mat),
        MinPlusIntMatrix(f.id.mat),
        MinPlusIntMatrix(f.r1.mat),
        MinPlusIntMatrix(f.r2.mat),
        MinPlusIntMatrix(f.ext_r1_r2.mat),
        MinPlusIntMatrix(f.ext_r2_r1.mat),
    };

    for (size_t left=0; left<NUM_ELEMENTS(mats); ++left) {
        for (size_t right=0; right<NUM_ELEMENTS(mats); ++right) {
            EXPECT_EQ(left == right,
                      mats[left].equal(&mats[right]));
        }
    }
}


TEST(wali$domains$matrix$MinPlusIntMatrix$$zero_raw, basicTest3x3)
{
    RandomMatrix1_3x3 f;
    ZeroBackingMatrix_3x3 z;

    MinPlusIntMatrix m(f.mat);
    MinPlusIntMatrix mz(z.mat);

    boost::scoped_ptr<MinPlusIntMatrix> result(m.zero_raw());

    EXPECT_EQ(z.mat, result->matrix());
    EXPECT_TRUE(mz.equal(result.get()));
}


TEST(wali$domains$matrix$MinPlusIntMatrix$$one_raw, basicTest3x3)
{
    RandomMatrix1_3x3 f;
    IdBackingMatrix_3x3 z;

    MinPlusIntMatrix m(f.mat);
    MinPlusIntMatrix mid(z.mat);

    boost::scoped_ptr<MinPlusIntMatrix> result(m.one_raw());

    EXPECT_EQ(z.mat, result->matrix());
    EXPECT_TRUE(mid.equal(result.get()));
}


TEST(wali$domains$matrix$MinPlusIntMatrix$$extend_raw, twoRandomMatrices)
{
    RandomMatrix1_3x3 f1;
    RandomMatrix2_3x3 f2;
    ExtendR1R2_3x3 fr12;
    ExtendR2R1_3x3 fr21;

    MinPlusIntMatrix m1(f1.mat);
    MinPlusIntMatrix m2(f2.mat);

    boost::scoped_ptr<MinPlusIntMatrix> result12(m1.extend_raw(&m2));
    boost::scoped_ptr<MinPlusIntMatrix> result21(m2.extend_raw(&m1));

    EXPECT_EQ(fr12.mat, result12->matrix());
    EXPECT_EQ(fr21.mat, result21->matrix());
}


TEST(wali$domains$matrix$MinPlusIntMatrix$$extend_raw, extendAgainstZero)
{
    RandomMatrix1_3x3 f;
    ZeroBackingMatrix_3x3 z;

    MinPlusIntMatrix mf(f.mat);
    MinPlusIntMatrix mz(z.mat);

    boost::scoped_ptr<MinPlusIntMatrix>
        result1Z(mf.extend_raw(&mz)),
        resultZ1(mz.extend_raw(&mf)),
        resultZZ(mz.extend_raw(&mz));

    EXPECT_EQ(z.mat, result1Z->matrix());
    EXPECT_EQ(z.mat, resultZ1->matrix());
    EXPECT_EQ(z.mat, resultZZ->matrix());

    EXPECT_TRUE(mz.equal(result1Z.get()));
    EXPECT_TRUE(mz.equal(resultZ1.get()));
    EXPECT_TRUE(mz.equal(resultZZ.get()));
}


TEST(wali$domains$matrix$MinPlusIntMatrix$$extend_raw, extendAgainstOne)
{
    RandomMatrix1_3x3 fr1;
    IdBackingMatrix_3x3 id;

    MinPlusIntMatrix mr1(fr1.mat);
    MinPlusIntMatrix mid(id.mat);

    boost::scoped_ptr<MinPlusIntMatrix>
        result_R1_Id(mr1.extend_raw(&mid)),
        result_Id_R1(mid.extend_raw(&mr1)),
        result_Id_Id(mid.extend_raw(&mid));

    EXPECT_EQ(fr1.mat, result_R1_Id->matrix());
    EXPECT_EQ(fr1.mat, result_Id_R1->matrix());
    EXPECT_EQ(id.mat,  result_Id_Id->matrix());

    EXPECT_TRUE(mr1.equal(result_R1_Id.get()));
    EXPECT_TRUE(mr1.equal(result_Id_R1.get()));
    EXPECT_TRUE(mid.equal(result_Id_Id.get()));
}


TEST(wali$domains$matrix$MinPlusIntMatrix$$print, random)
{
    RandomMatrix1_3x3 f;
    MinPlusIntMatrix m(f.mat);
    std::stringstream ss;

    m.print(ss);

    EXPECT_EQ("Matrix: [3,3]"
              "(([1],[infinity],[infinity]),"
              "([12],[0],[1]),"
              "([infinity],[infinity],[17]))",
              ss.str());
}


TEST(wali$domains$matrix$MinPlusIntMatrix, callWaliTestSemElemImpl)
{
    RandomMatrix1_3x3 f;
    sem_elem_t m = new MinPlusIntMatrix(f.mat);
    test_semelem_impl(m);
}

}
}
