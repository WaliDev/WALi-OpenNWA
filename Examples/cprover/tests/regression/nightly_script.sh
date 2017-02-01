#!/bin/bash

# Jason found that this line was required to get nightly_script.sh to run from 
# (I just stole it from ~/.profile)
source /bat0/stac/.opam/opam-init/init.sh > /dev/null 2> /dev/null || true

shopt -s nullglob

NROOT=/bat0/stac/Code/CSFE_JAVA_API/jbreck
#NEWTON=~/Newton/WALi-OpenNWA
NEWTON=$NROOT/WALi-OpenNWA
LIBTSLRE=$NEWTON/Examples/cprover/libtslre
DUET=$NROOT/duet
SUITE=$NEWTON/Examples/cprover/tests/regression
#SERVER=/media/sf_stac/public/html
SERVER=/media/sf_html
TODAY=$(date +%Y%m%d)
#TODAY=NT2_$(date +%Y%m%d)

# Updating the repos
# cd $DUET
# git pull origin Newton || exit 1
# make || exit 1
# ./make_libduet.sh || exit 1
# cd $LIBTSLRE
# git pull || exit 1
# cd $NEWTON
# git pull origin NewtonDuet || exit 1
# ./duet_build.sh || exit 1

cp -f $SUITE/outputs/__result.out $SUITE/__result.out.old
cp -f $SUITE/outputs/__result_java.out $SUITE/__result_java.out.old
rm -rf $SUITE/inputs/
rm -rf $SUITE/outputs/

cd $NEWTON

echo "JAVA BENCHMARKS:"
$SUITE/test_suite_java.sh || exit 1
echo "C ASSERTION CHECKING:"
$SUITE/test_suite_duet.sh || exit 1
echo "C BOUNDS GENERATION:"
$SUITE/test_suite_printHull.sh || exit 1
cd $SUITE
python $SUITE/compare.py || exit 1
python $SUITE/updateIndex.py || exit 1

echo "COPYING FILES:"
mkdir -p $SERVER/$TODAY
cp -f $SUITE/result.html $SERVER/$TODAY
cp -f $SUITE/result_java.html $SERVER/$TODAY
cp -f $SUITE/compare.html $SERVER/$TODAY
cp -f $SUITE/print_hull.html $SERVER/$TODAY
cp -Rf $SUITE/inputs/ $SERVER/$TODAY/
cp -Rf $SUITE/outputs/ $SERVER/$TODAY/
cp -f $SUITE/index.html $SERVER

echo "DONE WITH nightly_script.sh"
printf "Hello,\n\nThis is to inform you of the results of ICRA's nightly tests.  Running the tests finished on $(date).\n\nThe ICRA/bytecode (Java) results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_java.html\n\nThe ICRA/C assertion-checking results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe ICRA/C comparison results can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe ICRA/C bounds-generation results can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "ICRA Test Results"  jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com turetsky@grammatech.com jcyphert@cs.wisc.edu
#printf "TEST VERSION OF EMAIL (please reset to regular mail list before midnight)\n\nHello,\n\nThis is to inform you of the result of the Newton test suite. Running the tests finished on $(date).\n\nThe ICRA/bytecode (Java) results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result_java.html\n\nThe ICRA/C results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe ICRA/C comparison can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe ICRA/C bounds generation can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "Newton Test Result" jbreck@cs.wisc.edu
#OLD VERSION
#printf "Hello,\n\nThis is to inform you of the result of the Newton test suite. Running the tests finished on $(date).\n\nThe results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe comparison can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe bounds generation can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "Newton Test Result" ashkanfb@cs.wisc.edu jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com
