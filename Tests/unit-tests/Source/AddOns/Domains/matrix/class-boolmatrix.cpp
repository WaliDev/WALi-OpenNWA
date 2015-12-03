#include "gtest/gtest.h"

#include <sstream>
#include <boost/scoped_ptr.hpp>

#include "wali/domains/matrix/Matrix.hpp"

#include "fixtures-boolmatrix.hpp"
#include "matrix-equal.hpp"

using namespace testing::boolmatrix;

namespace wali {
namespace domains {

TEST(wali$domains$matrix$BoolMatrix$$constructorAndMatrix, basicTest3x3)
{
    RandomMatrix1_3x3 f;
    BoolMatrix m(f.mat);

    EXPECT_EQ(m.matrix(), f.mat);
}


#define NUM_ELEMENTS(arr) ((sizeof arr)/(sizeof arr[0]))

TEST(wali$domains$matrix$BoolMatrix$$equalAndIsZeroAndIsOne, battery)
{
    MatrixFixtures_3x3 f;
    BoolMatrix mats[] = {
        BoolMatrix(f.zero.mat),
        BoolMatrix(f.id.mat),
        BoolMatrix(f.r1.mat),
        BoolMatrix(f.r2.mat),
        BoolMatrix(f.ext_r1_r2.mat),
        BoolMatrix(f.ext_r2_r1.mat),
    };

    for (size_t left=0; left<NUM_ELEMENTS(mats); ++left) {
        for (size_t right=0; right<NUM_ELEMENTS(mats); ++right) {
            EXPECT_EQ(left == right,
                      mats[left].equal(&mats[right]));
        }
    }
}


TEST(wali$domains$matrix$BoolMatrix$$zero_raw, basicTest3x3)
{
    RandomMatrix1_3x3 f;
    ZeroBackingMatrix_3x3 z;

    BoolMatrix m(f.mat);
    BoolMatrix mz(z.mat);

    boost::scoped_ptr<BoolMatrix> result(m.zero_raw());

    EXPECT_EQ(z.mat, result->matrix());
    EXPECT_TRUE(mz.equal(result.get()));
}


TEST(wali$domains$matrix$BoolMatrix$$one_raw, basicTest3x3)
{
    RandomMatrix1_3x3 f;
    IdBackingMatrix_3x3 z;

    BoolMatrix m(f.mat);
    BoolMatrix mid(z.mat);

    boost::scoped_ptr<BoolMatrix> result(m.one_raw());

    EXPECT_EQ(z.mat, result->matrix());
    EXPECT_TRUE(mid.equal(result.get()));
}


TEST(wali$domains$matrix$BoolMatrix$$extend_raw, twoRandomMatrices)
{
    RandomMatrix1_3x3 f1;
    RandomMatrix2_3x3 f2;
    ExtendR1R2_3x3 fr12;
    ExtendR2R1_3x3 fr21;

    BoolMatrix m1(f1.mat);
    BoolMatrix m2(f2.mat);

    boost::scoped_ptr<BoolMatrix> result12(m1.extend_raw(&m2));
    boost::scoped_ptr<BoolMatrix> result21(m2.extend_raw(&m1));

    EXPECT_EQ(fr12.mat, result12->matrix());
    EXPECT_EQ(fr21.mat, result21->matrix());
}


TEST(wali$domains$matrix$BoolMatrix$$extend_raw, extendAgainstZero)
{
    RandomMatrix1_3x3 f;
    ZeroBackingMatrix_3x3 z;

    BoolMatrix mf(f.mat);
    BoolMatrix mz(z.mat);

    boost::scoped_ptr<BoolMatrix>
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


TEST(wali$domains$matrix$BoolMatrix$$extend_raw, extendAgainstOne)
{
    RandomMatrix1_3x3 fr1;
    IdBackingMatrix_3x3 id;

    BoolMatrix mr1(fr1.mat);
    BoolMatrix mid(id.mat);

    boost::scoped_ptr<BoolMatrix>
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


TEST(wali$domains$matrix$BoolMatrix$$combine_raw, randomAndId)
{
    RandomMatrix1_3x3 f1;
    IdBackingMatrix_3x3 f2;
    CombineR1Id_3x3 fr;

    BoolMatrix m1(f1.mat);
    BoolMatrix m2(f2.mat);
    BoolMatrix mr(fr.mat);

    boost::scoped_ptr<BoolMatrix> result12(m1.combine_raw(&m2));
    boost::scoped_ptr<BoolMatrix> result21(m2.combine_raw(&m1));

    EXPECT_EQ(fr.mat, result12->matrix());
    EXPECT_EQ(fr.mat, result21->matrix());

    EXPECT_TRUE(mr.equal(result12.get()));
    EXPECT_TRUE(mr.equal(result21.get()));
}


TEST(wali$domains$matrix$BoolMatrix$$print, random)
{
    RandomMatrix1_3x3 f;
    BoolMatrix m(f.mat);
    std::stringstream ss;

    m.print(ss);

    EXPECT_EQ("Matrix: [3,3]((1,1,0),(1,0,1),(0,0,1))", ss.str());
}


TEST(wali$domains$matrix$BoolMatrix, callWaliTestSemElemImpl)
{
    RandomMatrix1_3x3 f;
    sem_elem_t m = new BoolMatrix(f.mat);
    test_semelem_impl(m);
}

}
}
