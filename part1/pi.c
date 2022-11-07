#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <immintrin.h>
// REFERENCE: https://github.com/f4exb/cm256cc/blob/master/sse2neon.h

int threadCount = 0;
long long numberOfTosses = 0;
long long numberInCircle = 0;

void* calculate (void* threadId) {
    long long *localCircle = (long long *)malloc(sizeof(long long));
    *localCircle = 0;
    long long tossNum = numberOfTosses / threadCount;
    long long remainNum = numberOfTosses % threadCount;
    long longTid = (long)threadId;
    int tid = (int)longTid;
    unsigned int seed = (long)threadId + 1;

    __m128 ONE = _mm_set_ps1(1.f);
    __m128 randX, randY, distanceSquared, mask;
    unsigned int hits;

    for (long long toss=0; toss < tossNum; toss+=4) {
        randX = _mm_set_ps((float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX);
        randY = _mm_set_ps((float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX);

        distanceSquared = _mm_add_ps(_mm_mul_ps(randX, randX), _mm_mul_ps(randY, randY));
        mask = _mm_cmple_ps(distanceSquared, ONE);
        hits = _mm_movemask_ps(mask);
        *localCircle += _mm_popcnt_u32(hits);
    }

    if (tid == 0) {
        for (long long toss=0; toss<remainNum; toss+=4) {
            randX = _mm_set_ps((float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX);
            randY = _mm_set_ps((float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX);
            distanceSquared = _mm_add_ps(_mm_mul_ps(randX, randX), _mm_mul_ps(randY, randY));

            mask = _mm_cmple_ps(distanceSquared, ONE);
            hits = _mm_movemask_ps(mask);
            *localCircle += _mm_popcnt_u32(hits);
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
        pthread_create(&threads[thread], NULL, calculate, (void*)thread);
    }
    for (long thread=0; thread<threadCount; thread++) {
        pthread_join(threads[thread], &status);
        numberInCircle += *(long long*)status;
    }
    double piEstimate = 4 * numberInCircle / ((double)numberOfTosses);
    printf("%lf\n", piEstimate);

    pthread_exit(NULL);
    return 0;
}