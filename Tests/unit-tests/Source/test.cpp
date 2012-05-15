#include <iostream>
#include "gtest/gtest.h"
#include "glog/logging.h"

int gtest_num_test_parts = 0;

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  google::SetLogDestination(google::INFO, "logs.INFO");
  google::SetLogDestination(google::WARNING, "logs.WARNING");
  google::SetLogDestination(google::ERROR, "logs.ERROR");
  google::SetLogDestination(google::FATAL, "logs.FATAL");
  ::testing::InitGoogleTest(&argc, argv);

  int ret = RUN_ALL_TESTS();

  if (gtest_num_test_parts > 0) {
      std::cout << "There were " << gtest_num_test_parts << " assertions checked!\n";
  }

  google::ShutdownGoogleLogging();
  return ret;
}
