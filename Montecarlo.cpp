#include <boost/random.hpp>
#include <iostream>
#include <ctime>
#include <cstdint>
#include <boost/random/random_device.hpp>


int main(int argc, char * argv[]){
   

    boost::mt19937 rng; // I don't seed it on purpouse (it's not relevant)
    
      boost::normal_distribution<> nd(0.0, 1.0);
    
      boost::variate_generator<boost::mt19937&, 
                               boost::normal_distribution<> > var_nor(rng, nd);
return 0;

}