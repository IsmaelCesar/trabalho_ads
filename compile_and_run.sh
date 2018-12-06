#!/bin/bash
#Rodar script em modo administrador
#potencias
declare -a powers=("10" "32" "100" "317" "1000" "3163" "10000" "31623" "100000" "316228" "1000000" "3162278" "10000000" "31622777" "100000000" "316227766")

metodo="ordenacao"
declare -a algoritmo=("quick" "radix" "heap" "merge" "counting" "insertion" "bubble")

gcc algoritmos_ordenacao_tempo.c -o main #Compilando implementacao
opc_alg=0
while (( opc_alg < 1))
do
    index=0                                  #indice para controle do array
    echo "Algoritmo ${algoritmo[(( opc_alg ))]}"

    while (( index < 16 ))
    do
        echo "Iteration $index"

        ./main ${powers[(( index ))]} $opc_alg > tempo_ismael/"$metodo"_"${algoritmo[(( opc_alg ))]}"_${powers[(( index ))]}.txt

        (( index ++ ))
    done

    (( opc_alg ++ ))
done    
echo "FINISHED!"
