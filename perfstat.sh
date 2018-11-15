#!/bin/bash

pidmain=`pidof main`
echo  'PID:' $pidmain

perfevent=`perf list |grep cache-misses | awk '{print $1}'|sed -n '1p'`

#echo  'Evento monitorado:'$perfevent
out=`perf stat -B -e $perfevent -p $pidmain`

#echo $out
