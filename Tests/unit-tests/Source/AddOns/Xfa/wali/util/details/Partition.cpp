#include <glog/logging.h>
#include "gtest/gtest.h"
#include "Partition.hpp"

namespace Duvidha
{


  TEST(Partition, Basic0) {

    Partition p;

    EXPECT_FALSE(p.AreEquivalent(1,2));
  }
  
  TEST(Partition, Basic1) {

    Partition p;

    p.SetEquivalent(1,2);
    EXPECT_TRUE(p.AreEquivalent(1,2));
  }

  TEST(Partition, BasicNeg0) {

    Partition p;

    EXPECT_FALSE(p.AreEquivalent(-1,-2));
  }
  
  TEST(Partition, BasicNeg1) {

    Partition p;

    p.SetEquivalent(-1,-2);
    EXPECT_TRUE(p.AreEquivalent(-1,-2));
  }


  TEST(Partition, Basic2) {

    Partition p;

    p.SetEquivalent(1,2);
    EXPECT_FALSE(p.AreEquivalent(1,3));

  }

  TEST(Partition, Basic3) {

    Partition p;

    p.SetEquivalent(1,2);
    p.SetEquivalent(3,2);

    EXPECT_TRUE(p.AreEquivalent(1,3));
  }

  TEST(Partition, Basic4) {

    Partition p;

    p.SetEquivalent(1,1);
    EXPECT_TRUE(p.AreEquivalent(1,1));
  }


  
  TEST(Partition, CopyConstructor) {

    Partition q;
    {
    Partition p;

    p.SetEquivalent(1,2);

    q = p;
    }
    // p is out of scope here
    EXPECT_TRUE(q.AreEquivalent(1,2));    
    EXPECT_FALSE(q.AreEquivalent(1,3));    
  }

  TEST(Partition, IsUnconstrained1) 
  {
    Partition p;
    
    EXPECT_TRUE(p.IsUnconstrained());
  }

  TEST(Partition, IsUnconstrained2) 
  {
    Partition p;
    p.SetEquivalent(1,1);
    
    EXPECT_TRUE(p.IsUnconstrained());
  }


  TEST(Partition, FindSet1)
  {
    Partition p;
    EXPECT_EQ(p.FindSet(1),1);
  }

  TEST(Partition, FindSet2)
  {
    Partition p;
    p.SetEquivalent(1,4);
    EXPECT_EQ(p.FindSet(1),p.FindSet(4));
  }

  TEST(Partition, FindSet3)
  {
    Partition p;
    p.SetEquivalent(1,4);
    EXPECT_TRUE(p.FindSet(1)==1 || p.FindSet(1)==4);
  }


  TEST(Partition, CountSets1)
  {
    Partition p;
    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(0));
  }

  TEST(Partition, CountSets2)
  {
    Partition p;
    p.SetEquivalent(1,2);
    p.SetEquivalent(3,4);
    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(2));
  }

  TEST(Partition, Clear1)
  {
    Partition p;
    p.Clear();

    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(0));
  }


  TEST(Partition, Clear2)
  {
    Partition p;
    p.SetEquivalent(1,2);
    p.SetEquivalent(3,4);

    p.Clear();

    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(0));
    EXPECT_TRUE(p.IsUnconstrained());
  }

  TEST(Partition, Intersection1)
  {
    Partition p, res;

    intersect_partitions(p, p, res);

    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(0));
    EXPECT_TRUE(p.IsUnconstrained());
  }

  TEST(Partition, Intersection2)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);
    q = p;
    
    intersect_partitions(p, q, res);
    EXPECT_TRUE(res.AreEquivalent(1,2));
  }

  TEST(Partition, Intersection3)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);
    
    intersect_partitions(p, q, res);

    EXPECT_FALSE(res.AreEquivalent(1,2));
    EXPECT_TRUE(res.IsUnconstrained());
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(0)) << "Intersection " << res.ToString();
  }


  TEST(Partition, Intersection4)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);
    p.SetEquivalent(2,3);

    q.SetEquivalent(1,3);
    
    intersect_partitions(p, q, res);

    EXPECT_TRUE(res.AreEquivalent(1,3));
    EXPECT_FALSE(res.AreEquivalent(1,2));
    EXPECT_FALSE(res.AreEquivalent(2,3));
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(1));
  }


  TEST(Partition, Intersection5)
  {
    Partition p, q, res;

    int const n=10;
    for (int i=0;i<n; i++) {
      p.SetEquivalent(i,i+3);
    }
    
    for (int i=0;i<n; i++) {
      q.SetEquivalent(i,i+2);
    }
    
    intersect_partitions(p, q, res);
    int const halfish = (n+2)/2;
    for (int i=0; i<halfish; i++) {
      EXPECT_TRUE(res.AreEquivalent(i,i+6));
    }
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(6));
  }


  TEST(Partition, LazyInitialization1)
  {
    Partition p;

    p.AreEquivalent(1,2);
    // The above query should not increase the number of sets.
    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(0)) << p.ToString();
  }

  TEST(Partition, Union1)
  {
    Partition p, res;

    union_partitions(p, p, res);

    EXPECT_EQ(p.CountSets(), static_cast<std::size_t>(0));
    EXPECT_TRUE(p.IsUnconstrained());
  }

  TEST(Partition, Union2)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);
    q = p;
    
    union_partitions(p, q, res);
    EXPECT_TRUE(res.AreEquivalent(1,2));
  }

  TEST(Partition, Union3)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);
    
    union_partitions(p, q, res);

    EXPECT_TRUE(res.AreEquivalent(1,2));
    EXPECT_FALSE(res.IsUnconstrained());
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(1)) << "Union " << res.ToString();
  }


  TEST(Partition, Union4)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);
    p.SetEquivalent(2,3);

    q.SetEquivalent(1,3);
    
    union_partitions(p, q, res);

    EXPECT_TRUE(res.AreEquivalent(1,3));
    EXPECT_TRUE(res.AreEquivalent(1,2));
    EXPECT_TRUE(res.AreEquivalent(2,3));
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(1));
  }


  TEST(Partition, Union5)
  {
    Partition p, q, res;

    int const n=10;
    for (int i=0;i<n; i++) {
      p.SetEquivalent(i,i+3);
    }
    
    for (int i=0;i<n; i++) {
      q.SetEquivalent(i,i+2);
    }
    
    union_partitions(p, q, res);


    for (int i=0; i<n+2; i++) {
      EXPECT_TRUE(res.AreEquivalent(i,i+1));
    }
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(1));
  }



  TEST(Partition, Union6)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);

    q.SetEquivalent(1,3);
    
    union_partitions(p, q, res);

    EXPECT_TRUE(res.AreEquivalent(1,3));
    EXPECT_TRUE(res.AreEquivalent(1,2));
    EXPECT_TRUE(res.AreEquivalent(2,3));
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(1));
  }


  TEST(Partition, Union7)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 1);

    q.SetEquivalent(2,2);
    
    union_partitions(p, q, res);

    EXPECT_TRUE(res.AreEquivalent(1,1));
    EXPECT_TRUE(res.AreEquivalent(2,2));
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(0));
  }


  TEST(Partition, Union8)
  {
    Partition p, q, res;

    p.SetEquivalent(1, 2);

    q.SetEquivalent(4,5);
    
    union_partitions(p, q, res);

    EXPECT_TRUE(res.AreEquivalent(1,2));
    EXPECT_TRUE(res.AreEquivalent(4,5));
    EXPECT_FALSE(res.AreEquivalent(1,5));
    EXPECT_FALSE(res.AreEquivalent(4,1));
    EXPECT_EQ(res.CountSets(), static_cast<std::size_t>(2));
  }

  TEST(Partition, Equal1) 
  {
    Partition p;
    EXPECT_EQ(p,p);
  }

  TEST(Partition, Equal2)
  {
    Partition p, q;

    p.SetEquivalent(1,2);

    q.SetEquivalent(1,3);
    q.SetEquivalent(1,2);

    EXPECT_TRUE(p!=q);
  }

  TEST(Partition, Equal3)
  {
    Partition p, q;

    p.SetEquivalent(1,2);

    q.SetEquivalent(3,4);

    EXPECT_TRUE(p!=q);
  }

  TEST(Partition, Equal4)
  {
    Partition p, q;

    p.SetEquivalent(1,2);
    
    q.SetEquivalent(2,1);

    EXPECT_EQ(p,q);
  }

  TEST(Partition, Equal5)
  {
    Partition p, q;

    p.SetEquivalent(1,2);
    p.SetEquivalent(1,3);
    
    q.SetEquivalent(2,3);
    q.SetEquivalent(2,1);

    EXPECT_EQ(p,q);
  }

  TEST(Partition, Equal6)
  {
    Partition p, q;

    p.SetEquivalent(1,2);
    p.SetEquivalent(1,3);
    p.SetEquivalent(-4,-5);
    p.SetEquivalent(10,11);
    
    q.SetEquivalent(2,3);
    q.SetEquivalent(2,1);
    q.SetEquivalent(-5,-4);
    q.SetEquivalent(11,10);

    EXPECT_EQ(p,q);

  }

  TEST(Partition, Equal7)
  {
    Partition p, q;

    int const n = 10;
    for (int i=0; i<n; i+=3) {
      p.SetEquivalent(i,i+1);
      p.SetEquivalent(i,i+2);
      p.SetEquivalent(-i-1, -i-2);
      p.SetEquivalent(i+n+3, i+n+4);

      q.SetEquivalent(i+2,i+1);
      q.SetEquivalent(i,i+1);
      q.SetEquivalent(-i-1, -i-2);
      q.SetEquivalent(i+n+3, i+n+4);
    }

    EXPECT_EQ(p,q) << "p " << p.ToString();
  }

  TEST(Partition, GetRelatedSet1)
  {
    Partition p;
    std::list<int> set;

    set = p.GetRelatedSet(1);
    std::list<int>::iterator result = find(set.begin(), set.end(), 1);
    EXPECT_EQ(*result, 1);
    EXPECT_EQ(set.size(), 1);
  }


  TEST(Partition, GetRelatedSet2)
  {
    Partition p;

    p.SetEquivalent(1,2);
    p.SetEquivalent(2,3);

    for (int j=1; j<4; j++) {
      std::list<int> set = p.GetRelatedSet(j);
      std::list<int>::iterator result;
      for (int i=1; i<4; i++) {
	result = find(set.begin(), set.end(), i);
	EXPECT_EQ(*result, i);
      }
      EXPECT_EQ(set.size(), 3);
    }
  }



  TEST(Partition, GetRelatedSet3)
  {
    Partition p;

    p.SetEquivalent(1,2);
    p.SetEquivalent(2,3);

    p.SetEquivalent(5,6);
    p.SetEquivalent(7,6);
    p.SetEquivalent(5,8);

    for (int j=1; j<4; j++) {
      std::list<int> set = p.GetRelatedSet(j);
      std::list<int>::iterator result;
      for (int i=1; i<4; i++) {
	result = find(set.begin(), set.end(), i);
	EXPECT_EQ(*result, i);
      }
      EXPECT_EQ(set.size(), 3);
    }

    for (int j=5; j<9; j++) {
      std::list<int> set = p.GetRelatedSet(j);
      std::list<int>::iterator result;
      for (int i=5; i<9; i++) {
	result = find(set.begin(), set.end(), i);
	EXPECT_EQ(*result, i);
      }
      EXPECT_EQ(set.size(), 4);
    }


  }


}//end namespace
