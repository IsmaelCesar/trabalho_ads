
#!/bin/bash
#Rodar script em modo administrador
#potencias
declare -a powers=("10" "32" "100" "317" "1000" "3163" "10000" "31623" "100000" "316228" "1000000" "3162278" "10000000" "31622777" "100000000" "316227766")

metodo="ordenacao"
algoritmo="counting"

gcc algoritmos_ordenacao_tempo.c -o main #Compilando implementacao
index=0                                  #indice para controle do array
while[index -lt 4]
do
    echo "Iteration $index"
    ./main ${powers[$index]} > "tempo_ismael"/$metodo_$algoritmo_${powers[$index]}.txt
done
echo "FINISHED!"
