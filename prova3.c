#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv)
{
  MPI_Init (&argc, &argv);     
int world_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
int world_size;
MPI_Comm_size(MPI_COMM_WORLD, &world_size);
int e=0;
 MPI_Status status;
int number;
if (world_rank == 0) {
    e=2;
   
} 

if(e>1){
 number = -1;
    MPI_Send(&number, 1, MPI_INT, 1,0, MPI_COMM_WORLD);
}

if (world_rank == 1) {
    MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE,0, MPI_COMM_WORLD,
             &status);
    printf("Process 1 received number %d from process 0\n",
           number);
}

 MPI_Finalize();             /* let MPI finish up */
}
