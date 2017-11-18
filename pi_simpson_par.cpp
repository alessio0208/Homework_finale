#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>
#include <boost/mpi/timer.hpp>
#include <boost/mpi/collectives.hpp>
#include <stdlib.h>

namespace mpi = boost::mpi;

double f(double x){
    double t = 4.0/(1+x*x);
    return t;
}


double PI(int m,mpi::communicator world){
    double sum = f(0.0)+ f(1.0);
    int n=2*m;
    double global_sum;
    broadcast(world, m, 0);

    for (int i=world.rank(); i<m; i+=world.size()){
        sum+= 2*f(1.0*i/m);
    }

    for (int i=world.rank(); i<m+1; i+=world.size()){
        sum+= 4*f(1.0*(2*i-1)/(2*m));
    }
    
    mpi::reduce(world, sum, global_sum, std::plus<double>(),0);

    double pi = 1.0/(3*n)*global_sum;
    
    return pi;
}


int main(int argc, char* argv[]) 
{
  
  mpi::environment env(argc, argv);
  mpi::communicator world;
  world.barrier();
  mpi::timer timer;
  double start;


  double pi=PI(100000,world);
  world.barrier();
  double end= timer.elapsed();
  
  if(world.rank()==0)
  std::cout << "PI "<< pi<< std::endl;

  std::cout<< "end " << end << " rank " << world.rank()<< std::endl;


  return 0;
}

