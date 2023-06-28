#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello!\n");
    MPI_Barrier(MPI_COMM_WORLD);
    printf("We are %d in total!\n", size);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Rank %d reporting for duty!\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);
    char *processor_name;
    int namesize;
    processor_name = (char *)malloc(sizeof(char) * MPI_MAX_PROCESSOR_NAME);
    MPI_Get_processor_name(processor_name, &namesize);
    printf("%s\n", processor_name);
    MPI_Finalize();
}
