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
UA_DIR="$NEWTON_DIR/../UAutomizer"
#UA_DIR="/home/turetsky/uAutomizer/uAutomizer/UAutomizer-linux"
CPA_DIR="/home/turetsky/cpaChecker/CPAchecker-1.6.12-svcomp17-unix"
#CPA_DIR="$NEWTON_DIR/../CPAchecker"
#LPI_DIR="$NEWTON_DIR/../lpi-svcomp16"
#SEA_DIR="$NEWTON_DIR/../SeaHorn-0.1.0-Linux-x86_64"
SEA_DIR="/home/turetsky/seahorn/SeaHorn-0.1.0-Linux-x86_64/"

SUITE="$NEWTON_DIR/Examples/cprover/tests/regression"

#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/TEMP_FAST ) # temp fast

#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/frankenstein/functional/exponential ) # frankenstein suite

TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/frankenstein/functional/ $NEWTON_DIR/Examples/cprover/tests/frankenstein/functional/exponential $NEWTON_DIR/Examples/cprover/tests/frankenstein/HOLA/ $NEWTON_DIR/Examples/cprover/tests/frankenstein/relational/ ) # frankenstein suite

#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/sv-benchmarks/* $NEWTON_DIR/Examples/cprover/tests/misc-recursive $NEWTON_DIR/Examples/cprover/tests/rec-loop-lit $NEWTON_DIR/Examples/cprover/tests/rec-loop-new ) # PLDI subset
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/rec-new $NEWTON_DIR/Examples/cprover/tests/rec-loop-lit $NEWTON_DIR/Examples/cprover/tests/rec-loop-new )
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/misc-recursive $NEWTON_DIR/Examples/cprover/tests/sv-benchmarks/* ) # OLD PLDI (POPL) subset
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/misc-recursive ) # quick test
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/duet $NEWTON_DIR/Examples/cprover/tests $NEWTON_DIR/Examples/cprover/tests/STAC/LESE $NEWTON_DIR/Examples/cprover/tests/STAC/LowerBound $NEWTON_DIR/Examples/cprover/tests/STAC/LZ $NEWTON_DIR/Examples/cprover/tests/sv-benchmarks/* ) # all benchmarks

NEWTON="$NEWTON_DIR/_build64/Examples/cprover/NewtonOcaml"
DUET="$DUET_DIR/duet.native"
UA="$UA_DIR/Ultimate.py"
CPA="$CPA_DIR/scripts/cpa.sh"
LPI="$LPI_DIR/scripts/cpa.sh"
#SEA="$SEA_DIR/bin/sea_svcomp"
SEA="~/seahorn/SeaHorn-0.1.0-Linux-x86_64/bin/sea_svcomp"

# The outputs are saved here
OUTDIR="$SUITE/outputs"
# The input tests are copied here
INDIR="$SUITE/inputs"
RESULT="$OUTDIR/__result_C4.out"
SCRIPT="$SUITE/toHTML_compareFiveTools.py"

TIMEOUT="1m"
#TIMEOUT="5m"

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
		ua_outfile="$OUTDIR/$(basename $testf).ua.out"
		cpa_outfile="$OUTDIR/$(basename $testf).cpa.out"
		lpi_outfile="$OUTDIR/$(basename $testf).lpi.out"
		sea_outfile="$OUTDIR/$(basename $testf).sea.out"
		infile="$INDIR/$(basename $testf)"
		rm -f $below_outfile
		rm -f $duet_outfile
		rm -f $ua_outfile
		rm -f $cpa_outfile
		rm -f $lpi_outfile
		rm -f $sea_outfile
		rm -f $infile
		rm -f $infile.i
		cp $testf $infile
		
		echo -n "Running test $i of ${#TESTS[@]} $(basename $testf) ... "
		
		echo -n " Newton ..."
		start=$(date +%s%N)
		cd $NEWTON_DIR
		#eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULT $testf &> $below_outfile"
		eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -use-ocrs -cra-matrix --test=$RESULT $testf &> $below_outfile"
		success=$?
		if (($success==124)); then
			echo "__TIMEOUT" >> $RESULT
			echo -ne "\e[31mTimeout\e[0m"
		elif (($success!=0)); then
			echo "__EXCEPTION" >> $RESULT
			echo -ne "\e[31mException\e[0m"
		else
			echo "" >> $RESULT
		fi
		end=$(date +%s%N)
		len=$(expr $end - $start)
		echo "__NTIME $len" >> $RESULT
		
		####### The following was Ashkan's original code for running Duet:
		##########echo -n " Duet ..."
		##########start=$(date +%s%N)
		##########echo -n "__DUET " >> $RESULT
		##########cd $DUET_DIR
		##########eval "timeout $TIMEOUT  $DUET -cra -cra-forward-inv -cra-split-loops $testf &> $duet_outfile"
		##########if (($?==124)); then
		##########	echo "TIMEOUT" >> $RESULT
		##########	echo -ne "\e[31mTimeout\e[0m"
		##########else
		##########	echo "" >> $RESULT
		##########fi
		##########end=$(date +%s%N)
		##########len=$(expr $end - $start)
		##########echo "__DTIME $len" >> $RESULT

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
		
		# Run Preprocessor for UAutomizer and CPAchecker
		gcc -E $testf -o $infile.i
		
		echo -n " UAutomizer ..."
		start=$(date +%s%N)
		echo -n "__UA " >> $RESULT
		cd $UA_DIR
		eval "timeout $TIMEOUT $UA --full-output $directory/ALL.prp 64bit precise $infile.i &> $ua_outfile"
		if (($?==124)); then
			echo "TIMEOUT" >> $RESULT
			echo -ne "\e[31mTimeout\e[0m"
		else
			if grep -Pzoq "Result:\nTRUE" $ua_outfile; then
				echo "PASS" >> $RESULT
			elif grep -Pzoq "Result:\nFALSE" $ua_outfile; then
				echo "FAIL" >> $RESULT
			elif grep -Pzoq "Result:\nUNKNOWN" $ua_outfile; then
				echo "UNKNOWN" >> $RESULT
			else
				echo "EXCEPTION" >> $RESULT
				echo -ne "\e[31mException\e[0m"
			fi
		fi
		end=$(date +%s%N)
		len=$(expr $end - $start)
		echo "__UATIME $len" >> $RESULT
		
		echo -n " CPAchecker ..."
		start=$(date +%s%N)
		echo -n "__CPA " >> $RESULT
		cd $CPA_DIR
		#cd $UA_DIR
		eval "timeout $TIMEOUT $CPA -config $CPA_DIR/config/sv-comp17.properties -noout -timelimit 900 $infile.i &> $cpa_outfile"
		if (($?==124)); then
			echo "TIMEOUT" >> $RESULT
			echo -ne "\e[31mTimeout\e[0m"
		else
			if grep -Pzoq "result: TRUE" $cpa_outfile; then
				echo "PASS" >> $RESULT
			elif grep -Pzoq "result: FALSE" $cpa_outfile; then
				echo "FAIL" >> $RESULT
			elif grep -Pzoq "result: UNKNOWN" $cpa_outfile; then
				echo "UNKNOWN" >> $RESULT
			else
				echo "EXCEPTION" >> $RESULT
				echo -ne "\e[31mException\e[0m"
			fi
		fi
		end=$(date +%s%N)
		len=$(expr $end - $start)
		echo "__CPATIME $len" >> $RESULT
		
		#echo -n " LPI ..."
		#start=$(date +%s%N)
		#echo -n "__LPI " >> $RESULT
		#cd $LPI_DIR
		#eval "timeout $TIMEOUT $LPI -config $LPI_DIR/config/lpi-svcomp16.properties -noout $infile.i &> $lpi_outfile"
		##eval "timeout $TIMEOUT $CPA -config $CPA_DIR/config/sv-comp16.properties -noout $infile.i &> $cpa_outfile"
		#if (($?==124)); then
		#	echo "TIMEOUT" >> $RESULT
		#	echo -ne "\e[31mTimeout\e[0m"
		#else
		#	if grep -Pzoq "result: TRUE" $lpi_outfile; then
		#		echo "PASS" >> $RESULT
		#	elif grep -Pzoq "result: FALSE" $lpi_outfile; then
		#		echo "FAIL" >> $RESULT
		#	elif grep -Pzoq "result: UNKNOWN" $lpi_outfile; then
		#		echo "UNKNOWN" >> $RESULT
		#	else
		#		echo "EXCEPTION" >> $RESULT
		#		echo -ne "\e[31mException\e[0m"
		#	fi
		#fi
		#end=$(date +%s%N)
		#len=$(expr $end - $start)
		#echo "__LPITIME $len" >> $RESULT
		
		echo -n " SEA ..."
		start=$(date +%s%N)
		echo -n "__SEA " >> $RESULT
		cd $SEA_DIR
		eval "timeout $TIMEOUT $SEA $infile.i &> $sea_outfile"
		if (($?==124)); then
			echo "TIMEOUT" >> $RESULT
			echo -ne "\e[31mTimeout\e[0m"
		else
			if grep -Pzoq "Result TRUE" $sea_outfile; then
				echo "PASS" >> $RESULT
			elif grep -Pzoq "Result FALSE" $sea_outfile; then
				echo "FAIL" >> $RESULT
			elif grep -Pzoq "Result UNKNOWN" $sea_outfile; then
				echo "UNKNOWN" >> $RESULT
			else
				echo "EXCEPTION" >> $RESULT
				echo -ne "\e[31mException\e[0m"
			fi
		fi
		end=$(date +%s%N)
		len=$(expr $end - $start)
		echo "__SEATIME $len" >> $RESULT
		
		echo -n " Done"
                echo -n " (Cleaning up SeaHorn...)"
                killall seahorn &> /dev/null
                echo ""
			
		let i+=1
	done
done

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
echo "Done. Please see result_C5.html"
