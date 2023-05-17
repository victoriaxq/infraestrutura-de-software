#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define B 5 // TaMANHo MÁXIMO DO BUFFER
#define N 2 // Representa a quantidade de processadores ou núcleos do sistema

pthread_t threads[N];
// Parametros para as funexec
typedef struct parametro {
    int p1;
    int idRequisicao;
    int idThread;
}ParamFunExec;

// Struct pra salvar cada requisicao no buffer
typedef struct requisicao {
    void *func;
    ParamFunExec p;
}Buffer;


Buffer *bufferRequisicoes;
int *bufferResultados;
int threadOcupada[N] = {0};
int sizeRequisicoes = 0;
int sizeResultados = 0;
int out = 0; // index para a prox posicao do buffer que vai ser despachada
int in = 0; // index para prencher a prox posicao do buffer do agendamento

pthread_cond_t newRequisicao = PTHREAD_COND_INITIALIZER; 
pthread_cond_t newResultado = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutexResultado = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t mutexRequisicao = PTHREAD_MUTEX_INITIALIZER; 

// FUNÇÕES PEDIDAS PELA QUESTÃO
int agendarExecucao(void *funexec, ParamFunExec parameters)
{
    pthread_mutex_lock(&mutexRequisicao);
    Buffer req; 
    req.p = parameters; 
    req.func = funexec;

    printf("Agendando requisicao %d...\n", in);
    req.p.idRequisicao = in;
    bufferRequisicoes[in] = req; // coloca a requisicao no buffer
    sizeRequisicoes++; 
    in++;
    in = in % B;  // se chegou no tamanho máximo, temos que voltar a colocar as requisicoes no inicio do buffer, então in = 0
    if (sizeRequisicoes == 1) pthread_cond_broadcast(&newRequisicao); // se nao tinha nenhuma outra requisicao ele emite o sinal de uma nova requisicao para a thread despachante
    pthread_mutex_unlock(&mutexRequisicao);

    return req.p.idRequisicao;
}

int pegarResultadoExecucao(int id)
{
    pthread_mutex_lock(&mutexResultado);
    int resultado = 0;
    while (sizeResultados == 0 || (sizeResultados > 0 && bufferResultados[id] == 0)) {
        // Se o bufferResultados está vazio ou se o resultado que eu quero não chegou, vamos esperar o sinal newResultado até chegar o resultado.
        pthread_cond_wait(&newResultado, &mutexResultado);
    }
    // salvo a resposta e depois coloco 0 naquela posicao do buffer, esse vai ser o valor de quando não temos nada naquela posicao.
    printf("Pegando o resultado do id %d...\n", id);
    resultado = bufferResultados[id];
    bufferResultados[id] = 0;
    sizeResultados--;
    pthread_mutex_unlock(&mutexResultado);
    return resultado;
}

// Função para execuçao
void *square(void *parameters) {

    ParamFunExec *p = (ParamFunExec *) parameters;
    
    int result ;
    result =  p->p1 * p->p1;
    pthread_mutex_lock(&mutexResultado);
    bufferResultados[p->idRequisicao] = result;
    sizeResultados++;
    pthread_cond_signal(&newResultado);
    pthread_mutex_unlock(&mutexResultado);

    threadOcupada[p->idThread] = 0;
    printf("Funcao SQUARE emitiu novo resultado, thread %d está livre.\n", p->idThread);
    pthread_exit(NULL);
}

// FUNÇÃO THREAD

void *despachante() {
    pthread_mutex_lock(&mutexRequisicao);
    while (sizeRequisicoes == 0) pthread_cond_wait(&newRequisicao, &mutexRequisicao); // Caso não tenha requisicao ele vai ficar esperando até que tenha.
    while (sizeRequisicoes > 0) {
        for (int i = 0; i < N && bufferRequisicoes[out].func != NULL; i++) {
            if (threadOcupada[i] == 0) { // Se a thread atual estiver desocupada, vamos atribuir a requisicao pra ela.
                threadOcupada[i] = 1;
                bufferRequisicoes[out].p.idThread = i; // atualiza o id da thread que vai ficar responsável por aquela execução
                pthread_create(&threads[i], NULL, bufferRequisicoes[out].func, (void *)&bufferRequisicoes[out].p); // cria a thread que vai executar uma das funexec 
            }
            else continue;
            printf("Despachou %d\n", out);
            bufferRequisicoes[out].func = NULL;
            sizeRequisicoes--; 
            out++;

            out = out % B; // Se a última requisicao foi na ultima posicao, agora vamos começar a colocar no inicio do buffer novamente.
            if (sizeRequisicoes == 0) break;
        }

        struct timespec timeToWait;
        struct timeval now;
        gettimeofday(&now,NULL);
        timeToWait.tv_sec = now.tv_sec + 5;
        if (sizeRequisicoes == 0) pthread_cond_timedwait(&newRequisicao, &mutexRequisicao, &timeToWait); // Aqui tem um tempo máximo de espera, pra nao ficar infinitamente esperando uma requisicao quando ja tiver chegado no fim do programa.
    }
    pthread_mutex_unlock(&mutexRequisicao);
    printf("Fim da espera por requisicao.\n");
    pthread_exit(NULL);
}

int main(void) {
    bufferRequisicoes = (Buffer *) calloc (B, sizeof(Buffer));
    bufferResultados = (int *) calloc (B, sizeof(int));

    pthread_t despachant;
    pthread_create(&despachant, NULL, despachante, NULL); // Cria a thread despachante
    

    // Entrada, primeiros vamos agendar todas as requisicoes e depois pegar todos os resultados em ordem
    int ids[10]; 
    for (int i = 0 ; i < 5 ; i++) {
        ParamFunExec p; p.p1 = i + 1;
       ids[i] = agendarExecucao((void *)square, p);
    }
    for (int i = 0 ; i < 5 ; i++) {
        int resultado = pegarResultadoExecucao(ids[i]);
        printf("Resultado de %d: %d\n", ids[i], resultado);
    }

    // Espera o fim de todas as threads
    for (int i = 0 ; i < N ; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(despachant, NULL);
    printf("Todas as threads acabaram.\n");
    
    free(bufferResultados);
    free(bufferRequisicoes);
    pthread_exit(NULL);
    return 0;
}