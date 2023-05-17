#include <iostream>
#include <pthread.h>
long counter = 0; //contador global
#define qt_threads 5
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER; //inicializando mutex
using namespace std;

void *incrementa_counter(void *thread_id){

  int id = (*(int *)thread_id); //casting 

  while(counter < 1000000){
    while(pthread_mutex_trylock(&mymutex)!= 0){ //regiao critica -> usa o mutex para bloquear
      if(counter == 1000000){
        pthread_exit(NULL);
      }
    } 
    counter++;
    cout << "thread " << id << ": "<< counter << endl;
    if(counter == 1000000){
      cout << "a thread " << id << "fez o contador chegar a 1.000.000!" << endl;
    }
    pthread_mutex_unlock(&mymutex); //saída da região critica
  }
  pthread_exit(NULL);
}

int main(){
  //inicializando variaveis como 0
    int aux = 0;
    int *res = 0;

  //declarando as threads e seu vetor de id's
    pthread_t threads[qt_threads];
    int *task_id[qt_threads];

    for(int i = 0; i < qt_threads; i++){
     task_id[i] = new int;
     *task_id[i] = i; //nao seria certo usar i como parametro pois ele sempre estara mudando devido ao loop
     aux = pthread_create(&threads[i], NULL, incrementa_counter, (void *)task_id[i]);
    }
    pthread_exit(NULL);

    return 0;
}