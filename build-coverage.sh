#!/bin/bash

set -e

export BOOST_HOME=/unsup/boost/amd64_rhel6/include 
export BOOST_LIB=/unsup/boost/amd64_rhel6/lib
 
scons -j12 tests coverage=True checking=fast CXX=/s/gcc-4.6.3/bin/g++ strong_warnings=False

find . -name "*.gcda" -delete
rm -rf coverage

./unit-tests.sh

/unsup/lcov-1.9.1/bin/lcov --gcov-tool=/s/gcc-4.6.3/bin/gcov --directory . -b . --capture --output-file wali.info
/unsup/lcov-1.9.1/bin/lcov --gcov-tool=/s/gcc-4.6.3/bin/gcov --remove wali.info "*/Tests/*" -o wali.info
/unsup/lcov-1.9.1/bin/lcov --gcov-tool=/s/gcc-4.6.3/bin/gcov --remove wali.info "/usr/*" -o wali.info
/unsup/lcov-1.9.1/bin/lcov --gcov-tool=/s/gcc-4.6.3/bin/gcov --remove wali.info "/afs/cs.wisc.edu/s/*" -o wali.info
/unsup/lcov-1.9.1/bin/lcov --gcov-tool=/s/gcc-4.6.3/bin/gcov --remove wali.info "/unsup/*" -o wali.info
/unsup/lcov-1.9.1/bin/lcov --gcov-tool=/s/gcc-4.6.3/bin/gcov --remove wali.info "*ThirdParty/*" -o wali.info
/unsup/lcov-1.9.1/bin/genhtml -o coverage --legend wali.info
