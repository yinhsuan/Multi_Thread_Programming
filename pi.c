#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


int main (int argc, char* argv[]) {
    if (argc < 3) {
        printf("Format: ./pi.out <#thread> <#toss>\n");
        exit(1);
    }
    long long int numberInCircle = 0;
    long long int numberOfTosses = atoi(argv[2]);
    int numberOfThread = atoi(argv[1]);

    srand((unsigned)time(NULL));

    for (long long int toss = 0; toss < numberOfTosses; toss++) {
        double x = (double)rand()/RAND_MAX;
        double y = (double)rand()/RAND_MAX;
        double distanceSquared = x * x + y * y;
        if (distanceSquared <= 1) {
            numberInCircle++;
        }
    }
    double piEstimate = 4 * numberInCircle /((double) numberOfTosses);
    printf("%lf\n", piEstimate);

    return 0;
}