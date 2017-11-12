#include <stdio.h>
#include <mpi.h>

int main (argc, argv) 
    int argc; 
    char *argv[];
{
  int rank, size;

    MPI_Init (&argc, &argv);  /* starts MPI */
    // Get the rank and size in the original communicator
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the group of processes in MPI_COMM_WORLD
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    const int server_ranks[1] = {world_size-1};

    // Construct a group containing all of the world_ranks except in server_ranks in world_group
    MPI_Group worker_group;
    MPI_Group_excl(world_group, 1, server_ranks, &worker_group);

    // Create a new communicator based on the group
    MPI_Comm worker_comm;
    MPI_Comm_create(MPI_COMM_WORLD, worker_group, &worker_comm);

    int worker_rank = -1, worker_size = -1;
    // If this rank isn't in the new communicator, it will be
    // MPI_COMM_NULL. Using MPI_COMM_NULL for MPI_Comm_rank or
    // MPI_Comm_size is erroneous
    if (MPI_COMM_NULL != worker_comm) {
        MPI_Comm_rank(worker_comm, &worker_rank);
        MPI_Comm_size(worker_comm, &worker_size);
    }

    printf("WORLD RANK/SIZE: %d/%d \t PRIME RANK/SIZE: %d/%d\n",
        world_rank, world_size, worker_rank, worker_size);
    
    // MPI_Comm_free(&worker_comm);
    MPI_Group_free(&worker_group);
    MPI_Group_free(&world_group);
    
    MPI_Finalize();
    return 0;
}