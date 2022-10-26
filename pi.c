#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int threadCount = 0;
long long numberOfTosses = 0;
long long numberInCircle = 0;
pthread_mutex_t mutex;

void* calculate (void* rank) {
    long long count = numberOfTosses / threadCount;
    for (long long toss=0; toss < count; toss++) {
        double x = drand48();
        double y = drand48();
        // printf("%lf, %lf\n", x, y);
        double distanceSquared = x * x + y * y;
        if (distanceSquared <= 1) {
            pthread_mutex_lock(&mutex);
            numberInCircle++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main (int argc, char* argv[]) {
    if (argc < 3) {
        printf("Format: ./pi.out <#thread> <#toss>\n");
        exit(1);
    }
    pthread_t* threadHandles;
    threadCount = atoi(argv[1]);
    numberOfTosses = strtoll(argv[2], NULL, 10);
    threadHandles = (pthread_t*) malloc(threadCount*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    for (long thread=0; thread<threadCount; thread++) {
        pthread_create(&threadHandles[thread], NULL, calculate, (void*) thread);
    }
    for (long thread=0; thread<threadCount; thread++) {
        pthread_join(threadHandles[thread], NULL);
    }
    pthread_mutex_destroy(&mutex);
    free(threadHandles);

    double piEstimate = 4 * numberInCircle / ((double) numberOfTosses);
    printf("%lf\n", piEstimate);

    return 0;
}