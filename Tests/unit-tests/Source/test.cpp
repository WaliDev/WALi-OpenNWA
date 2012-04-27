#include <iostream>
#include "gtest/gtest.h"

int gtest_num_test_parts = 0;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();

  if (gtest_num_test_parts > 0) {
      std::cout << "There were " << gtest_num_test_parts << " assertions checked!\n";
  }
  
  return ret;
}
