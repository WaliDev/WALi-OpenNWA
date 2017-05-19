#!/bin/bash

cd ../duet
echo "@@@@@@@@@@@@@@@@@@@@@@ Beginning ICRA rebuild"

# The following steps were required before we started using the Newton-ark2 branch of duet
#
#echo "@@@@@@@@@@@@@@@@@@@@@@ Step (1/5): [Main duet files]      make duet"
#make duet || exit 1
#if [ -f build_plugin.sh ]; then
#    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (2/5): [A few more duet files]  make libduet"
#    make libduet || exit 1
#    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (3/5): [CodeSurfer plugin]    ./build_plugin.sh"
#    ./build_plugin.sh || exit 1
#else
#    echo "@@@@@@@@@@ Skipping steps 2 and 3, which are only needed for ICRA/bytecode"
#fi
#echo "@@@@@@@@@@@@@@@@@@@@@@ Step (4/5): [.so file for ICRA]    ./make_libduet.sh"
#./make_libduet.sh || exit 1

# The following step suffices when using the Newton-ark2 branch of duet
echo "@@@@@@@@@@@@@@@@@@@@@@ Step (1/5): [Main duet files]      make newton"
make newton || exit 1
cd ../WALi-OpenNWA
echo "@@@@@@@@@@@@@@@@@@@@@@ Step (5/5): [ICRA C++ code]        ./duet_build.sh"
./duet_build.sh || exit 1
echo "@@@@@@@@@@@@@@@@@@@@@@ ICRA rebuild completed successfully"
