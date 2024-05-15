#include <pthread.h>
#include <stdio.h>

int shared_variable = 0; // Variable compartida
pthread_mutex_t mutex; // Declaración del mutex

void* increment(void* arg) {
    for (int i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&mutex); // Bloquea el mutex
        shared_variable++; // Sección crítica
        pthread_mutex_unlock(&mutex); // Desbloquea el mutex
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL); // Inicializa el mutex

    pthread_create(&thread1, NULL, increment, NULL); // Crea el primer hilo
    pthread_create(&thread2, NULL, increment, NULL); // Crea el segundo hilo

    pthread_join(thread1, NULL); // Espera a que el primer hilo termine
    pthread_join(thread2, NULL); // Espera a que el segundo hilo termine

    pthread_mutex_destroy(&mutex); // Destruye el mutex

    printf("Valor final: %d\n", shared_variable); // Imprime el valor final

    return 0;
}
