
#!/bin/bash
#Rodar script em modo administrador
declare -a powers=("10" "32" "100" "317" "1000" "3163" "10000" "31623" "100000" "316228" "1000000" "3162278" "10000000" "31622777" "100000000" "316227766")

algoritmo="counting"
metodo="ordenacao"

gcc algoritmos_ordenacao_tempo.c -o main

./main ${powers[0]} > "tempo_ismael"/$metodo_$algoritmo_${powers[0]}.txt

echo "FINISHED!"
