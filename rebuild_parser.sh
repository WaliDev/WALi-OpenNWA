#!/bin/bash
# This file was created by Jason Breck in March 2016 to help rebuild
#   some of the library files used by NewtonDuet
#
# It requires:
#   flex
#   bison
#
# Its inputs are:
#   Examples/cprover/
#                    bp_scanner.l
#                    bp_scanner.y
#                    ast.c
#                    BplToPds.cpp
# Its output is:
#   lib64/libbpparser.so
scons addons
if [ ! -d "_build64/Examples/cprover" ]; then
    mkdir -p _build64/Examples/cprover
fi
bison --defines -o _build64/Examples/cprover/bp_parser.c Examples/cprover/bp_parser.y -d || exit 1

cp Examples/cprover/bp_scanner.l _build64/Examples/cprover/
flex -t _build64/Examples/cprover/bp_scanner.l > _build64/Examples/cprover/bp_scanner.c || exit 1


/usr/bin/gcc -o _build64/Examples/cprover/bp_scanner.os -c -Wall -g -Wextra -fdiagnostics-show-option -fPIC -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -I/home/tsl-sandbox -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include _build64/Examples/cprover/bp_scanner.c || exit 1


/usr/bin/gcc -o _build64/Examples/cprover/bp_parser.os -c -Wall -g -Wextra -fdiagnostics-show-option -fPIC -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -I/home/tsl-sandbox -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include _build64/Examples/cprover/bp_parser.c || exit 1


/usr/bin/gcc -o _build64/Examples/cprover/ast.os -c -Wall -g -Wextra -Wformat=2 -Winit-self -Wunused -Wfloat-equal -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion -fdiagnostics-show-option -fPIC -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -I/home/tsl-sandbox -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include Examples/cprover/ast.c || exit 1

/usr/bin/g++ -o _build64/Examples/cprover/BplToPds.os -c -O0 -Wall -g -Wextra -fdiagnostics-show-option -fPIC -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -I/home/tsl-sandbox -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include Examples/cprover/BplToPds.cpp || exit 1

echo "***************** Link phase: *****************"
/usr/bin/g++ -o _build64/Examples/cprover/libbpparser.so -rdynamic -shared -Wl,-rpath=/unsup/boost-1.55.0/amd64_rhel6/lib -Wl,-rpath=/home/turetsky/OCamlTest -Wl,-rpath=/home/turetsky/WALi-OpenNWA/lib64 _build64/Examples/cprover/bp_parser.os _build64/Examples/cprover/bp_scanner.os _build64/Examples/cprover/BplToPds.os _build64/Examples/cprover/ast.os -L/unsup/boost-1.55.0/amd64_rhel6/lib -L/home/turetsky/OCamlTest -Llib64 -lboost_filesystem -lboost_system -lboost_serialization -lrt -ldl -lm -lwali || exit 1

cp _build64/Examples/cprover/libbpparser.so lib64/libbpparser.so || exit 1

echo "***** Successful end of rebuild_parser.sh *****"
