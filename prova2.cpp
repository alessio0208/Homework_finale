#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
        int rank;
        float buf;
        const int root=0;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if(rank == root) {
           buf = 777.2;
        }

        printf("[%d]: Before Bcast, buf is %f\n", rank, buf);

        /* everyone calls bcast, data is taken from root and ends up in everyone's buf */
        MPI_Bcast(&buf, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

        printf("[%d]: After Bcast, buf is %f\n", rank, buf);

        MPI_Finalize();
        return 0;
}