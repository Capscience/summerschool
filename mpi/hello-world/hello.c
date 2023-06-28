#include <stdio.h>
<<<<<<< HEAD
=======
#include <stdlib.h>
>>>>>>> a6a2235a8ef168328a15417619e2d86fee9a03db
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
<<<<<<< HEAD

    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello!\n");
    printf("We are %d in total!\n", size);
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("We are rank %d\n", rank);
=======
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
>>>>>>> a6a2235a8ef168328a15417619e2d86fee9a03db
    MPI_Finalize();
}
