#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int shared_variable = 0; // Variable compartida
sem_t semaphore; // Declaración del semáforo

void* increment(void* arg) {
    for (int i = 0; i < 1000000; ++i) {
        sem_wait(&semaphore); // Espera (decrementa) el semáforo
        shared_variable++; // Sección crítica
        sem_post(&semaphore); // Señaliza (incrementa) el semáforo
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    sem_init(&semaphore, 0, 1); // Inicializa el semáforo con valor 1

    pthread_create(&thread1, NULL, increment, NULL); // Crea el primer hilo
    pthread_create(&thread2, NULL, increment, NULL); // Crea el segundo hilo

    pthread_join(thread1, NULL); // Espera a que el primer hilo termine
    pthread_join(thread2, NULL); // Espera a que el segundo hilo termine

    sem_destroy(&semaphore); // Destruye el semáforo

    printf("Valor final: %d\n", shared_variable); // Imprime el valor final

    return 0;
}
