#!/bin/bash

# Jason found that this line was required to get nightly_script.sh to run from 
# (I just stole it from ~/.profile)
source /bat0/stac/.opam/opam-init/init.sh > /dev/null 2> /dev/null || true

shopt -s nullglob

#NROOT=/bat0/stac/Code/CSFE_JAVA_API/jbreck
#NROOT=/bat0/stac/Code/Ark2-Sandbox
NROOT=/bat0/stac/Code/WALi-Sandbox
#NEWTON=~/Newton/WALi-OpenNWA
NEWTON=$NROOT/WALi-OpenNWA
LIBTSLRE=$NEWTON/Examples/cprover/libtslre
DUET=$NROOT/duet
SUITE=$NEWTON/Examples/cprover/tests/regression
#SERVER=/media/sf_stac/public/html
SERVER=/media/sf_html
TODAY=$(date +%Y%m%d) # like 20171102
DATE_ADBY=$(date +"%a, %d %b %Y") # like Thu, 02 Nov 2017

##### No longer needed; Ark2 is the default
####ARK2NROOT=/bat0/stac/Code/CSFE_JAVA_API/jbreck/Ark2
####ARK2NEWTON=$ARK2NROOT/WALi-OpenNWA
####ARK2SUITE=$ARK2NEWTON/Examples/cprover/tests/regression

# Updating the repos
### Note: this is now done in a separate script (recompile_everything.sh)
# cd $DUET
# git pull origin Newton || exit 1
# make || exit 1
# ./make_libduet.sh || exit 1
# cd $LIBTSLRE
# git pull || exit 1
# cd $NEWTON
# git pull origin NewtonDuet || exit 1
# ./duet_build.sh || exit 1

# It makes more sense to do these *after* the tests
#cp -f $SUITE/outputs/__result.out $SUITE/__result.out.old
#cp -f $SUITE/outputs/__result_java.out $SUITE/__result_java.out.old

echo "Not removing inputs and outputs directories..."
#rm -rf $SUITE/inputs/
#rm -rf $SUITE/outputs/

cd $NEWTON

echo "JAVA BENCHMARKS:"
#$SUITE/test_suite_java.sh || exit 1
$SUITE/test_suite_java.sh
echo "C ASSERTION CHECKING:"
#$SUITE/test_suite_duet.sh || exit 1
$SUITE/test_suite_duet.sh
echo "C BOUNDS GENERATION:"
#$SUITE/test_suite_printHull.sh || exit 1
$SUITE/test_suite_printHull.sh

$SUITE/test_suite_ocrs.sh

cd $SUITE
#python $SUITE/compare.py || exit 1
python $SUITE/compare.py
#python $SUITE/updateIndex.py || exit 1
python $SUITE/updateIndex.py "$TODAY" "$DATE_ADBY"

python $SUITE/compare_java.py
python $SUITE/compare_ocrs.py

mkdir -p $SERVER/$TODAY
mkdir -p $SERVER/$TODAY/outputs

##### No longer needed; Ark2 is the default
####cd $ARK2NEWTON
####echo "ARK2 BENCHMARKS:"
####$ARK2SUITE/test_suite_ark2.sh || exit 1
####cp -f $ARK2SUITE/result_ark2.html $SERVER/$TODAY/
####cp -f $ARK2SUITE/outputs/*.ark2.out $SERVER/$TODAY/outputs/

cd $SUITE
echo "COPYING FILES:"
cp -f $SUITE/result.html $SERVER/$TODAY/
cp -f $SUITE/result_java.html $SERVER/$TODAY/
cp -f $SUITE/compare.html $SERVER/$TODAY/
cp -f $SUITE/compare_java.html $SERVER/$TODAY/
cp -f $SUITE/print_hull.html $SERVER/$TODAY/
cp -Rf $SUITE/inputs/ $SERVER/$TODAY/
cp -Rf $SUITE/outputs/ $SERVER/$TODAY/
cp -f $SUITE/index.html $SERVER/

cp -f $SUITE/result_ocrs.html $SERVER/$TODAY/
cp -f $SUITE/compare_ocrs.html $SERVER/$TODAY/

cp -f $SUITE/__result.out.old $SUITE/__result.out.old2
cp -f $SUITE/__result_java.out.old $SUITE/__result_java.out.old2
cp -f $SUITE/outputs/__result.out $SUITE/__result.out.old
cp -f $SUITE/outputs/__result_java.out $SUITE/__result_java.out.old

echo "COMPRESSING LARGE OUTPUT FILES:"
python compress_large_outputs.py $SERVER/$TODAY/

echo "DONE WITH nightly_script.sh"

# Version distinguishing OCRS:
printf "Hello,\n\nThis is to inform you of the results of ICRA's nightly tests.  Running the tests finished on $(date).\n\nThe ICRA/bytecode (Java) results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_java.html\nThe ICRA/bytecode (Java) comparison results can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare_java.html\n\nThe ICRA/C (Ark2 without OCRS) assertion-checking results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe ICRA/C (Ark2 without OCRS) comparison results can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe ICRA/C (Ark2 without OCRS) bounds-generation results can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n\nThe ICRA/C (Ark2 with OCRS) assertion-checking results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_ocrs.html\nThe comparison between tonight's ICRA/C results with OCRS and without OCRS can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare_ocrs.html\n" | mail -s "ICRA Test Results" jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com turetsky@grammatech.com jcyphert@cs.wisc.edu

#Version before distinguishing OCRS: (come back to this later)
#printf "Hello,\n\nThis is to inform you of the results of ICRA's nightly tests.  Running the tests finished on $(date).\n\nThe ICRA/bytecode (Java) results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_java.html\nThe ICRA/bytecode (Java) comparison results can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare_java.html\n\nThe ICRA/C (Ark2) assertion-checking results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe ICRA/C (Ark2) comparison results can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe ICRA/C (Ark2) bounds-generation results can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "ICRA Test Results" jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com turetsky@grammatech.com jcyphert@cs.wisc.edu

# This version included a separate Ark2:
#printf "Hello,\n\nThis is to inform you of the results of ICRA's nightly tests.  Running the tests finished on $(date).\n\nThe ICRA/bytecode (Java) results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_java.html\n\nThe ICRA/C assertion-checking results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe ICRA/C comparison results can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe ICRA/C bounds-generation results can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n\nNewton-ark2 branch results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_ark2.html\n" | mail -s "ICRA Test Results" jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com turetsky@grammatech.com jcyphert@cs.wisc.edu

#printf "TEST VERSION OF EMAIL (please reset to regular mail list before midnight)\n\nHello,\n\nThis is to inform you of the result of the Newton test suite. Running the tests finished on $(date).\n\nThe ICRA/bytecode (Java) results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_java.html\n\nThe ICRA/C results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe ICRA/C comparison can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe ICRA/C bounds generation can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "Newton Test Result" jbreck@cs.wisc.edu

#OLD VERSION
#printf "Hello,\n\nThis is to inform you of the result of the Newton test suite. Running the tests finished on $(date).\n\nThe results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe comparison can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe bounds generation can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "Newton Test Result" ashkanfb@cs.wisc.edu jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com

