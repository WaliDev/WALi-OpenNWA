#!/bin/sh

find examples_c -name "*.c" | while read file; do
    echo -n $file
    STARTTIME=$(date +"%s%3N")
    timeout 60 ~/src/duet/duet.native -cra-forward-inv -cra-split-loops -cra $file >/dev/null
    ENDTIME=$(date +"%s%3N")
    echo "\t" $(( $ENDTIME - $STARTTIME ))
done
