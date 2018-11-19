#!/bin/bash

pidmain=`pidof main`
echo  'PID:' $pidmain

#eventos a serem monitorados
cacheMissesEv=`perf list |grep cache-misses | awk '{print $1}'|sed -n '1p'`
L1dCacheLoads=`perf list | grep L1-dcache-loads|awk '{print $1}'`
#echo  'Evento monitorado:'$perfevent
out=`perf stat -B -e $cacheMissesEv -p $pidmain`

#echo $out
