#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//create global count variables
int read_count = 0;
int write_count = 0;

//create global conditional variable
pthread_cond_t no_readers;
pthread_cond_t no_writers;

void *reader(int* id) {
    //usleep(1000 * (random() % 10));
    //acquire mutex. Wait if condition is not satisfied
    pthread_mutex_lock(&mutex);
        while (write_count > 0) {
            pthread_cond_wait(&no_writers, &mutex);
        }
        printf("\nstarting reader thread %d \n", *id);
        read_count++;
    pthread_mutex_unlock(&mutex);

    //perform READ
    printf("performing read...\n");
    FILE *fp;
    //fp = fopen(path, "r");
    fp = fopen("<filepath>", "r");
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
        read_count--;
        if (read_count == 0) {
            pthread_cond_signal(&no_readers);
        }
    pthread_mutex_unlock(&mutex);
}

void *writer(int *id) {
    //usleep(1000 * (random() % 10));
    //acquire mutex. Wait if condition is not satisfied.
    pthread_mutex_lock(&mutex);
        while (read_count > 0) {
            pthread_cond_wait(&no_readers, &mutex);
            while (write_count > 0) {
                pthread_cond_wait(&no_writers, &mutex);
            }
        }
        printf("\nstarting writer thread %d \n", *id);
        write_count++;
    pthread_mutex_unlock(&mutex);

    //perform WRITE
    printf("performing write...\n");
    FILE *fp;
    //fp = fopen(path, "r");
    fp = fopen("<filepath>", "a");
    fprintf(fp, "\nwrite from thread %d", *id);
    fflush(fp);
    fclose(fp);
    printf("File closed.\n");

    //reacquire mutex. Update conditional variable and boadcast readers and signal writers.
    pthread_mutex_lock(&mutex);
        printf("write performed. \n");
        write_count--;
        if  (write_count == 0) {
            pthread_cond_broadcast(&no_writers);
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

