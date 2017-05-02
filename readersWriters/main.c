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

//create global conditional variable
pthread_cond_t read_phase;
pthread_cond_t write_phase;

void *reader(int* id) {
    //usleep(1000 * (random() % 10));
    //acquire mutex. Wait if condition is not satisfied
    pthread_mutex_lock(&mutex);
        while (phase_count < 0) {
            pthread_cond_wait(&read_phase, &mutex);
        }
        printf("\nstarting reader thread %d \n", *id);
        phase_count++;
    pthread_mutex_unlock(&mutex);

    //perform READ
    printf("performing read...\n");
    FILE *fp;
    //fp = fopen(path, "r");
    fp = fopen("<path>", "r");
    char buf[150];
    while(!feof(fp)) {
        fgets(buf, 150, fp);
        printf(buf);
        //fflush(stdout);
    }
    fflush(stdout);
    fclose(fp);
    printf("\nFile closed\n");

    //reacquire mutex. Update conditional variable and broadcast readers and signal writers
    pthread_mutex_lock(&mutex);
        printf("read performed. \n");
        phase_count--;
        if (phase_count == 0) {
            pthread_cond_signal(&write_phase);
        }
    pthread_mutex_unlock(&mutex);
}

void *writer(int *id) {
    //usleep(1000 * (random() % 10));
    //acquire mutex. Wait if condition is not satisfied.
    pthread_mutex_lock(&mutex);
        while (phase_count != 0) {
            //printf("waiting .... %d\n", *id);
            pthread_cond_wait(&write_phase, &mutex);
            //printf("done waiting .... %d\n", *id);
        }
        printf("\nstarting writer thread %d \n", *id);
        phase_count--;
    pthread_mutex_unlock(&mutex);

    //perform WRITE
    printf("performing write...\n");
    FILE *fp;
    //fp = fopen(path, "r");
    fp = fopen("<path>", "a");
    fprintf(fp, "\nwrite from thread %d", *id);
    fflush(fp);
    fclose(fp);
    printf("File closed.\n");
    printf("phase_count %d\n", phase_count);
    //reacquire mutex. Update conditional variable and boadcast readers and signal writers.
    pthread_mutex_lock(&mutex);
        printf("write performed. \n");
        phase_count++;
        printf("setting phase count %d", phase_count);
        if  (phase_count == 0) {
            //pthread_cond_broadcast(&read_phase);
            pthread_cond_signal(&write_phase);
        } else if (phase_count > 0) {
            pthread_cond_broadcast(&read_phase);
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
            printf("\n creating reader thread %d", threadId[i]);
            pthread_create(&tid[i], NULL, reader, &threadId[i]);
        } else {
            printf("\n creating writer thread %d", threadId[i]);
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

