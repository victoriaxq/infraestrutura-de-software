#include <iostream>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

#define N 4 //num de incognitas
#define P 5 //num de iterações
#define NUM_THREADS 4

int A[N][N] = { //matriz A
    {8, 7, 8, 9},
    {1, 5, 4, 9},
    {4, 1, 2, 6},
    {2, 7, 6, 2}
    };
int b[N] = {7, 1, 2, 1}; //matriz b
int x[N] = {1, 1, 1, 1}; //matriz incognitas x inicial
float x_ref[P][N]; //matriz incognitas x refinadas

pthread_barrier_t mybarrier;

void *jacobino(void *thread_id){

    //inicializando variaveis

    int k = 0; // contador de iteraçoes
    int id = (*(int *)thread_id); 
    float soma = 0;
    float Aii = 0, bi = 0; 

    while( k < P){

        for(int i = id; i < N; i+=NUM_THREADS){
            soma = 0;
            Aii = A[i][i];
            bi = b[i];

            //somatorio
            for(int j = 0; j < N; j++){
                if(i!=j){
                    if(k==0){
                      soma += A[i][j];
                    }else{
                        soma += A[i][j]*x[j];
                    }
                }
            }
            x_ref[k][i] = (1/Aii)*(bi - soma); // formula
        }
        pthread_barrier_wait(&mybarrier); //fazendo as threads esperarem para nao dar problema nas linhas 51-53
        for(int i = id; i < N; i+=NUM_THREADS){
            x[i] = x_ref[k][i]; //passando incognitas refinadas para o vetor x para nova iteraçao
        }
        pthread_barrier_wait(&mybarrier);
        k++; 
    }
    pthread_exit(NULL);
}

int main(){

    //inicializando barreira, threads e afins
    pthread_barrier_init(&mybarrier, NULL, NUM_THREADS); 
    pthread_t threads[NUM_THREADS];
    int aux; 
    int *task_id[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++){
     task_id[i] = new int;
     *task_id[i] = i; //nao seria certo usar i como parametro pois ele sempre estara mudando devido ao loop
     aux = pthread_create(&threads[i], NULL, jacobino, (void *)task_id[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    } //esperar todas as threads acabarem suas respectivas execucoes

    //printando as incognitas refinadas
    for(int i = 0; i < P; i++){
        cout << "Para iteracao " << i << ":"<< endl;
        for(int j = 0; j < N; j++){
            cout << "x" << j << " = " << x_ref[i][j] << endl;
        }
        cout << endl;
    }
    
    return 0;
}