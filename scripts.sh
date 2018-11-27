#!/bin/bash

gcc algoritmos_ordenacao.c -o main
i=0;
while [ $i -lt 1 ]
do
res= `./main`
misses= `echo $res | cut -f\ -f1`
acesses= `echo $res | cut -f\ -f2`
echo $misses
echo $references
i= `expr $i + 1`;
done
exit 0
