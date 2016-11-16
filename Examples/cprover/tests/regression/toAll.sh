#!/bin/bash
#
# This script performs all the necessary steps for creating supplementary 
#   materials submission, like the supplementary materials for PLDI17.
# 
# It assumes that you have already done the experiments by running both:
#
#   test_suite_compareFiveTools.sh
#
#  and
#
#   test_suite_printHull.sh

# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-which-directory-it-is-stored-in
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Generating assertion-checking HTML"
python $SCRIPTDIR/toHTML.py
echo "Generating bounds-generation HTML"
python $SCRIPTDIR/toPrintHullHTML.py
echo "Generating CSV data"
python $SCRIPTDIR/toCSV.py
echo "Generating Latex table"
python $SCRIPTDIR/toLatex.py
echo "Generating plots and summary text"
python $SCRIPTDIR/toPlotsAndSummaryText.py
echo "Generating supplementary material"
python $SCRIPTDIR/toSupplementaryMaterial.py
