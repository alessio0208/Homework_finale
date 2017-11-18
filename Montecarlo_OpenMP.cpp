#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

float get_unit_random_number()
{
   int random_value = rand(); 
   float unit_random = random_value / (float) RAND_MAX; 
   return unit_random;
  }
  


int samplesInsideCircle(const int numSamples)
{
   
    int counter = 0;
    for (int s = 0; s != numSamples; s++)
    {
      
         float x = get_unit_random_number();
         float y = get_unit_random_number();
        
        if (x * x + y * y < 1)
        {
            counter++;
        }
    }
    
    return counter;
}



void parallelPi()
{
    
    const int numTotalSamples = 10000000; 
    int numChunks = 8;
    int chunk = numTotalSamples / numChunks;
    
    int counter = 0;
    
#pragma omp parallel for shared(numChunks, chunk) reduction(+:counter)
    for (int i = 0; i < numChunks; i++)
    {
        counter += samplesInsideCircle(chunk); 
    }

    const double approxPi = 4.0 * counter / numTotalSamples;
    
    std::cout << "Parallel version: " << "\n";
    std::cout << "number of samples: " << numTotalSamples << "\n";
    std::cout << "real Pi: 3.141592653589..." << "\n";
    std::cout << "ApproxPi "<<approxPi<<std::endl;
}

int main()
{
    
    parallelPi();


    return 0;
}