#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//create global count variables
int read_count = 0;
int write_count = 0;

//create global conditional variable
pthread_cond_t no_readers;
pthread_cond_t no_writers;

void *reader(void *arg) {
    //acquire mutex. Wait if condition is not satisfied
    pthread_mutex_lock(&mutex);
        while (write_count > 0) {
            pthread_cond_wait(&no_writers, &mutex);
        }
        read_count++;
    pthread_mutex_unlock(&mutex);

    //perform READ
    FILE *fp;
    char buf[255];

    //reacquire mutex. Update conditional variable and broadcast readers and signal writers
    pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            pthread_cond_signal(&no_readers);
        }
    pthread_mutex_unlock(&mutex);
}

void *writer(void *arg) {
    //acquire mutex. Wait if condition is not satisfied.
    pthread_mutex_lock(&mutex);
        while (read_count > 0) {
            pthread_cond_wait(&no_readers, &mutex);
            while (write_count > 0) {
                pthread_cond_wait(&no_writers, &mutex);
            }
        }
        write_count++;
    pthread_mutex_unlock(&mutex);

    //perform WRITE

    //reacquire mutex. Update conditional variable and boadcast readers and signal writers.
    pthread_mutex_lock(&mutex);
        write_count--;
        if  (write_count == 0) {
            pthread_cond_broadcast(&no_writers);
        }
    pthread_mutex_unlock(&mutex);
}

int main() {
    int i;
    pthread_t tid[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++) {
        int threadId = tid[i];
        if (i % 2 == 0) {
            pthread_create(&tid[i], NULL, reader, threadId);
        } else {
            pthread_create(&tid[i], NULL, writer, threadId);
        }

    }
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("Finished running.\n");
    return 0;
}

