#include <iostream>
#include <time.h>
using namespace std;

// you need to implement the Monte Carlo method using Pthreads
int main (int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Format: ./pi.out <#thread> <#toss>" << endl;
        exit(1);
    }
    long long int numberInCircle = 0;
    long long int numberOfTosses = atoi(argv[2]);
    int numberOfThread = atoi(argv[1]);

    srand((unsigned)time(NULL));

    for ( int toss = 0; toss < numberOfTosses; toss++) {
        double x = (double)rand()/RAND_MAX;
        double y = (double)rand()/RAND_MAX;
        double distanceSquared = x * x + y * y;
        if (distanceSquared <= 1)
            numberInCircle++;
    }
    double piEstimate = 4 * numberInCircle /((double) numberOfTosses);
    cout << piEstimate << endl;
 
    return 0;
}