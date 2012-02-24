#include "gtest/gtest.h"
#include "glog/logging.h"
 
int main(int argc, char **argv){
  google::InitGoogleLogging(argv[0]);
  google::SetLogDestination(google::INFO, "logs.INFO");
  google::SetLogDestination(google::WARNING, "logs.WARNING");
  google::SetLogDestination(google::ERROR, "logs.ERROR");
  google::SetLogDestination(google::FATAL, "logs.FATAL");

  ::testing::InitGoogleTest(&argc, argv);

  unsigned ret = RUN_ALL_TESTS();

  google::ShutdownGoogleLogging();

  return ret;
}
