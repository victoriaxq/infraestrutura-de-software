#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#define NUM_VERTEX 5
using namespace std;
int counterG = 0;
int counterH = 0;
/*
Condições para deadlock acontecer:
1.Exclusão mútua
2.Posse e espera
3.Não preempção
4.Espera circular
*/

//Struct para Lista de Adjacencia do Grafo
typedef struct list{
    struct list *next;
    int value;
}List;

//Struck para cada Vertice do Grafo
typedef struct vertex{
    List *sentinel;
    List *tail;
    bool visited;
    int grade;
}Vertex;

Vertex *g; // Grafo global

//List insert para fazer a lista de adj
Vertex list_insert(Vertex g, int value){
    List *node = new List;
    node->value = value;
    node->next = NULL;
    g.tail->next = node;
    g.tail = node;
    g.grade++;

    return g;
}

//funçao de printar listas para checar se esta tudo em ordem
void print_list(List *list, int vertex){
    List *aux = list;
    aux = aux->next;
    cout << "Lista do vertice " << vertex << ": ";
    while(aux){
        cout << aux->value << "|";
        aux = aux->next;
    }
    cout << endl;
}

void DFS_visit(Vertex *g, int V_ORIGIN, bool visited[], int id){

    visited[V_ORIGIN] = 1;
    int target = 0;
    List *e = g[V_ORIGIN].sentinel->next;

    while(e != NULL){
        target = e->value;
        if(visited[target] == 0){
            DFS_visit(g, target, visited, id); 
        }
        if(id == target){ //detecçao de ciclo
            cout << "ciclo detectado no grafo G pela thread "<< id <<"!"<< endl;
            counterG++;
            
        }
        e = e->next;
    }
}

void *DFS(void *ID){
    int id = *(int *)ID;
    bool visited[NUM_VERTEX] = {0};
    DFS_visit(g, id, visited, id);
    pthread_exit(NULL);
}

int main(){

    //vetor de visitados inicializando com 0
    bool visited[NUM_VERTEX] = {0};
    counterG = 0;
    counterH = 0;

    //inicializando grafo ciclico
    g = (Vertex *)calloc(NUM_VERTEX, sizeof(Vertex));

    for(int i = 0; i < NUM_VERTEX; i++){
        g[i].sentinel = new List;
        g[i].sentinel->next = NULL;
        g[i].tail = g[i].sentinel;
        g[i].grade = 0;
        g[i].visited = 0;
    }

    //fazendo arestas
    //foto do grafo ciclico escolhido: https://drive.google.com/file/d/19bJ3BfbTj2pYDg4xyVSmC7gDPCMdoFrk/view?usp=sharing
    
    g[0] = list_insert(g[0], 1);
    g[1] = list_insert(g[1], 2);
    g[2] = list_insert(g[2], 3);
    g[2] = list_insert(g[2], 4);
    g[4] = list_insert(g[4], 1);

    //criando threads
    pthread_t threadsG[NUM_VERTEX];
    int *idG[NUM_VERTEX];

    for(int i = 0; i < NUM_VERTEX; i++){
     idG[i] = new int;
     *idG[i] = i; //nao seria certo usar i como parametro pois ele sempre estara mudando devido ao loop
     int auxG = pthread_create(&threadsG[i], NULL, DFS, (void *)idG[i]);
    }
    
    /*
    o algoritmo deve mostrar 4 vezes que o ciclo
    foi detectado pois, para o grafo escolhido,
    existem 4 caminhos diferentes que chegam ao
    deadlock
    */

    pthread_exit(NULL);
    return 0;
}