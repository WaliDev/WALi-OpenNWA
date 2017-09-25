#!/bin/bash

# Runs Newton on tests in directories listed in $TESTDIRS
# Caution: the name of the test files should be different, even when
#          they're in different directories; otherwise, one's output
#          will be overwritten by the other.
# Puts the result in Examples/cprover/tests/regression/result.html

trap "exit" INT

shopt -s nullglob

#NEWTON_DIR="/bat0/stac/Code/CSFE_JAVA_API/jbreck/WALi-OpenNWA/"
NEWTON_DIR="/bat0/stac/Code/Ark2-Sandbox/WALi-OpenNWA/"
#NEWTON_DIR="$(readlink -f .)"
#NEWTON_DIR="$(pwd)"
#DUET_DIR="$NEWTON_DIR/../duet"

SUITE="$NEWTON_DIR/Examples/cprover/tests/regression"
JAVA_PACKAGE="/bat0/stac/Code/Java-benchmarks/java_package"

#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/java/quick )
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/STAC/polynomial/assert $NEWTON_DIR/Examples/cprover/tests/STAC/canonical )
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/c4b $NEWTON_DIR/Examples/cprover/tests/misc-recursive $NEWTON_DIR/Examples/cprover/tests/duet $NEWTON_DIR/Examples/cprover/tests $NEWTON_DIR/Examples/cprover/tests/STAC/LESE $NEWTON_DIR/Examples/cprover/tests/STAC/LowerBound $NEWTON_DIR/Examples/cprover/tests/STAC/LZ $NEWTON_DIR/Examples/cprover/tests/sv-benchmarks/* )
#TESTDIRS=( /bat0/stac/Code/CSFE_JAVA_API/java_package/single_example/ )
#TESTDIRS=( /bat0/stac/Code/CSFE_JAVA_API/java_package/quick_java_examples/ )
#TESTDIRS=( /bat0/stac/Code/CSFE_JAVA_API/java_package/single_example/ /bat0/stac/Code/CSFE_JAVA_API/java_package/quick_java_examples/ /bat0/stac/Code/CSFE_JAVA_API/java_package/simple_java_examples/ /bat0/stac/Code/CSFE_JAVA_API/java_package/selected_canonical/ ) # prior to 2017-03-30
#
#TESTDIRS=( $JAVA_PACKAGE/single_example/ $JAVA_PACKAGE/quick_java_examples/ $JAVA_PACKAGE/simple_java_examples/ $JAVA_PACKAGE/selected_canonical/ $JAVA_PACKAGE/new-canonical-examples/all_canonical_jars/ )
#TESTDIRS=( $NEWTON_DIR/Examples/cprover/tests/java/abstraction_ladder_1/ )

#TESTDIRS=( $JAVA_PACKAGE/quick_java_examples/ )
#TESTDIRS=( /bat0/stac/Code/Ark2-Sandbox/WALi-OpenNWA/Category2_E1_E4_unmodified/ ) 
TESTDIRS=( $JAVA_PACKAGE/quick_java_examples/ $JAVA_PACKAGE/simple_java_examples/ $JAVA_PACKAGE/selected_canonical/ $JAVA_PACKAGE/new-canonical-examples/all_canonical_jars/ $NEWTON_DIR/Examples/cprover/tests/java/abstraction_ladder_1/ ) # current as of 2017-04-29

#/Code/Java-benchmarks/java_package/new-canonical-examples/all_canonical_jars

NEWTON="/bat0/stac/Code/CSFE_JAVA_API/params-icra-bytecode.sh"
#NEWTON="$NEWTON_DIR/_build64/Examples/cprover/NewtonOcaml"
#DUET="$DUET_DIR/duet.native"

PRINT_TXT="/bat0/stac/Code/CSFE_JAVA_API/print.txt"

# The outputs are saved here
OUTDIR="$SUITE/outputs"
# The input tests are copied here
INDIR="$SUITE/inputs"
RESULTJAVA="$OUTDIR/__result_java.out"
SCRIPT="$SUITE/toHTML_java.py"

#TIMEOUT="1m"
TIMEOUT="5m"

cd $NEWTON_DIR

mkdir -p $OUTDIR
mkdir -p $INDIR
rm -f $RESULTJAVA

echo "${#TESTDIRS[@]} directories detected."

for directory in ${TESTDIRS[@]}; do

    TESTS=( $directory/*.jar )
    echo "${#TESTS[@]} tests found in $directory"
    echo "__DIRECTORY $directory" >> $RESULTJAVA

    i=1
    for testf in ${TESTS[@]}; do
        below_outfile="$OUTDIR/$(basename $testf).below.out"
        #duet_outfile="$OUTDIR/$(basename $testf).duet.out"
        #above_outfile="$OUTDIR/$(basename $testf).above.out"
        sourcefile="${testf%.jar}.java"
        infile="$INDIR/$(basename $sourcefile)"
        # These variables are used for saving the intermediate .duet file
        duetfile="$INDIR/$(basename ${testf%.jar}.duet)"
        most_recent=/bat0/stac/Code/CSFE_JAVA_API/most-recent-temporary.duet
        #rm -f $outfile
        #rm -f $duet_outfile
        rm -f $infile
        cp $sourcefile $infile
        rm -f $most_recent

        echo -n "Running test $i of ${#TESTS[@]} ($(basename $testf))..."
        echo "__NAMEOFINPUT $(basename $sourcefile)" >> $RESULTJAVA
        echo "__NAMEOFOUTPUT $(basename $below_outfile)" >> $RESULTJAVA
        

        echo -n " ICRA/bytecode ..."
        start=$(date +%s%N)

        # Manage the "print.txt" file, which specifies the functions in which
        #   we will perform a print_hull, i.e., the functions for which we
        #   will ask for resource-usage bounds to be generated.
        rm $PRINT_TXT
        TEST_SPECIFIC_PRINT_TXT="${testf%.jar}_print.txt"
        if [ -f $TEST_SPECIFIC_PRINT_TXT ]; then
            echo -n " {Prnt} "
            cp $TEST_SPECIFIC_PRINT_TXT $PRINT_TXT
        else
            touch $PRINT_TXT
        fi
        
        # TODO: add support for _assume.txt files also...

        # Allow a per-test "_properties.txt" file that adds more properties
        #   to the Code Surfer properties file. (see phase1.sh for more info)
        # This is used to add things like "csir.abstract_strings = true".
        unset EXTRA_CSURF_PROPERTIES
        CSURF_PROPERTIES_FILE="${testf%.jar}_properties.txt"
        if [ -f $CSURF_PROPERTIES_FILE ]; then
            echo -n " {Prop} "
            export EXTRA_CSURF_PROPERTIES=`cat $CSURF_PROPERTIES_FILE`
            # The EXTRA_CSURF_PROPERTIES variable is read by phase1.sh,
            #   which is called by params-icra-bytecode.sh
        fi

        cd $NEWTON_DIR
        sourcefile="${testf%.jar}.java"
        #eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULTJAVA $testf &> $below_outfile"
        # Here was the original version:
        #eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULTJAVA $testf &> $below_outfile"
        #
        #
        rm -f $below_outfile
        #COMMAND="$NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops" # until 2017-08-01
        COMMAND="$NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops --bound-all bytecodecost "
        echo $COMMAND >> $below_outfile
        echo "" >> $below_outfile
		eval "timeout $TIMEOUT $COMMAND --test=$RESULTJAVA $testf &>> $below_outfile"
        #
        #eval "timeout $TIMEOUT $NEWTON -cra_newton_basic -cra-forward-inv -cra-split-loops --test=$RESULTJAVA $testf &> $below_outfile"
        success=$?
        if (($success==124)); then
            echo "__TIMEOUT" >> $RESULTJAVA
            echo -ne "\e[31mTimeout\e[0m"
        elif (($success!=0)); then
            echo "__EXCEPTION" >> $RESULTJAVA
            echo -ne "\e[31mException\e[0m"
        else
            echo "" >> $RESULTJAVA
        fi
        end=$(date +%s%N)
        len=$(expr $end - $start)
        echo "__NTIME $len" >> $RESULTJAVA

        cp $most_recent $duetfile # save the intermediate .duet file so that users can inspect it later
        
        ####echo -n " Duet ..."
        ####echo -n "__DUET " >> $RESULTJAVA
        #####cd $DUET_DIR
        ####eval "timeout 1s $DUET -cra -cra-forward-inv -cra-split-loops -test $RESULTJAVA $testf &> $duet_outfile"
        #####if (($?==124)); then
        ####    echo "TIMEOUT" >> $RESULTJAVA
        #####   echo -ne "\e[31mTimeout\e[0m"
        #####else
        #####   echo "" >> $RESULTJAVA
        #####fi
        ####
        ####echo -n " Above ..."
        ####start=$(date +%s%N)
        #####cd $NEWTON_DIR
        ####eval "timeout 1s $NEWTON -cra_newton_above -cra-forward-inv -cra-split-loops -cra-disable-simplify --test=$RESULTJAVA $testf &> $above_outfile"
        #####success=$?
        #####if (($success==124)); then
        ####    echo "__TIMEOUT" >> $RESULTJAVA
        #####   echo -e "\e[31mTimeout\e[0m"
        #####elif (($success!=0)); then
        #####   echo "__EXCEPTION" >> $RESULTJAVA
        #####   echo -e "\e[31mException\e[0m"
        #####else
        #####   echo "" >> $RESULTJAVA
        #####fi
        ####end=$(date +%s%N)
        ####len=$(expr $end - $start)
        ####echo "__NTIME $len" >> $RESULTJAVA
        
        echo " Done"
            
        let i+=1
    done
done

echo -n "Generating HTML ... "
cd $SUITE
python $SCRIPT || exit 1
echo "Done. Please see result_java.html"
