#!/bin/bash

# Runs Newton on tests in directories listed in $TESTDIRS
# Caution: the name of the test files should be different, even when
#          they're in different directories; otherwise, one's output
#          will be overwritten by the other.
# Puts the result in Examples/cprover/tests/regression/result.html

trap "exit" INT

shopt -s nullglob

NEWTON_DIR="$(pwd)"
DUET_DIR="$NEWTON_DIR/../duet"

SUITE="$NEWTON_DIR/Examples/cprover/tests/regression"

TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/misc-recursive $NEWTON_DIR/Examples/cprover/tests/duet $NEWTON_DIR/Examples/cprover/tests $NEWTON_DIR/Examples/cprover/tests/STAC/polynomial/assert $NEWTON_DIR/Examples/cprover/tests/snlee/snlee_tests $NEWTON_DIR/Examples/cprover/tests/STAC/FiniteDifferencing $NEWTON_DIR/Examples/cprover/tests/STAC/LESE $NEWTON_DIR/Examples/cprover/tests/STAC/LowerBound $NEWTON_DIR/Examples/cprover/tests/STAC/LZ $NEWTON_DIR/Examples/cprover/tests/sv-benchmarks/* $NEWTON_DIR/Examples/cprover/tests/rec-sv-benchmarks/rec-loop-lit $NEWTON_DIR/Examples/cprover/tests/rec-sv-benchmarks/rec-loop-new $NEWTON_DIR/Examples/cprover/tests/recurrences $NEWTON_DIR/Examples/cprover/tests/exponential $NEWTON_DIR/Examples/cprover/tests/frankenstein/HOLA $NEWTON_DIR/Examples/cprover/tests/frankenstein/relational $NEWTON_DIR/Examples/cprover/tests/misc2017 $NEWTON_DIR/Examples/cprover/tests/max_equals $NEWTON_DIR/Examples/cprover/tests/branching_loops $NEWTON_DIR/Examples/cprover/tests/strings_numeric )
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/exponential )

NEWTON="$NEWTON_DIR/_build64/Examples/cprover/NewtonOcaml"
DUET="$DUET_DIR/duet.native"

# The outputs are saved here
OUTDIR="$SUITE/outputs"
# The input tests are copied here
INDIR="$SUITE/inputs"
RESULT="$OUTDIR/__result_ocrs.out"
SCRIPT="$SUITE/toHTML_ocrs.py"

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
		below_outfile="$OUTDIR/$(basename $testf).ocrs.out"
		infile="$INDIR/$(basename $testf)"
		rm -f $outfile
		rm -f $infile
		cp $testf $infile
		
		echo -n "Running test $i of ${#TESTS[@]} ($(basename $testf))"
		
		echo -n " ICRA "
		start=$(date +%s%N)
		cd $NEWTON_DIR
        #
        rm -f $below_outfile
        COMMAND="$NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-matrix"
        #COMMAND="$NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -use-ocrs -cra-matrix"
        #COMMAND="$NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -use-ocrs"
        echo $COMMAND >> $below_outfile
        echo "" >> $below_outfile
		eval "timeout $TIMEOUT $COMMAND --test=$RESULT $testf &>> $below_outfile"
        #
		#eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -use-ocrs --test=$RESULT $testf &> $below_outfile"
		success=$?
		if (($success==124)); then
			echo "__TIMEOUT" >> $RESULT
			echo -e "\e[31mTimeout\e[0m"
		elif (($success!=0)); then
			echo "__EXCEPTION" >> $RESULT
			echo -e "\e[31mException\e[0m"
		else
            echo "(OK)"
            echo "" >> $RESULT
		fi
		end=$(date +%s%N)
		len=$(expr $end - $start)
		echo "__NTIME $len" >> $RESULT

		# Here's a modified version of Ashkan's code, which is supposed to avoid ever running Duet: 
		#echo -n " (skipping Duet) ..."
		#start=$(date +%s%N)
		echo -n "__DUET " >> $RESULT
		#cd $DUET_DIR
		#if (($?==124)); then
			echo "TIMEOUT" >> $RESULT
			#echo -ne "\e[31mTimeout\e[0m"
		#else
			#echo "" >> $RESULT
		#fi
		#end=$(date +%s%N)
		#len=$(expr $end - $start)
                len=0
		echo "__DTIME $len" >> $RESULT
		#echo -n " Above ..."
		cd $NEWTON_DIR

		echo "__EXCEPTION" >> $RESULT
		#echo "SKIPPING NEWTON FROM ABOVE"
			
		let i+=1
	done
done

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
echo "Done. Please see result_ocrs.html"
