#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 30
#define LIMITE 100

// Estrutura expandida para suportar nomes
typedef struct {
    int id;
    char nome[30];
} thread_data;

pthread_barrier_t barrier;
char nome_vencedor[30];
int venceu = 0;
pthread_mutex_t mutex_vencedor = PTHREAD_MUTEX_INITIALIZER;

void* corrida(void* arg) {
    thread_data* data = (thread_data*)arg;
    
    pthread_barrier_wait(&barrier);

    // Loop da corrida
    for (int i = 0; i < LIMITE; i++);

    pthread_mutex_lock(&mutex_vencedor);
    if (!venceu) {
        strcpy(nome_vencedor, data->nome);
        venceu = 1;
    }
    pthread_mutex_unlock(&mutex_vencedor);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data t_info[NUM_THREADS];
    
    //Umas participantes
    char *nomes[] = {
        "Special Week", "Tokai Teio", "Fuji Kiseki", "Oguri Cap", "Gold Ship", 
        "Vodka", "Daiwa Scarlet", "Hishi Amazon", "El Condor Pasa", "TM Opera O",
        "Narita Brian", "Symboli Rudolf", "Air Groove", "Biwa  Hayahide", "Sakura Bakushin O",
        "Haru Urara", "Kitasan Black", "Twin Turbo", "Sirius Symboli", "Super Creek",
        "Air Shakur", "Seeking the Pearl", "Agnes Tachyon","Rice Shower", "Manhattan Cafe",
        "Mayano Top Gun", "Mihono Bourbon", "Mejiro Ryan", "Sweep Tosho", "Nakayama Festa"
    };
    printf("Iniciando Arima Kinen com %d competidores!\n", NUM_THREADS);

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        t_info[i].id = i + 1;
        strcpy(t_info[i].nome, nomes[i]);
        
        pthread_create(&threads[i], NULL, corrida, &t_info[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n--- RESULTADO FINAL ---\n");
    printf("O vencedor foi: %s!\n", nome_vencedor);

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex_vencedor);

    return 0;
}