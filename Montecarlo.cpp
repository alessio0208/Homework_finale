
 #include <iostream>
 #include <fstream>

 #include<cmath>
 #include <boost/random/linear_congruential.hpp>
 #include <boost/random/uniform_int.hpp>
 #include <boost/random/uniform_real.hpp>
 #include <boost/random/variate_generator.hpp>
 #include <boost/generator_iterator.hpp>
 #include <ctime>
 // This is a typedef for a random number generator.
 // Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
 typedef boost::minstd_rand base_generator_type;
 
 // This is a reproducible simulation experiment.  See main().
 double Montecarlo(double e){
   int n=pow(1.0/(2*e),2);

   base_generator_type generator(std::time(0));
   int inside=0;
   int outside=0;

   
     boost::uniform_real<> uni_dist(0,1);
     boost::variate_generator<base_generator_type&, boost::uniform_real<> > X(generator, uni_dist);
     boost::variate_generator<base_generator_type&, boost::uniform_real<> > Y(generator, uni_dist);
    
     std::cout.setf(std::ios::fixed);
   
     for(int i = 0; i < n; i++){
         double x=X();
         double y=Y();
         if(x*x+y*y<=1)
           inside++;
        
     }
    
    double pi= double(4*inside)/n;
    return pi;
 }
 
 int main()
 {
   double pi=Montecarlo(0.001);
   std::cout<<"PI "<< pi;
   
   return 0;
 }