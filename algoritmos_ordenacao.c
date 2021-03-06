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
long long tam = 100000000;

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


//Procedimento auxiliar para calcular o pai de um elemento inserido.
void calcula_pai_ei(int *ei,int *paiEi){
    int tempEi = *ei;
    //ifte para verificar se ei é impar ou par e calcular quem é o elemento pai
    if(tempEi % 2 == 0)
        *paiEi = tempEi/2 -1;//divissao inteira
    else
        *paiEi = tempEi/2;
}

/* Procedimento auxiliar para construir uma
 * heap maxima apartir de arrays aleatorios
 * */
void constroi_heap_maxima(long long vetor[],int t){
    //Primeiro loop para insersao de elementos
    //ei -> Elemento inserido
    int ei = 0;
    for(int i=0;i < tam;i++){
        ei=i;
        int paiEi = 0;//Variavel auxiliar para o calculo do pai_ei
        calcula_pai_ei(&ei,&paiEi);
        //segundo loop para construcao da heaṕ maxima
        while(ei>0 ){
            if(vetor[ei] > vetor[paiEi]){
                //Efetua a troca
                long long temp= vetor[paiEi];
                vetor[paiEi] = vetor[ei];
                vetor[ei]= temp;
                ei = paiEi;
                calcula_pai_ei(&ei,&paiEi);
                //imprime_array(vetor,tam);
            }
            else
                ei = 0;//Força saida do loop
        }
        //imprime_array(vetor,tam);
    }

}

void heap_sort(long long vetor[], long long t){
    constroi_heap_maxima(vetor,t);//pega um array de inteiros aleatorios e constroi a heap maxima
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


int main(){
    float clks = CLOCKS_PER_SEC;
    int i = 0;
    /*char *cache_misses_res = (char*) malloc(1000*sizeof(char));
    char *cache_references_res = (char*) malloc(1000*sizeof(char));*/
    while(i < 30){
//	int fd_cache_misses,fd_cache_references;
//	struct perf_event_attr* cache_misses = startPerfStruct(11, &fd_cache_misses);
//	struct perf_event_attr* cache_references = startPerfStruct(12, &fd_cache_references);
//	ativarPerf(fd_cache_misses);
//	ativarPerf(fd_cache_references);
//	ioctl(fd_cache_misses, PERF_EVENT_IOC_RESET,0);
//        ioctl(fd_cache_references,PERF_EVENT_IOC_RESET,0);
        long long tic  = clock();
        long long *array = inicializa_array(tam);
//	  quicksort(array, 0, tam);
//        radixsort(array, tam);
//        insertion_sort(array, tam);
        bubble_sort(array, tam);
//        heap_sort(array, tam);
//        merge_sort(array,0,tam-1);
//        array = counting_sort(array, tam);
        long long toc = clock();
//        desativarPerf(fd_cache_misses);
//	desativarPerf(fd_cache_references);
//	char aux[100];
//	long long count;
//	read(fd_cache_misses, &count, sizeof(long long));
//	sprintf(aux, "%lld,",count);
//	strcat(cache_misses_res, aux);
//	read(fd_cache_references, &count,sizeof(long long));
//	sprintf(aux, "%lld,", count);
//	strcat(cache_references_res, aux);
        free(array);
        i++;
////	ativarPerf(fd_cache_misses);
////	ativarPerf(fd_cache_references);
//   	close(fd_cache_misses);
//   	close(fd_cache_references);
        printf("%f,",(toc - tic)/clks);
    }
    //printf("%s\t%s\n,", cache_misses_res,cache_references_res);

//    printf("%s\n", cache_references_res);
    // int fd_cacheL1_read_misses, fd_cacheLL_read_misses, fd_PC, fd_page_faults;
    // int fd_cacheL1_read_access, fd_cacheLL_read_access;
    // int fd_cacheL1_write_misses, fd_cacheLL_write_misses;
    // int fd_cacheL1_write_access, fd_cacheLL_write_access;
    // int fd_cache_misses, fd_cache_accesses;
    // struct perf_event_attr* cacheL1_read_misses = startPerfStruct(1, &fd_cacheL1_read_misses);
    // struct perf_event_attr* cacheL1_read_access = startPerfStruct(2, &fd_cacheL1_read_access);
    // //struct perf_event_attr* cacheL1_write_misses = startPerfStruct(3, &fd_cacheL1_write_misses);
    // struct perf_event_attr* cacheL1_write_access = startPerfStruct(4, &fd_cacheL1_write_access);

    // //struct perf_event_attr* cacheLL_read_misses = startPerfStruct(5, &fd_cacheLL_read_misses);
    // struct perf_event_attr* cacheLL_read_acess = startPerfStruct(6, &fd_cacheLL_read_access);
    // //struct perf_event_attr* cacheLL_write_misses = startPerfStruct(7, &fd_cacheLL_write_misses);
    // struct perf_event_attr* cacheLL_write_access = startPerfStruct(8, &fd_cacheLL_write_access);

    // struct perf_event_attr* PC      = startPerfStruct(9, &fd_PC);
    // struct perf_event_attr* PG	    = startPerfStruct(10, &fd_page_faults);

    // struct perf_event_attr* cache_misses = startPerfStruct(11, &fd_cache_misses);
    // //struct perf_event_attr* cache_accesses = startPerfStruct(12, &fd_cache_accesses);
    // //ativarPerf(fd_cacheL1_read_misses);
    // ativarPerf(fd_cacheL1_read_access);
    // //ativarPerf(fd_cacheL1_write_misses);
    // ativarPerf(fd_cacheL1_write_access);
    // //ativarPerf(fd_cacheLL_read_misses);
    // ativarPerf(fd_cacheLL_read_access);
    // //ativarPerf(fd_cacheLL_write_misses);
    // ativarPerf(fd_cacheLL_write_access);
    // ativarPerf(fd_PC);
    // ativarPerf(fd_page_faults);
    // ativarPerf(fd_cache_misses);
    // //ativarPerf(fd_cache_accesses);

    // desativarPerf(fd_cacheL1_read_misses);
    // desativarPerf(fd_cacheL1_read_access);
    // //desativarPerf(fd_cacheL1_write_misses);
    // desativarPerf(fd_cacheL1_write_access);
    // //desativarPerf(fd_cacheLL_read_misses);
    // desativarPerf(fd_cacheLL_read_access);
    // //desativarPerf(fd_cacheLL_write_misses);
    // desativarPerf(fd_cacheLL_write_access);
    // desativarPerf(fd_PC);
    // desativarPerf(fd_page_faults);
    // desativarPerf(fd_cache_misses);
    // //desativarPerf(fd_cache_accesses);
    // long long count;

    // read(fd_cacheL1_read_misses, &count, sizeof(long long));
    // printf("%lld cache L1D read misses\n", count);
    // close (fd_cacheL1_read_misses);

    // read(fd_cacheL1_read_access, &count, sizeof(long long));
    // printf("%lld cache L1D read accesses\n", count);
    // close (fd_cacheL1_read_access);

    // //read(fd_cacheL1_write_misses, &count, sizeof(long long));
    // //printf("%lld cache L1D write misses\n", count);
    // //close (fd_cacheL1_write_misses);

    // read(fd_cacheL1_write_access, &count, sizeof(long long));
    // printf("%lld cache L1D write accesses\n", count);
    // close (fd_cacheL1_write_access);

    // //read(fd_cacheLL_read_misses, &count, sizeof(long long));
    // //printf("%lld cache LL read misses\n", count);
    // //close(fd_cacheLL_read_misses);

    // read(fd_cacheLL_read_access, &count, sizeof(long long));
    // printf("%lld cache LL read accesses\n", count);
    // close (fd_cacheLL_read_access);

    // //read(fd_cacheLL_write_misses, &count, sizeof(long long));
    // //printf("%lld cache LL write misses\n", count);
    // //close (fd_cacheLL_write_misses);

    // read(fd_cacheLL_write_access, &count, sizeof(long long));
    // printf("%lld cache LLL write accesses\n", count);
    // close (fd_cacheLL_write_access);

    // read(fd_PC, &count, sizeof(long long));
    // printf("%lld instructions\n", count);
    // close(fd_PC);

    // read(fd_page_faults, &count,sizeof(long long));
    // printf("%lld page faults\n", count);
    // close(fd_PC);

    // read(fd_cache_misses, &count, sizeof(long long));
    // printf("%lld total cache misses\n", count);
    // close(fd_cache_misses);

    // //read(fd_cache_accesses, &count, sizeof(long long));
    // //printf("%lld total cache accesses\n", count);
    // //close(fd_cache_accesses);
}


