#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void gera_vetor(int v[], int n){
    int i;
    for(i=0; i<n; i++){
        v[i]=(rand()*rand())%(n*10);
    }
}

void bubblesort(int v[], int n){
    int i, j, aux;
    for(i=1; i<n; i++){
        for(j=0; j<n-i; j++){
            if(v[j]>v[j+1]){
                aux=v[j];
                v[j]=v[j+1];
                v[j+1]=aux;
            }
        }
    }
}

void selectionsort(int v[], int n){
    int i, j, pos_menor, aux;
    for(i=0; i<n-1; i++){
        pos_menor=i;
        for(j=1+i; j<n; j++){
            if(v[j]<v[pos_menor]){
                pos_menor=j;
            }
        }
        if(pos_menor != i){
            aux=v[i];
            v[i]=v[pos_menor];
            v[pos_menor]=aux;
        }
    }
}

void insertionsort( int v[], int n){
    int i, j, maior;
    for(i=1; i<n; i++){
        maior=v[i];
        j=i-1;
        while(j>=0 && v[j]>maior){
            v[j+1]=v[j];
            j--;
        }
        v[j+1]=maior;
    }
}

struct parametros_threads{
    int num_dados;
    int num_loops;
    int num_mult;
    char *nome_arquivo;
    int num_thread;
};

void * funcao_threads(void * args){
    struct parametros_threads *argumentos=(struct parametros_threads *)args;
    int *v, met, k, j;
    time_t ini, fim;
    char buffer[100];
    FILE *pont_arq;
    sprintf(buffer, "%d%s",argumentos->num_thread, argumentos->nome_arquivo);
    pont_arq=fopen(buffer, "w");
    for(j=1; j<=argumentos->num_mult; j++){
        met=1;
        v=(int *)malloc(argumentos->num_dados*sizeof(int));
        fprintf(pont_arq, "QUANTIDADE DE DADOS: %d\n", argumentos->num_dados);
        while(met<=3){
            for(k=1; k<=argumentos->num_loops; k++){
                gera_vetor(v, argumentos->num_dados);
                switch(met){
                case 1:
                    fprintf(pont_arq, "\nVetor gerado => Ordenacao com Bubble Sort");
                    ini=time(NULL);
                    bubblesort(v, argumentos->num_dados);
                    fim=time(NULL);
                    break;
                case 2:
                    fprintf(pont_arq, "\nVetor gerado => Ordenacao com Selection Sort.");
                    ini=time(NULL);
                    selectionsort(v, argumentos->num_dados);
                    fim=time(NULL);
                    break;
                case 3:
                    fprintf(pont_arq, "\nVetor gerado => Ordenacao com Insertion Sort.");
                    ini=time(NULL);
                    insertionsort(v, argumentos->num_dados);
                    fim=time(NULL);
                    break;
                }
                fprintf(pont_arq, "\n                Tempo requerido: %ld", fim-ini);
                fflush(pont_arq);
            }
            met++;
            fprintf(pont_arq, "\n");
        }
        free(v);
        argumentos->num_dados*=2;
        fprintf(pont_arq, "\n");
    }
    fclose(pont_arq); 
    pthread_exit(NULL);

}

int main(int argc, char *argv[])
{   srand(time(NULL));
    int n=10000, n_loops=10, n_mult=1, n_threads=1, i, cr_thread;
    char *nome_arq;
    switch(argc){
        case 1:
            nome_arq="saida.txt";
            break;
        case 2:
            nome_arq=argv[1];
            break;
        case 3:
            nome_arq=argv[1];
            n=atoi(argv[2]);
            break;
        case 4:
            nome_arq=argv[1];
            n=atoi(argv[2]);
            n_loops=atoi(argv[3]);
            break;
        case 5:
            nome_arq=argv[1];
            n=atoi(argv[2]);
            n_loops=atoi(argv[3]);
            n_mult=atoi(argv[4]);
            break;
        case 6:
            nome_arq=argv[1];
            n=atoi(argv[2]);
            n_loops=atoi(argv[3]);
            n_mult=atoi(argv[4]);
            n_threads=atoi(argv[5]);
            break;
    }
    struct parametros_threads par_thread[n_threads];
    pthread_t threads[n_threads];
    for(i=0; i<n_threads; i++){
        par_thread[i].num_dados=n;
        par_thread[i].num_loops=n_loops;
        par_thread[i].num_mult=n_mult;
        par_thread[i].nome_arquivo=nome_arq;
        par_thread[i].num_thread=i+1;
        cr_thread=pthread_create(&threads[i], NULL, funcao_threads, (void*)(par_thread+i));
    }
    for(i=0; i<=n_threads; i++){
        pthread_join(threads[i], NULL);
    }


    return 0;
}