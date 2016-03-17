#!/bin/bash
BOOST_PATH=/unsup/boost-1.55.0/amd64_rhel6
TSLRE_LIB_PATH=Examples/cprover/libtslre/build
TSLRE_INCLUDE_PATH=Examples/cprover/libtslre/redist-tsl-sandbox
if [ "x$WALI_ROOT" == "x" ]; then
    #WALI_ROOT=/home/turetsky/WALi-OpenNWA
    WALI_ROOT=.
fi
if [ "x$NEWTON_DUET_ROOT" == "x" ]; then
    NEWTON_DUET_ROOT=../duet
    if [ ! -d "$NEWTON_DUET_ROOT/duet/src" ]; then
        NEWTON_DUET_ROOT=./duet
        if [ ! -d "$NEWTON_DUET_ROOT/duet/src" ]; then
            echo "Please set NEWTON_DUET_ROOT to the base directory of Duet."
            echo "  (We must link against NEWTON_DUET_ROOT/_build/duet/src/libduet.so)"
            exit 1
        fi
    fi
fi
if [ ! -d "_build64/Examples/cprover" ]; then
    mkdir -p _build64/Examples/cprover
fi

scons addons || exit 1

g++ -o newton_interface.os -c -Wall -g -O0 -Wextra -fdiagnostics-show-option -fPIC -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=1 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D__TSL_REGEXP=1 -ISource -I$BOOST_PATH/include -IAddOns/Domains/ThirdParty/include -IAddOns/Domains/Source -IThirdParty/include -I"`ocamlc -where`" Examples/cprover/NewtonOcamlInterface.cpp || exit 1

g++ -o libocamlinterface.so -rdynamic -shared -Wl,-rpath=$BOOST_PATH/lib -Wl,-rpath=/home/turetsky/OCamlTest newton_interface.os -L$BOOST_PATH/lib -L/home/turetsky/OCamlTest -lboost_filesystem -lboost_system -lboost_serialization -lrt || exit 1

g++ -o _build64/Examples/cprover/NewtonCompare.o -c -O0 -Wall -g -Wextra -Wformat=2 -Winit-self -Wfloat-equal -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion -Woverloaded-virtual -fdiagnostics-show-option -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -DUSE_DUET=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -I$TSLRE_INCLUDE_PATH -ISource -I$BOOST_PATH/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include -I"`ocamlc -where`" Examples/cprover/NewtonCompare.cpp || exit 1

#/usr/bin/g++ -o _build64/Examples/cprover/CallOcaml.o -c -O0 -Wall -g -Wextra -Wformat=2 -Winit-self -Wunused -Wfloat-equal -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion -Woverloaded-virtual -fdiagnostics-show-option -DBOOST_NO_DEFAULTED_FUNCTIONS=1 -DCHECKED_LEVEL=2 -DEXPORT_GTR_SYMBOLS=0 -DPRATHMEHS_NWA_DETENSOR=0 -DREGEXP_TEST=1 -DTSL_DETENSOR=1 -D_GLIBCXX_DEBUG=1 -D__TSL_REGEXP=1 -ISource -I$BOOST_PATH/include -IExamples/cprover -IAddOns/Domains/Source -IAddOns/Domains/ThirdParty/include -I"`ocamlc -where`" Examples/cprover/CallOcaml.cpp

echo "***************** Link phase: *****************"
g++ -g -o _build64/Examples/cprover/NewtonOcaml -Wl,-rpath=$BOOST_PATH/lib -Wl,-rpath=$NEWTON_DUET_ROOT/_build/duet/src -Wl,-rpath=$WALI_ROOT -Wl,-rpath=$WALI_ROOT/lib64 -Wl,--start-group _build64/Examples/cprover/NewtonCompare.o -L$BOOST_PATH/lib -Llib64 -L"`ocamlc -where`" -L$NEWTON_DUET_ROOT/_build/duet/src -L$WALI_ROOT -L$TSLRE_LIB_PATH -ltslre -lboost_filesystem -lboost_system -lboost_serialization -lrt -lduet -locamlinterface -lwali -lbpparser -lwalidomains -lglog -lbdd -Wl,--end-group || exit 1
echo "******* Successful end of duet_build.sh *******"
