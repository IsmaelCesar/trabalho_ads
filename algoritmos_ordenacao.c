#include <stdio.h>
#include <stlib.h>

int main(){
    int isFinishedRunning = 0;
    while(!isFinishedRunning){
        int opc = 0;        
        printf("Selecione o algoritmo de ordenacao:\n");
        printf("1->QuickSort\n");
        printf("2->RadixSort\n");
        printf("3->BucketSort\n");
        printf("4->InsertionSort\n");
        printf("5->BubbleSort\n");
        scanf("%d",&opc);
        
        switch(opc){
            case 1:
            printf("QuickSort Selecionado:\n");
            isFinishedRunning = 1;
            break;
            case 2:
            printf("RadixSort Selecionado:\n");
            isFinishedRunning = 1;
            break;
            case 3:
            printf("BucketSort Selecionado:\n");
            isFinishedRunning = 1;
            break;
            case 4:
            printf("InsertionSort Selecionado:\n");
            isFinishedRunning = 1;
            break;
            case 5:
            printf("BubbleSort Selecionado:\n");
            isFinishedRunning = 1;
            break;
            default:
            printf("Opcao invalida\n");
            break;            
        }
    }

    return 0;
}