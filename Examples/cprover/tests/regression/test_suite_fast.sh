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

TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/sv-benchmarks/* $NEWTON_DIR/Examples/cprover/tests/misc-recursive $NEWTON_DIR/Examples/cprover/tests/rec-loop-lit $NEWTON_DIR/Examples/cprover/tests/rec-loop-new ) # PLDI subset

NEWTON="$NEWTON_DIR/_build64/Examples/cprover/NewtonOcaml"
DUET="$DUET_DIR/duet.native"

# The outputs are saved here
OUTDIR="$SUITE/outputs"
# The input tests are copied here
INDIR="$SUITE/inputs"
RESULT="$OUTDIR/__result_fast.out"
SCRIPT="$SUITE/toHTML_fast.py"

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
		below_outfile="$OUTDIR/$(basename $testf).below.out"
		duet_outfile="$OUTDIR/$(basename $testf).duet.out"
		above_outfile="$OUTDIR/$(basename $testf).above.out"
		infile="$INDIR/$(basename $testf)"
		rm -f $outfile
		rm -f $duet_outfile
		rm -f $infile
		cp $testf $infile
		
		echo -n "Running test $i of ${#TESTS[@]} ($(basename $testf))"
		
		echo -n " ICRA "
		start=$(date +%s%N)
		cd $NEWTON_DIR
		eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULT $testf &> $below_outfile"
		#eval "timeout $TIMEOUT sh -c '{ $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULT $testf; } &> $below_outfile'"
		success=$?
		if (($success==124)); then
			echo "__TIMEOUT" >> $RESULT
			echo -e "\e[31mTimeout\e[0m"
		elif (($success!=0)); then
			echo "__EXCEPTION" >> $RESULT
			echo -e "\e[31mException\e[0m"
		else
            echo "(OK)"
			###echo -n " Duet ..."
			###echo -n "__DUET " >> $RESULT
			###cd $DUET_DIR
			###eval "timeout $TIMEOUT sh -c '{ $DUET -cra -cra-forward-inv -cra-split-loops -verbosity info $testf; } &> $duet_outfile'"
			####eval "timeout $TIMEOUT sh -c '{ $DUET -cra -cra-forward-inv -cra-split-loops $testf; } &> $duet_outfile'"
			###if (($?==124)); then
			###	echo "TIMEOUT" >> $RESULT
			###	echo -ne "\e[31mTimeout\e[0m"
			###else
			###	echo "" >> $RESULT
			###fi
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
		#eval "timeout $TIMEOUT  $DUET -cra -cra-forward-inv -cra-split-loops $testf &> $duet_outfile"
                touch $duet_outfile
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

		#eval "timeout $TIMEOUT sh -c '{ $NEWTON -cra_newton_above -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULT $testf; } &> $above_outfile'"
		#success=$?
		#if (($success==124)); then
		#	echo "__TIMEOUT" >> $RESULT
		#	echo -e "\e[31mTimeout\e[0m"
		#elif (($success!=0)); then
		#	echo "__EXCEPTION" >> $RESULT
		#	echo -e "\e[31mException\e[0m"
		#else
		#	echo " Done"
		#fi
			
		let i+=1
	done
done

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
echo "Done. Please see result_fast.html"
