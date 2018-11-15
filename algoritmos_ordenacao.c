#include <stdio.h>
#include <stdlib.h>

/*Procedimento para criar e inicializar um array de tamanho t
*e retorna-lo. O array sera inicializado com valores de tras pra frente
*/
int *inicializa_array(int t){
    int *array = calloc(t,sizeof(int));
    for(int i = 0; i < t ; i++){
        array[i] = t--;
    }
    return array;
}



int main(){
    int isFinishedRunning = 0;
    int *array = NULL;
    while(!isFinishedRunning){
        int opc = 0;        
        printf("Selecione o algoritmo de ordenacao:\n");
        printf("1->QuickSort\n");
        printf("2->RadixSort\n");
        printf("3->BucketSort\n");
        printf("4->InsertionSort\n");
        printf("5->BubbleSort\n");
        printf("0->sair\n");
        scanf("%d",&opc);            
        int tam = 0;
        switch(opc){
            case 0:
            isFinishedRunning = 1;
            break;
            case 1:
                printf("QuickSort Selecionado:\n");                                    
                printf("Digite o tamanho do array a ser ordenado:");
                scanf("%d\n",&tam);
                array = inicializa_array(tam);
            break;
            case 2:
                printf("RadixSort Selecionado:\n");
                printf("Digite o tamanho do array a ser ordenado:");
                scanf("%d\n",&tam);
                array = inicializa_array(tam);
            break;
            case 3:
            printf("BucketSort Selecionado:\n");
                printf("Digite o tamanho do array a ser ordenado:");
                scanf("%d\n",&tam);
                array = inicializa_array(tam);
            break;
            case 4:
            printf("InsertionSort Selecionado:\n");            
                printf("Digite o tamanho do array a ser ordenado:");
                scanf("%d\n",&tam);
                array = inicializa_array(tam);
            break;
            case 5:
            printf("BubbleSort Selecionado:\n");
                printf("Digite o tamanho do array a ser ordenado:");
                scanf("%d\n",&tam);
                array = inicializa_array(tam);
            break;
            default:
            printf("Opcao invalida\n");
            break;            
        }
    }

    return 0;
}