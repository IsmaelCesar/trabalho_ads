#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

/*Procedimento para criar e inicializar um array de tamanho t
*e retorna-lo. O array sera inicializado com valores de tras pra frente
*/
int *inicializa_array(int t){
    int *array = calloc(t,sizeof(int));
    int cont = t;
    for(int i = 0; i < t ; i++){
        array[i] = cont--;
    }
    return array;
}

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
	int ret;
	ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
	return ret;
}

struct perf_event_attr* startPerfStruct(int type, int *fd){
	struct perf_event_attr *aux = calloc(1,sizeof(struct perf_event_attr));
	memset(aux, 0, sizeof(struct perf_event_attr));
	switch(type){
		case 1: //L1 CACHE
			aux->type = PERF_TYPE_HW_CACHE;
			aux->size = sizeof(struct perf_event_attr);
			aux->config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
			aux->disabled = 1;
			aux->exclude_kernel = 1;
			aux->exclude_hv = 1;
			aux->exclude_idle = 1;
			break;
		case 2: //LL CACHE
			aux->type = PERF_TYPE_HW_CACHE;
			aux->size = sizeof(struct perf_event_attr);
			aux->config = (PERF_COUNT_HW_CACHE_LL) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
			aux->disabled = 1;
			aux->exclude_kernel = 1;
			aux->exclude_hv = 1;
			aux->exclude_idle=1;
		case 3: //Program Counter
			aux->type = PERF_TYPE_HARDWARE;
			aux->size = sizeof(struct perf_event_attr);
			aux->config = PERF_COUNT_HW_INSTRUCTIONS;
			aux->disabled = 1;
			aux->exclude_kernel = 1;
			aux->exclude_hv = 1;
			aux->exclude_idle = 1;
			break;
		default:
			return 0;
	}
	*fd = perf_event_open(aux, 0, -1, -1, 0);
	if(*fd == -1){
		fprintf(stderr, "Error opening leader %llx\n", aux->config);
		exit(EXIT_FAILURE);
	}
	ioctl(*fd, PERF_EVENT_IOC_RESET, 0);
	return aux;
}


void ativarPerf(int fd){
	ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
}

void desativarPerf(int fd){
	ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
}

int main(){
    int fd_cacheL1, fd_cacheLL, fd_PC;
    struct perf_event_attr* cacheL1 = startPerfStruct(1, &fd_cacheL1);
    struct perf_event_attr* cacheLL = startPerfStruct(2, &fd_cacheLL);
    struct perf_event_attr* PC = startPerfStruct(3, &fd_PC);
    ativarPerf(fd_cacheL1);
    ativarPerf(fd_cacheLL);
    ativarPerf(fd_PC);
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
    desativarPerf(fd_cacheL1);
    desativarPerf(fd_cacheLL);
    desativarPerf(fd_PC);
    long long count;
    read(fd_cacheL1, &count, sizeof(long long));
    printf("Used %lld cache L1D misses\n", count);
    close (fd_cacheL1);
    read(fd_cacheLL, &count, sizeof(long long));
    printf("Used %lld cache LL Misses\n", count);
    close(fd_cacheLL);
    read(fd_PC, &count, sizeof(long long));
    printf("use %lld instructions\n", count);
    close(fd_PC);
    return 0;
}

