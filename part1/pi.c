#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
# include <nmmintrin.h>
# include <atomic>
#include "./SIMDxorshift/include/simdxorshift128plus.h"

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

    avx_xorshift128plus_key_t mykey;
    avx_xorshift128plus_init(324, 4444, &mykey);

    __m256 MAX = _mm256_set1_ps(INT32_MAX);
    __m256 ONES = _mm256_set1_ps(1.0f);

    __m256i randomX, randomY;
    __m256 floatX, floatY, x, y, distanceSquared, mask;
    unsigned int hits;

    for (long long toss=0; toss < tossNum; toss+=8) {
        randomX =  avx_xorshift128plus(&mykey);
        randomY =  avx_xorshift128plus(&mykey);

        floatX = _mm256_cvtepi32_ps(randomX);
        floatY = _mm256_cvtepi32_ps(randomY);

        x = _mm256_div_ps(floatX, MAX);
        y = _mm256_div_ps(floatY, MAX);

        distanceSquared = _mm256_add_ps(_mm256_mul_ps(x, x), _mm256_mul_ps(y, y));
        mask = _mm256_cmp_ps(distanceSquared, ONES, _CMP_LE_OQ);
        hits = _mm256_movemask_ps(mask);
        *localCircle += _mm_popcnt_u32(hits);
    }
    // if numberOfTosses is not well devided by thread#
    if (tid == 0) {
        for (long long toss=0; toss<remainNum; toss++) {
           randomX =  avx_xorshift128plus(&mykey);
            randomY =  avx_xorshift128plus(&mykey);

            floatX = _mm256_cvtepi32_ps(randomX);
            floatY = _mm256_cvtepi32_ps(randomY);

            x = _mm256_div_ps(floatX, MAX);
            y = _mm256_div_ps(floatY, MAX);

            distanceSquared = _mm256_add_ps(_mm256_mul_ps(x, x), _mm256_mul_ps(y, y));
            mask = _mm256_cmp_ps(distanceSquared, ONES, _CMP_LE_OQ);
            hits = _mm256_movemask_ps(mask);
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