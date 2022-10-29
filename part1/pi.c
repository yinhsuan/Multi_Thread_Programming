#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int threadCount = 0;
long long numberOfTosses = 0;
long long numberInCircle = 0;

void* calculate (void* threadId) {
    long long *localCircle = (long long *)malloc(sizeof(long long));
    long long count = numberOfTosses / threadCount;
    unsigned int seed = (long)threadId + 1;

    for (long long toss=0; toss < count; toss++) {
        double x = (double) rand_r(&seed)/RAND_MAX;
        double y = (double) rand_r(&seed)/RAND_MAX;
        double distanceSquared = x * x + y * y;
        if (distanceSquared <= 1) {
            *localCircle+=1;
        }
    }
    pthread_exit((void *)localCircle);
}

int main (int argc, char* argv[]) {
    if (argc < 3) {
        printf("Format: ./pi.out <#thread> <#toss>\n");
        exit(1);
    }
    void *status;
    threadCount = atoi(argv[1]);
    numberOfTosses = strtoll(argv[2], NULL, 10);
    pthread_t threads[threadCount];

    for (long thread=0; thread<threadCount; thread++) {
        pthread_create(&threads[thread], NULL, calculate, (void*) thread);
    }
    for (long thread=0; thread<threadCount; thread++) {
        pthread_join(threads[thread], &status);
        numberInCircle += *(long long*)status;
    }
    double piEstimate = 4 * numberInCircle / ((double) numberOfTosses);
    printf("%lf\n", piEstimate);

    pthread_exit(NULL);
    return 0;
}