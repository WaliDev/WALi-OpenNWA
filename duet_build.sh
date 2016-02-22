#!/bin/bash
scons addons

TSLRE_LIB_PATH=Examples/cprover/libtslre/build
TSLRE_INCLUDE_PATH=Examples/cprover/libtslre/redist-tsl-sandbox

/usr/bin/g++ -o newton_interface.os -c -Wall -g -O0 -Wextra -Werror -fdiagnostics-show-option -fPIC -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=1 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D__TSL_REGEXP=1 -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IAddOns/Domains/ThirdParty/include -IAddOns/Domains/Source -IThirdParty/include -I"`ocamlc -where`" Examples/cprover/NewtonOcamlInterface.cpp

/usr/bin/g++ -o libocamlinterface.so -rdynamic -shared -Wl,-rpath=/unsup/boost-1.55.0/amd64_rhel6/lib -Wl,-rpath=/home/turetsky/OCamlTest newton_interface.os -L/unsup/boost-1.55.0/amd64_rhel6/lib -L/home/turetsky/OCamlTest -lboost_filesystem -lboost_system -lboost_serialization -lrt

/usr/bin/g++ -o _build64/Examples/cprover/NewtonCompare.o -c -O0 -Wall -g -Wextra -Wformat=2 -Winit-self -Wfloat-equal -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion -Woverloaded-virtual -fdiagnostics-show-option -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -DUSE_DUET=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -I$TSLRE_INCLUDE_PATH -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include -I"`ocamlc -where`" Examples/cprover/NewtonCompare.cpp

#/usr/bin/g++ -o _build64/Examples/cprover/CallOcaml.o -c -O0 -Wall -g -Wextra -Wformat=2 -Winit-self -Wunused -Wfloat-equal -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion -Woverloaded-virtual -fdiagnostics-show-option -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -ISource -I/unsup/boost-1.55.0/amd64_rhel6/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include -I"`ocamlc -where`" Examples/cprover/CallOcaml.cpp

/usr/bin/g++ -g -o _build64/Examples/cprover/NewtonOcaml -Wl,-rpath=/unsup/boost-1.55.0/amd64_rhel6/lib -Wl,-rpath=/home/turetsky/duet/_build/duet/src -Wl,-rpath=/home/turetsky/WALi-OpenNWA -Wl,-rpath=/home/turetsky/WALi-OpenNWA/lib64 -Wl,--start-group _build64/Examples/cprover/NewtonCompare.o -L/unsup/boost-1.55.0/amd64_rhel6/lib -Llib64 -L"`ocamlc -where`" -L/home/turetsky/duet/_build/duet/src -L/home/turetsky/WALi-OpenNWA -L$TSLRE_LIB_PATH -ltslre -lboost_filesystem -lboost_system -lboost_serialization -lrt -lduet -locamlinterface -lwali -lbpparser -lwalidomains -lglog -lbdd -Wl,--end-group



