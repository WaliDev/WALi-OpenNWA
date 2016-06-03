#!/bin/bash

# Runs Newton on tests in directories listed in $TESTDIRS
# Caution: the name of the test files should be different, even when
#          they're in different directories; otherwise, one's output
#          will be overwritten by the other.
# Puts the result in Examples/cprover/tests/regression/result.html

shopt -s nullglob

NEWTON_DIR="$(pwd)"
DUET_DIR="$NEWTON_DIR/../duet"

SUITE="$NEWTON_DIR/Examples/cprover/tests/regression"
TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/duet $NEWTON_DIR/Examples/cprover/tests $NEWTON_DIR/Examples/cprover/tests/STAC/LESE $NEWTON_DIR/Examples/cprover/tests/STAC/LowerBound $NEWTON_DIR/Examples/cprover/tests/STAC/LZ )

NEWTON="$NEWTON_DIR/_build64/Examples/cprover/NewtonOcaml"
DUET="$DUET_DIR/duet.native"

# The outputs are saved here
OUTDIR="$SUITE/outputs"
# The input tests are copied here
INDIR="$SUITE/inputs"
RESULT="$OUTDIR/__result.out"
SCRIPT="$SUITE/toHTML.py"

TIMEOUT="5m"

mkdir -p $OUTDIR
mkdir -p $INDIR
rm -f $RESULT

echo "${#TESTDIRS[@]} directories detected."

for directory in ${TESTDIRS[@]}; do

	TESTS=( $directory/*.c )
	echo "${#TESTS[@]} tests found in $directory"
	echo "__DIRECTORY $directory" >> $RESULT

	i=1
	for testf in ${TESTS[@]}; do
		outfile="$OUTDIR/$(basename $testf).out"
		duet_outfile="$OUTDIR/$(basename $testf).duet.out"
		infile="$INDIR/$(basename $testf)"
		rm -f $outfile
		rm -f $duet_outfile
		rm -f $infile
		cp $testf $infile
		
		echo -n "Running test $i of ${#TESTS[@]} ... "
		cd $NEWTON_DIR
		eval "timeout $TIMEOUT sh -c '{ $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULT $testf; } &> $outfile'"
		success=$?
		
		if (($success==124)); then
			echo "__TIMEOUT" >> $RESULT
			echo -e "\e[31mTimeout\e[0m"
		elif (($success!=0)); then
			echo "__EXCEPTION" >> $RESULT
			echo -e "\e[31mException\e[0m"
		else
			echo -n "Duet ... "
			echo -n "__DUET " >> $RESULT
			cd $DUET_DIR
			eval "timeout $TIMEOUT sh -c '{ $DUET -cra -cra-forward-inv -cra-split-loops $testf; } &> $duet_outfile'"
			if (($?==124)); then
				echo "TIMEOUT" >> $RESULT
				echo -e "\e[31mTimeout\e[0m"
			else
				echo "" >> $RESULT
				echo "Done"
			fi
		fi
		let i+=1
	done
done

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
echo "Done. Please see result.html"
