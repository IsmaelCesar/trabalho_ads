#!/bin/bash

pidmain=`echo pidof main`

echo `perf stat -e chache-misses -p $pidmain`
