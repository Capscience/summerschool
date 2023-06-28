#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello!\n");
    printf("We are %d in total!\n", size);
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("We are rank %d\n", rank);
    MPI_Finalize();
}
