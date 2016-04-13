#!/bin/bash

# Runs Newton on tests in Examples/cprover/tests/regression/tests/
# Puts the result in Examples/cprover/tests/regression/result.html

shopt -s nullglob

SUITE="$(pwd)/Examples/cprover/tests/regression"
TESTS=( $SUITE/tests/*.c )
OUTDIR="$SUITE/outputs"
RESULT="$OUTDIR/result.out"
NEWTON="$(pwd)/_build64/Examples/cprover/NewtonOcaml"
RUNMODE="-cra_newton_basic"
SCRIPT="$SUITE/toHTML.py"
TIMEOUT="30m"

mkdir -p $OUTDIR
rm -f $RESULT

echo "${#TESTS[@]} tests found."

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

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
cd ../../../../
echo "Done. Please see result.html"
