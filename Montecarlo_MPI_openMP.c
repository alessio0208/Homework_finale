#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <omp.h>
#define PI 3.14159265



int main(int argc, char** argv)
{
    float e;


    MPI_Status status;

    int request_tag = 0;
    int chunk_tag = 1;
    int pi_tag = 2;
    int total_done =0;
    const int chunk_size = 100000;
    int chunk[chunk_size];
    double pi=0;
    omp_set_num_threads(4);
   
    MPI_Init (&argc, &argv); 
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int server_rank = world_size - 1;
    
    const int server_ranks[1] = {server_rank};
   
   #pragma omp parallel
   {
    
    printf("threads=%d ",omp_get_num_threads());
   }

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Group worker_group;
    MPI_Group_excl(world_group, 1, server_ranks, &worker_group);

    MPI_Comm worker_comm;
    MPI_Comm_create(MPI_COMM_WORLD, worker_group, &worker_comm);

    int worker_rank = -1, worker_size = -1;
    if (MPI_COMM_NULL != worker_comm) {
        MPI_Comm_rank(worker_comm, &worker_rank);
        MPI_Comm_size(worker_comm, &worker_size);
    }

  

    if (world_rank == 0){
        sscanf(argv[1],"%f",&e);

    }
  
    MPI_Bcast(&e,1,MPI_REAL,0,MPI_COMM_WORLD);


    int done=0;

    if (world_rank == server_rank) {
        while(done !=(world_size-1)){
         
            int request;
            int flag = 0;


            while(!flag){
           
                MPI_Iprobe(MPI_ANY_SOURCE, request_tag, MPI_COMM_WORLD, &flag, &status );
                
            }

            MPI_Recv(&request,1,MPI_INT,MPI_ANY_SOURCE,request_tag,MPI_COMM_WORLD,&status);
            
            if(request){
                #pragma omp parallel for
                for (int i=0; i<chunk_size; i++){
                    chunk[i] = rand();
                }
                MPI_Send(&chunk, chunk_size, MPI_REAL,status.MPI_SOURCE,chunk_tag,MPI_COMM_WORLD);
            }
            else{
                done++;
         
            }
        }
      
    }


    else {
        while(!total_done){
            int inside_count = 0;
            int request = 1;
            MPI_Send(&request, 1, MPI_INT,server_rank,request_tag,MPI_COMM_WORLD);

            MPI_Recv(&chunk,chunk_size,MPI_REAL,server_rank,chunk_tag,MPI_COMM_WORLD,&status);
            
            #pragma omp parallel for reduction(+:inside_count)
            for (int i=0; i<chunk_size; i+=2) {
                float x = (float)chunk[i]/RAND_MAX;
                float y = (float)chunk[i+1]/RAND_MAX;
                float z = x*x + y*y;
                if (z<=1) inside_count++;
              
            }


            pi=(float)(4*inside_count)/(chunk_size/2); 
            float temp_e = fabs((pi-PI)/PI);
            
            if(temp_e<= e) {
                MPI_Send(&pi, 1, MPI_REAL, 0, pi_tag, worker_comm);
               
                done = 1;  
            }
            MPI_Allreduce(&done, &total_done, 1, MPI_INT, MPI_SUM, worker_comm);
            
            if (total_done){
                int request = 0;
                MPI_Send(&request, 1, MPI_INT,server_rank,request_tag,MPI_COMM_WORLD);
                
            }
        }
    }
    
    if (world_rank == 0){
        int flag = 0;
        while(!flag){
            
            MPI_Iprobe(MPI_ANY_SOURCE, pi_tag, worker_comm, &flag, &status );
        }

        MPI_Recv(&pi,1,MPI_REAL,status.MPI_SOURCE,pi_tag,worker_comm,&status);
        printf("OUTPUT:\n");
        printf("PI = %f\n",pi);
    }  
    
    MPI_Group_free(&worker_group);
    MPI_Group_free(&world_group);
    
    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();
    double local_time=end-start;
    printf("Time = %f \n",local_time);
    MPI_Finalize();
  
return 0;
}
