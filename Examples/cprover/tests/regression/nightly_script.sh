#!/bin/bash

shopt -s nullglob

NEWTON=~/Newton/WALi-OpenNWA
LIBSTL=$NEWTON/Examples/cprover/libtslre
DUET=~/Newton/duet
SUITE=$NEWTON/Examples/cprover/tests/regression
SERVER=/media/sf_stac/public/html
TODAY=$(date +%Y%m%d)

# Updating the repos
# cd $DUET
# git pull origin Newton || exit 1
# make || exit 1
# ./make_libduet.sh || exit 1
# cd $LIBSTL
# git pull || exit 1
# cd $NEWTON
# git pull origin NewtonDuet || exit 1
# ./duet_build.sh || exit 1

cp -f $SUITE/outputs/__result.out $SUITE/__result.out.old
rm -rf $SUITE/inputs/
rm -rf $SUITE/outputs/

cd $NEWTON
echo "ASSERTION CHECKING:"
$SUITE/test_suite_duet.sh || exit 1
echo "BBOUNDS GENERATION:"
$SUITE/test_suite_print_hull.sh || exit 1
cd $SUITE
python $SUITE/compare.py || exit 1
python $SUITE/updateIndex.py || exit 1

mkdir -p $SERVER/$TODAY
cp -f $SUITE/result.html $SERVER/$TODAY
cp -f $SUITE/compare.html $SERVER/$TODAY
cp -f $SUITE/print_hull.html $SERVER/$TODAY
cp -Rf $SUITE/inputs/ $SERVER/$TODAY/
cp -Rf $SUITE/outputs/ $SERVER/$TODAY/
cp -f $SUITE/index.html $SERVER

printf "Hello,\n\nThis is to inform you of the result of the Newton test suite. Running the tests finished on $(date).\n\nThe results can be accessed from: http://stac.cs.wisc.edu/$TODAY/result.html\nThe comparison can be accessed from: http://stac.cs.wisc.edu/$TODAY/compare.html\nThe bounds generation can be accessed from: http://stac.cs.wisc.edu/$TODAY/print_hull.html\n" | mail -s "Newton Test Result" ashkanfb@cs.wisc.edu jbreck@cs.wisc.edu reps@cs.wisc.edu zkincaid@gmail.com
