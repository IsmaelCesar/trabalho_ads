#!/bin/bash

pidmain=`pidof main`
echo $pidmain

perfevent=`perf list |grep cache-misses | awk '{print $1}'|sed -n '1p'`

echo $perfevent
out=`perf stat -e $perfevent -p $pidmain`

#echo $out
