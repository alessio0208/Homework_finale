#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

double f(double x){
    double t = 4.0/(1+x*x);

    return t;
}


double PI(int n, int p, int id){
  

    double pi;


  
    MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    double local_sum = 0;

    for (int i=id; i<n; i+=p){
        local_sum+= f(1.0*i/n) + f(1.0*(i+1)/n);
    }

    local_sum = local_sum * 1.0/(2*n);


    MPI_Reduce(&local_sum , &pi , 1 , MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD); 


    return pi;
}



int main(int argc, char*argv[])
{   
    int p, id;
    double start, end;
    int n = 100000000;
    double local_time;
    double total_time;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

  
    double pi= PI(n,p,id);
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    local_time=end-start;
    printf("Time = %f \n",local_time);
    
    
    if(id==0) 
    printf("PI = %f \n",pi);

    
    MPI_Finalize ();
    return 0;
}
