#!/bin/sh

find examples_koat -name "*.koat.txt" | while read file; do
    echo $file
    dir=`dirname $file`
    newdir=`echo $dir | sed s/examples_koat/examples_c/`
    newfile=`echo $file.c | sed s/examples_koat/examples_c/`
    mkdir -p $newdir
    ./koatCConv.native $file > $newfile
done
