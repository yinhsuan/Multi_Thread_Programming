#include <iostream>
#include <time.h>
using namespace std;

int main () {
    int number_in_circle = 0;
    int number_of_tosses = 1000;
    srand((unsigned)time(NULL));

    for ( int toss = 0; toss < number_of_tosses; toss ++) {
        float x = (float)rand()/RAND_MAX;
        float y = (float)rand()/RAND_MAX;
        float distance_squared = x * x + y * y;
        if (distance_squared <= 1)
            number_in_circle++;
    }
    float pi_estimate = 4 * number_in_circle /((float) number_of_tosses);
    cout << pi_estimate << endl;
 
    return 0;
}