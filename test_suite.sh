#!/bin/bash

# Runs Newton on tests in directories listed in $TESTDIRS
# Caution: the name of the test files should be different, even when
#          they're in different directories; otherwise, one's output
#          will be overwritten by the other.
# Puts the result in Examples/cprover/tests/regression/result.html

shopt -s nullglob

SUITE="$(pwd)/Examples/cprover/tests/regression"
TESTDIRS=( $SUITE/tests $SUITE/tests2 )
OUTDIR="$SUITE/outputs"
RESULT="$OUTDIR/result.out"
NEWTON="$(pwd)/_build64/Examples/cprover/NewtonOcaml"
RUNMODE="-cra_newton_basic"
SCRIPT="$SUITE/toHTML.py"
TIMEOUT="30m"

mkdir -p $OUTDIR
rm -f $RESULT

echo "${#TESTDIRS[@]} directories detected."

for directory in ${TESTDIRS[@]}; do

	TESTS=( $directory/*.c )
	echo "${#TESTS[@]} tests found in $directory"
	echo "__DIRECTORY $directory" >> $RESULT

	i=1
	for testf in ${TESTS[@]}; do
		outfile="$OUTDIR/$(basename $testf).out"
		echo -n "Running test $i of ${#TESTS[@]}..."
		timeout $TIMEOUT $NEWTON $RUNMODE --test=$RESULT $testf 2>&1 >>$outfile
		if (($?==124)); then
			echo "__TIMEOUT" >> $RESULT
		fi
		echo done
		let i+=1
	done
done

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
cd ../../../../
echo "Done. Please see result.html"
