#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//create global count variables
//int read_count = 0;
//int write_count = 0;
int phase_count = 0;
int readers_waiting = NUM_THREADS/2;

//create global conditional variable
pthread_cond_t read_phase = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_phase = PTHREAD_COND_INITIALIZER;

void *reader(int* id) {
    usleep(1000 * (random() % 10));
    //acquire mutex. Wait if condition is not satisfied
    pthread_mutex_lock(&mutex);
        while (phase_count < 0) {
            pthread_cond_wait(&read_phase, &mutex);
        }
        printf("\nStarting reader thread %d\n", *id);
        phase_count++;
    pthread_mutex_unlock(&mutex);

    //perform READ
    printf("Performing read by thread %d\n", *id);
    FILE *fp;
    printf("File opened by thread %d\n", *id);
    fp = fopen("file.txt", "r");
    char buf[150];
    while(!feof(fp)) {
        fgets(buf, 150, fp);
        printf(buf);
        //fflush(stdout);
    }
    fflush(stdout);
    fclose(fp);
    printf("\nFile closed by thread %d\n", *id);

    //reacquire mutex. Update conditional variable and broadcast readers and signal writers
    pthread_mutex_lock(&mutex);
        printf("Read performed by thread %d\n", *id);
        phase_count--;
        readers_waiting--;
        if (phase_count == 0 && readers_waiting == 0) {
            pthread_cond_signal(&write_phase);
        }
    pthread_mutex_unlock(&mutex);
}

void *writer(int *id) {
    usleep(1000 * (random() % 10));
    //acquire mutex. Wait if condition is not satisfied.
    pthread_mutex_lock(&mutex);
        while (phase_count != 0) {
            pthread_cond_wait(&write_phase, &mutex);
        }
        printf("\nStarting writer thread %d \n", *id);
        phase_count--;
    pthread_mutex_unlock(&mutex);

    //perform WRITE
    printf("Performing write by thread %d\n", *id);
    FILE *fp;
    fp = fopen("file.txt", "a");
    printf("File opened by thread %d.\n", *id);
    fprintf(fp, "\nWrite from thread %d", *id);
    fflush(fp);
    fclose(fp);
    printf("\nFile closed by thread %d.\n", *id);

    //reacquire mutex. Update conditional variable and boadcast readers and signal writers.
    pthread_mutex_lock(&mutex);
        printf("Write performed by thread %d.\n", *id);
        phase_count++;
        if (readers_waiting > 0 ) {
            pthread_cond_broadcast(&read_phase);
        } else {
            if (phase_count == 0) {
                pthread_cond_signal(&write_phase);
            }
        }
    pthread_mutex_unlock(&mutex);
}

int main() {

    int i;
    pthread_t tid[NUM_THREADS];
    int threadId[NUM_THREADS];
    srandom((unsigned int)time(NULL));

    for (i = 0; i < NUM_THREADS; i++) {
        threadId[i] = i;

        if (i < 5) {
            printf("\nCreating reader thread %d", threadId[i]);
            pthread_create(&tid[i], NULL, reader, &threadId[i]);
        } else {
            printf("\nCreating writer thread %d", threadId[i]);
            pthread_create(&tid[i], NULL, writer, &threadId[i]);
        }
    }


     /*
    int n = 1;
    pthread_create(&tid[i], NULL, reader, &n);
    */

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("\nFinished running.\n");
    return 0;
}

