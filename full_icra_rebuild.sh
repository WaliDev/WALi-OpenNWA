#!/bin/bash

cd ../duet
echo "@@@@@@@@@@@@@@@@@@@@@@ Beginning ICRA rebuild"

if [ -f build_plugin.sh ]; then

    # In case of NewtonICRA duet
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (1/5): [Main duet files]      make duet"
    make duet || exit 1
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (2/5): [A few more duet files]  make libduet"
    make libduet || exit 1
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (3/5): [CodeSurfer plugin]    ./build_plugin.sh"
    ./build_plugin.sh || exit 1
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (4/5): [.so file for ICRA]    ./make_libduet.sh"
    ./make_libduet.sh || exit 1
    
    cd ../WALi-OpenNWA
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (5/5): [ICRA C++ code]        ./duet_build.sh"
    ./duet_build.sh || exit 1
    echo "@@@@@@@@@@@@@@@@@@@@@@ ICRA rebuild completed successfully"

else

    # In case of Newton-Ark2 duet
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (1/2): [Main duet files]      make newton"
    make newton || exit 1

    cd ../WALi-OpenNWA
    echo "@@@@@@@@@@@@@@@@@@@@@@ Step (2/2): [ICRA C++ code]        ./duet_build.sh"
    ./duet_build.sh || exit 1
    echo "@@@@@@@@@@@@@@@@@@@@@@ ICRA rebuild completed successfully"
fi
