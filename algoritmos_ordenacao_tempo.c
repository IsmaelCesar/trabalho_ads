#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

//long long int tam = 1000000000;
long long tam = 316227766;

/*Procedimento para criar e inicializar um array de tamanho t
*e retorna-lo. O array sera inicializado com valores de tras pra frente
*/
long long *inicializa_array(long long t){
//    printf("Iniciado Geracao de valores\n");
    srand(time(NULL));
//    printf("%d\n", t);
    long long *array = (long long*) malloc(t*sizeof(long long));
//    printf("Array Criado\n");
    int i;
    for(i = 0; i < t ; i++){
//        printf("Iniciado elemento %d\n", i+1);
        array[i] = rand()%0x7FFFFFE + 1;
    }
//    printf("REtornando vetor\n");
    return array;
}

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
    int ret;
    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

struct perf_event_attr* startPerfStruct(int type, int *fd){
//    printf("%d\n", type);
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

// Procedimento auxiliar para efetuar o merge sort
void merge(long long *array,long long l,long long m , long long r){
    long long i, j, k;
    long long n1 = m - l + 1;
    long long n2 =  r - m;

    /* create temp arrays */
    //long long L[n1],R[n2];
    long long *L,*R;
    L =  malloc(n1*sizeof(long long));
    R =  malloc(n2*sizeof(long long));

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = array[m + 1 +j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    for(k = l; k <= r;  k++){
        if(i<n1 && j<n2){
            if(L[i]<=R[j]){
                array[k] = L[i++];
            }
            else{
                array[k] = R[j++];
            }
        }
        else{
            if(i<n1){
                array[k]=L[i++];
            }
            if(j<n2){
                array[k]=R[j++];
            }
        }
    }

   free(L);
   free(R);
}

//Procedimento que  efetua a execucao do merge sort
//ao efetuar a primeira chamada, faze-la passando r = r-1;
void merge_sort(long long *array, long long l, long long r){
    if(l < r) {
        long long middle = (long long) floor((l + r) / 2);
        merge_sort(array, l, middle);
        merge_sort(array, middle+1,r);
        merge(array,l,middle,r);
    }

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

long long* counting_sort(long long *v, int tam) {
    long long i;
    long long maior = v[0];

    for (i = 1; i < tam; i++) {
        if (v[i] > maior) {
            maior = v[i];
        }
    }

    long long *c = malloc(maior*sizeof(long long));

    for (i = 0; i < tam; i++) {
        c[v[i]-1]++;
    }

    for (i = 1; i < maior; i++){
        c[i] += c[i - 1];
    }
    long long *b = malloc(maior*sizeof(long long));
    for(i = 0; i < tam;i++) {
        b[--c[v[i] - 1]] = v[i];
    }
    free(c);
    free(v);
    return b;
}


int main(int argc, char *argv[]){
    float clks = CLOCKS_PER_SEC;
    int opc = 0;
    int i = 0;

    if(argc ==3){
	    char *a = argv[1];
	    tam     = atoll(a);
        char *b = argv[2];
        opc     = atoi(b);
    }
    // char *cache_misses_res = (char*) malloc(1000*sizeof(char));
    // char *cache_references_res = (char*) malloc(1000*sizeof(char));
    while(i < 30){
        // int fd_cache_misses,fd_cache_references;
        // struct perf_event_attr* cache_misses = startPerfStruct(11, &fd_cache_misses);
        // struct perf_event_attr* cache_references = startPerfStruct(12, &fd_cache_references);
        // ativarPerf(fd_cache_misses);
        // ativarPerf(fd_cache_references);
        // ioctl(fd_cache_misses, PERF_EVENT_IOC_RESET,0);
        // ioctl(fd_cache_references,PERF_EVENT_IOC_RESET,0);
        long long tic  = clock();
        long long *array = inicializa_array(tam);
        switch(opc){
            case 0:
                quicksort(array, 0, tam);
            break;                
            case 1:
                radixsort(array, tam);  
            break;
            case 2:
                heap_sort(array, tam);       
            break;
            case 3:
                merge_sort(array,0,tam-1);
            break;
            case 4:
                array = counting_sort(array, tam);
            break;
            case 5:
                insertion_sort(array, tam);
            break;
            case 6:
                bubble_sort(array, tam);
            break;
        }
        long long toc = clock();
        // desativarPerf(fd_cache_misses);
        // desativarPerf(fd_cache_references);
        // char aux[100];
        // long long count;
        // read(fd_cache_misses, &count, sizeof(long long));
        // sprintf(aux, "%lld,",count);
        // strcat(cache_misses_res, aux);
        // read(fd_cache_references, &count,sizeof(long long));
        // sprintf(aux, "%lld,", count);
        // strcat(cache_references_res, aux);
        free(array);
        i++;
        // ativarPerf(fd_cache_misses);
        // ativarPerf(fd_cache_references);
        // close(fd_cache_misses);
        // close(fd_cache_references);
        printf("%f,",(toc - tic)/clks);
    }
    //printf("%s\t%s\n,", cache_misses_res,cache_references_res);

//    printf("%s\n", cache_references_res);
    
}


