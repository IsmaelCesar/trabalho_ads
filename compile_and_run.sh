#!/bin/bash
#Rodar script em modo administrador
gcc algoritmos_ordenacao.c -o main

./main > "tempo"/cache_merge_t100000000.txt
