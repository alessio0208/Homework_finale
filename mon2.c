/* MPI program that uses a monte carlo method to compute the value of PI */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#define PI 3.14159265
/* Define the max number of accurate values until termination*/
#define MAX 10

int main(int argc, char** argv)
{   
    float temp_e;
    float e;
    int buff;
    int server_count=0,worker_count;
    float pi=0.0;
    MPI_Status status;
      int found= 0;
            int final_found=0;
            
    int request_tag = 0;
    int chunk_tag = 1;
    int pi_tag = 2;
    int io_rank=0;
    int request = 1;
    // long int n = pow(1.0/(2*e),2); /* how many points per iteration to increase*/
    // long int niter=iterval;
    const int chunk_size = 100000;
    float chunk[chunk_size];
    int best_worker;
    MPI_Request req;

    srand((unsigned int)time(NULL));

    MPI_Init (&argc, &argv);  /* starts MPI */
    // Get the rank and size in the original communicator
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int server_rank = world_size - 1;
    
    const int server_ranks[1] = {server_rank};
    // Get the group of processes in MPI_COMM_WORLD
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    // Construct a group containing all of the world_ranks except in server_ranks in world_group
    MPI_Group worker_group;
    MPI_Group_excl(world_group, 1, server_ranks, &worker_group);

    // Create a new communicator based on the group
    MPI_Comm worker_comm;
    MPI_Comm_create(MPI_COMM_WORLD, worker_group, &worker_comm);

    int worker_rank = -1, worker_size = -1;
    if (MPI_COMM_NULL != worker_comm) {
        MPI_Comm_rank(worker_comm, &worker_rank);
        MPI_Comm_size(worker_comm, &worker_size);
    }

    // MPI_Barrier(MPI_COMM_WORLD);

    // printf("WORLD RANK/SIZE: %d/%d \t PRIME RANK/SIZE: %d/%d\n",
    //     world_rank, world_size, worker_rank, worker_size);
    
    // srand48(time(NULL)*server_rank);
    //Test
    // printf("server_rank = %d", server_rank);

    if (world_rank == 0){
        sscanf(argv[1],"%f",&e);
        // printf("e = %f \n",e);        
        // printf("Worker %d send e= %f from all workers\n",world_rank,e);
    }
    // printf("[%d]: Before Bcast, e is %f\n", world_rank, e);
    MPI_Bcast(&e,1,MPI_REAL,0,MPI_COMM_WORLD);
    // printf("[%d]: After Bcast, e is %f\n", world_rank, e);
    // MPI_Barrier(MPI_COMM_WORLD);

        if (world_rank == server_rank) {
           
           // MPI_Bcast(&request,1,MPI_INT,server_rank,MPI_COMM_WORLD);
          
            // printf("At the server, worker_size %d\n",worker_size);
            for (int worker=0; worker < world_size-1; worker++) {
                // printf("For Worker=%d",worker);
                MPI_Recv(&request,1,MPI_INT,worker,request_tag,MPI_COMM_WORLD,&status);
                printf("Server %d receive request = %d from worker %d\n",world_rank,request,worker);
                if(request){
                    // printf("Server %d receive request = %d from worker %d\n",world_rank,request,worker);
                
                    for (int i=0; i<chunk_size; i++){
                        chunk[i] = ((float)rand())/RAND_MAX;
                        // chunk[i] = (float)drand48();
                        // printf("Server %d generate random_value= %f\n",world_rank,chunk[i]);
                    }
                    MPI_Send(&chunk, chunk_size, MPI_REAL,worker,chunk_tag,MPI_COMM_WORLD);
                    //printf("Server %d send array %f to worker %d\n",world_rank,chunk[2],worker);
                    // count_send += chunk_size;
                    MPI_Irecv(&request, 1, MPI_INT, server_rank, request_tag,
              MPI_COMM_WORLD, &req);
                 if(!request)
                    break;
                }

               
               
               
            }
        }







        else {
             
            
            while(!final_found){

            if(request){

        
    
            MPI_Send(&request, 1, MPI_INT,server_rank,request_tag,MPI_COMM_WORLD); 
            int inside_count = 0;
            
          
            //printf("Worker %d send request= %d to server %d\n",world_rank,request,server_rank);
            
            MPI_Recv(&chunk,chunk_size,MPI_REAL,server_rank,chunk_tag,MPI_COMM_WORLD,&status);
            //printf("Woker %d receive chunk= %f from server %d\n",world_rank,chunk[2],server_rank);
            for (int i=0; i<chunk_size; i+=2) {
                float x = chunk[i];
                float y = chunk[i+1];
                float z = x*x + y*y;
                if (z<=1) inside_count++; 
            } /* for all the slave processes send results to the master */

            pi=(float)(4*inside_count)/(chunk_size/2); //4 quadrants of circle
            temp_e = fabs((pi-PI)/PI);
           // printf("PI = %f and Temp_Error = %f and Error = %f\n",pi,temp_e,e);
            
            if(temp_e<= e) {
                printf("Worker %d satisfies!\n",world_rank);
                request = 0;
                // MPI_Send(&pi, 1, MPI_REAL,0,pi_tag,MPI_COMM_WORLD);
                // MPI_Bcast(&pi,1,MPI_REAL,worker_rank,worker_comm);
                printf("Satisfy: Worker %d send request= %d to server %d\n",world_rank,request,server_rank);
                found=1;
                final_found=1;
               }
                      MPI_Allreduce(&found, &final_found, 1, MPI_INT, MPI_SUM, worker_comm); 
                      MPI_Isend(&request, 1, MPI_INT, server_rank, request_tag,
              MPI_COMM_WORLD, &req);

             }
                               
                      
                                                      
            // MPI_Reduce(&worker_count,&server_count , 1 , MPI_INT, MPI_SUM, server_rank , MPI_COMM_WORLD);
            // printf("Server sum server_count= %d from all workers\n",server_count);
            // MPI_Send(&worker_count,1,MPI_REAL,server_rank,count_tag,MPI_COMM_WORLD);
              }
            }
        /* Have we received a done command */
     printf("world %d final found %d\n", world_rank,final_found);
    
  

    //MPI_Bcast(&pi,1,MPI_REAL,best_worker,MPI_COMM_WORLD);         
             

    if(world_rank==server_rank){
       
      printf("SERVER %d \n", world_rank);
    }
    
    if(temp_e<= e){
       //MPI_Send(&request, 1, MPI_INT,server_rank,request_tag,MPI_COMM_WORLD); 
       MPI_Send(&pi, 1, MPI_FLOAT, io_rank, pi_tag,  MPI_COMM_WORLD);          
    } 

    
    if (world_rank == io_rank){
        printf("PI = %f\n",pi);
        MPI_Recv(&pi, 1, MPI_FLOAT, MPI_ANY_SOURCE, pi_tag,  MPI_COMM_WORLD,&status);
      
        printf("\n\nOUTPUT:\n");
        printf("PI = %f\n",pi);   
    }   
    
   // MPI_Group_free(&worker_group);
    //MPI_Group_free(&world_group);
    MPI_Finalize();             /* let MPI finish up */
  
return 0;
}