#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
# include <immintrin.h>
// #include "./SIMDxorshift/include/simdxorshift128plus.h"
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

    // avx_xorshift128plus_key_t mykey;
    // avx_xorshift128plus_init(324, 4444, &mykey);

    // __m256 MAX = _mm256_set1_ps(INT32_MAX);
    // __m256 ONES = _mm256_set1_ps(1.0f);
    __m128i MAX = _mm_set1_epi32(INT32_MAX);
    __m128i ONEI = _mm_set1_epi32(1);
    __m128 ONE = _mm_cvtepi32_ps(ONEI);

    // __m256i randomX, randomY;
    // __m256 floatX, floatY, randX, randY, distanceSquared, mask;
    unsigned int result[4];
    __m128i randomX, randomY;
    __m128 floatX, floatY, randX, randY, distanceSquared, mask;
    int hits;
    float val[4];

    for (long long toss=0; toss < tossNum; toss+=4) {
        // Return a 256-bit random "number"
        // randomX =  avx_xorshift128plus(&mykey);
        // randomY =  avx_xorshift128plus(&mykey);
        // rand_sse(result);
        // _mm_storeu_si128((__m128i*) result, cur_seed);

        // Converts 8 int to 8 float
        // floatX = _mm256_cvtepi32_ps(randomX);
        // floatY = _mm256_cvtepi32_ps(randomY);

        // Get random number [-1, 1]
        // randX = _mm256_div_ps(floatX, MAX);
        // randY = _mm256_div_ps(floatY, MAX);


        randX = _mm_set_ps((float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX);
        randY = _mm_set_ps((float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX, (float)rand_r(&seed)/RAND_MAX);
        distanceSquared = _mm_add_ps(_mm_mul_ps(randX, randX), _mm_mul_ps(randY, randY));
        
        // _mm_store_ps(val, distanceSquared);
        // for (int i = 0; i < 4; i++){
        //     if (val[i] <= 1.f) {
        //         *localCircle += 1;
        //     }
        // }
        mask = _mm_cmple_ps(distanceSquared, ONE);
        hits = _mm_movemask_ps(mask);
        *localCircle += _mm_popcnt_u32(hits);
    }
    // if numberOfTosses is not well devided by thread#
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