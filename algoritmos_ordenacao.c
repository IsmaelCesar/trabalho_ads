#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

//long long int tam = 1000000000;
long long tam = 100000000;

/*Procedimento para criar e inicializar um array de tamanho t
*e retorna-lo. O array sera inicializado com valores de tras pra frente
*/
long long *inicializa_array(long long t){
    srand(time(NULL));
    long long *array = malloc(t*sizeof(long long));
    long long cont = t;
    for(int i = 0; i < t ; i++){
        array[i] = rand();
    }
    return array;
}

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
    int ret;
    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

struct perf_event_attr* startPerfStruct(int type, int *fd){
    printf("%d\n", type);
    struct perf_event_attr *aux = calloc(1,sizeof(struct perf_event_attr));
    memset(aux, 0, sizeof(struct perf_event_attr));
    aux->size = sizeof(struct perf_event_attr);
    aux->disabled = 1;
    aux->exclude_kernel = 1;
    aux->exclude_hv = 1;
    aux->exclude_idle = 1;
//    aux->exclude_user = 1;
    switch(type){
        case 1: //L1 CACHE READ MISSES
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case 2: //L1 CACHE READ ACCESS
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case 3: //L1 CACHE WRITE MISSES
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_WRITE << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case 4: //L1 CACHE WRITE ACCESS
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_WRITE << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case 5: //LL CACHE READ MISSES
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_LL) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case 6: //LL CACHE READ ACCESSES
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_LL) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case 7: //LL CACHE WRITE MISSES
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_LL) | (PERF_COUNT_HW_CACHE_OP_WRITE << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case 8: //LL CACHE WRITE ACCESSES
            aux->type = PERF_TYPE_HW_CACHE;
            aux->config = (PERF_COUNT_HW_CACHE_LL) | (PERF_COUNT_HW_CACHE_OP_WRITE << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);
            break;
        case 9: //PAGE FAULTS
            aux->type = PERF_TYPE_HARDWARE;
            aux->config = PERF_COUNT_HW_INSTRUCTIONS;
            break;
        case 10: //PROGRAM COUNTER
            aux->type = PERF_TYPE_SOFTWARE;
            aux->config = PERF_COUNT_SW_PAGE_FAULTS;
            break;
	case 11: //CACHE MISSES
	    aux->type = PERF_TYPE_HARDWARE;
	    aux->config = PERF_COUNT_HW_CACHE_MISSES;
	    break;
	case 12:
	    aux->type = PERF_TYPE_HARDWARE;
	    aux->config = PERF_COUNT_HW_CACHE_REFERENCES;
	    break;

    }
    *fd = perf_event_open(aux, 0, -1, -1, 0);
    //printf("%d ", type);
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

//          ALOGRITMOS DE ORDENACAO

void quicksort(long long values[], long long began, long long end)
{
    long long  i, j, pivo, aux;
    i = began;
    j = end-1;
    pivo = values[(began + end) / 2];
    while(i <= j)
    {
        while(values[i] < pivo && i < end)
        {
            i++;
        }
        while(values[j] > pivo && j > began)
        {
            j--;
        }
        if(i <= j)
        {
            aux = values[i];
            values[i] = values[j];
            values[j] = aux;
            i++;
            j--;
        }
    }
    if(j > began)
        quicksort(values, began, j+1);
    if(i < end)
        quicksort(values, i, end);
}

void radixsort(long long  vetor[], long long tamanho) {
    long long i;
    long long *b;
    long long maior = vetor[0];
    long long exp = 1;

    b = (long long *)calloc(tamanho, sizeof(long long));

    for (i = 0; i < tamanho; i++) {
        if (vetor[i] > maior)
            maior = vetor[i];
    }

    while (maior/exp > 0) {
        long long bucket[10] = { 0 };
        for (i = 0; i < tamanho; i++)
            bucket[(vetor[i] / exp) % 10]++;
        for (i = 1; i < 10; i++)
            bucket[i] += bucket[i - 1];
        for (i = tamanho - 1; i >= 0; i--)
            b[--bucket[(vetor[i] / exp) % 10]] = vetor[i];
        for (i = 0; i < tamanho; i++)
            vetor[i] = b[i];
        exp *= 10;
    }

    free(b);
}

void insertion_sort(long long vetor[],long long t){
    long long e, j;

    for (long long i = 1; i < t; i++) {
        e = vetor[i];
        j = i - 1;

        while ((j >= 0) && (vetor[j] > e)) {
            vetor[j + 1] = vetor[j];
            j--;
        }

        vetor[j + 1] = e;
    }
}


void bubble_sort(long long vetor[], long long t) {
    long long i, j, temp;

    for (i = 0; i < t - 1; i++){
        for (j = (i+1); j < t; j++){
            if (vetor[j] < vetor[i]){
                temp = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = temp;
            }
        }
    }
}


void heap_sort(long long vetor[], long long t){
    int i = t-1;
    while( i > 0){
        //troca
        long long temp = vetor[0];
        vetor[0] = vetor[i];
        vetor[i] = temp;

        i--;
        //Transforma em heap maxima
        long long fesq = 0;
        long long fdir = 0;
        long long maior = -1;
        long long j = 0;
        long long j_m1 = j-1;
        while(j_m1 != j){
            fesq = 2 * j+1;
            fdir = 2*j+2;

            if(fesq<= i && vetor[fesq] > vetor[j])
                maior = fesq;
            else
                maior = j;
            if(fdir <= i && vetor[fdir] > vetor[maior])
                maior = fdir;

            if(maior != j) {
                long long  temp_2 = vetor[j];
                vetor[j] = vetor[maior];
                vetor[maior] = temp_2;
                j_m1 = j;
            }
            else{
                j_m1 = maior;
            }
            j = maior;
        }
    }
}


int main(){
    long long *array = inicializa_array(tam);
    int fd_cacheL1_read_misses, fd_cacheLL_read_misses, fd_PC, fd_page_faults;
    int fd_cacheL1_read_access, fd_cacheLL_read_access;
    int fd_cacheL1_write_misses, fd_cacheLL_write_misses;
    int fd_cacheL1_write_access, fd_cacheLL_write_access;
    int fd_cache_misses, fd_cache_accesses;
    struct perf_event_attr* cacheL1_read_misses = startPerfStruct(1, &fd_cacheL1_read_misses);
    struct perf_event_attr* cacheL1_read_access = startPerfStruct(2, &fd_cacheL1_read_access);
    //struct perf_event_attr* cacheL1_write_misses = startPerfStruct(3, &fd_cacheL1_write_misses);
    struct perf_event_attr* cacheL1_write_access = startPerfStruct(4, &fd_cacheL1_write_access);

    struct perf_event_attr* cacheLL_read_misses = startPerfStruct(5, &fd_cacheLL_read_misses);
    struct perf_event_attr* cacheLL_read_acess = startPerfStruct(6, &fd_cacheLL_read_access);
    struct perf_event_attr* cacheLL_write_misses = startPerfStruct(7, &fd_cacheLL_write_misses);
    struct perf_event_attr* cacheLL_write_access = startPerfStruct(8, &fd_cacheLL_write_access);

    struct perf_event_attr* PC      = startPerfStruct(9, &fd_PC);
    struct perf_event_attr* PG	    = startPerfStruct(10, &fd_page_faults);

    struct perf_event_attr* cache_misses = startPerfStruct(11, &fd_cache_misses);
    struct perf_event_attr* cache_accesses = startPerfStruct(12, &fd_cache_accesses);
    ativarPerf(fd_cacheL1_read_misses);
    ativarPerf(fd_cacheL1_read_access);
    //ativarPerf(fd_cacheL1_write_misses);
    ativarPerf(fd_cacheL1_write_access);
    ativarPerf(fd_cacheLL_read_misses);
    ativarPerf(fd_cacheLL_read_access);
    ativarPerf(fd_cacheLL_write_misses);
    ativarPerf(fd_cacheLL_write_access);
    ativarPerf(fd_PC);
    ativarPerf(fd_page_faults);
    ativarPerf(fd_cache_misses);
    ativarPerf(fd_cache_accesses);

    int opc = 3;
    switch(opc){
        case 1:
            quicksort(array,0, tam);
            break;
        case 2:
            radixsort(array, tam);
            break;
        case 3:
            insertion_sort(array,tam);
            break;
        case 4:
            bubble_sort(array,tam);
            break;
        case 5:
            heap_sort(array,tam);
            break;
        default:
            printf("Opcao invalida\n");
            break;
    }
    desativarPerf(fd_cacheL1_read_misses);
    desativarPerf(fd_cacheL1_read_access);
    //desativarPerf(fd_cacheL1_write_misses);
    desativarPerf(fd_cacheL1_write_access);
    desativarPerf(fd_cacheLL_read_misses);
    desativarPerf(fd_cacheLL_read_access);
    desativarPerf(fd_cacheLL_write_misses);
    desativarPerf(fd_cacheLL_write_access);
    desativarPerf(fd_PC);
    desativarPerf(fd_page_faults);
    desativarPerf(fd_cache_misses);
    desativarPerf(fd_cache_accesses);
    long long count;

    read(fd_cacheL1_read_misses, &count, sizeof(long long));
    printf("%lld cache L1D read misses\n", count);
    close (fd_cacheL1_read_misses);

    read(fd_cacheL1_read_access, &count, sizeof(long long));
    printf("%lld cache L1D read accesses\n", count);
    close (fd_cacheL1_read_access);

    //read(fd_cacheL1_write_misses, &count, sizeof(long long));
    //printf("%lld cache L1D write misses\n", count);
    //close (fd_cacheL1_write_misses);

    read(fd_cacheL1_write_access, &count, sizeof(long long));
    printf("%lld cache L1D write accesses\n", count);
    close (fd_cacheL1_write_access);

    read(fd_cacheLL_read_misses, &count, sizeof(long long));
    printf("%lld cache LL read misses\n", count);
    close(fd_cacheLL_read_misses);

    read(fd_cacheLL_read_access, &count, sizeof(long long));
    printf("%lld cache LL read accesses\n", count);
    close (fd_cacheLL_read_access);

    read(fd_cacheLL_write_access, &count, sizeof(long long));
    printf("%lld cache LL write misses\n", count);
    close (fd_cacheLL_write_misses);

    read(fd_cacheLL_write_access, &count, sizeof(long long));
    printf("%lld cache LLL write accesses\n", count);
    close (fd_cacheLL_write_misses);

    read(fd_PC, &count, sizeof(long long));
    printf("%lld instructions\n", count);
    close(fd_PC);

    read(fd_page_faults, &count,sizeof(long long));
    printf("%lld page faults\n", count);
    close(fd_PC);

    read(fd_cache_misses, &count, sizeof(long long));
    printf("%lld total cache misses\n", count);
    close(fd_cache_misses);

    read(fd_cache_accesses, &count, sizeof(long long));
    printf("%lld total cache accesses\n", count);
    close(fd_cache_accesses);

    float clks = CLOCKS_PER_SEC;
    printf("%.2fs\n", clock()/clks);
    return 0;
}

