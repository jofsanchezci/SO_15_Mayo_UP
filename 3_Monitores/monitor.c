#include <pthread.h>
#include <stdio.h>

// Monitor
typedef struct {
    int shared_variable;
    pthread_mutex_t mtx;
    pthread_cond_t cv;
    int ready;
} Monitor;

void monitor_init(Monitor *monitor) {
    monitor->shared_variable = 0;
    pthread_mutex_init(&monitor->mtx, NULL);
    pthread_cond_init(&monitor->cv, NULL);
    monitor->ready = 0;
}

void monitor_destroy(Monitor *monitor) {
    pthread_mutex_destroy(&monitor->mtx);
    pthread_cond_destroy(&monitor->cv);
}

void monitor_increment(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mtx);
    while (!monitor->ready) {
        pthread_cond_wait(&monitor->cv, &monitor->mtx);
    }
    for (int i = 0; i < 1000000; ++i) {
        monitor->shared_variable++;
    }
    monitor->ready = 0;
    pthread_cond_signal(&monitor->cv);
    pthread_mutex_unlock(&monitor->mtx);
}

void monitor_set_ready(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mtx);
    monitor->ready = 1;
    pthread_cond_signal(&monitor->cv);
    pthread_mutex_unlock(&monitor->mtx);
}

int monitor_get_shared_variable(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mtx);
    int value = monitor->shared_variable;
    pthread_mutex_unlock(&monitor->mtx);
    return value;
}

void* thread_function(void* arg) {
    Monitor *monitor = (Monitor *)arg;
    monitor_set_ready(monitor);
    monitor_increment(monitor);
    return NULL;
}

int main() {
    Monitor monitor;
    monitor_init(&monitor);

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_function, &monitor);
    pthread_create(&thread2, NULL, thread_function, &monitor);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Valor final: %d\n", monitor_get_shared_variable(&monitor));

    monitor_destroy(&monitor);

    return 0;
}
