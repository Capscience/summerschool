#include <cstdio>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int my_id, omp_rank;
    int provided, required=MPI_THREAD_MULTIPLE;

    /* TODO: Initialize MPI with thread support. */
    MPI_Init_thread(&argc, &argv, required, &provided);
    if (required != provided)
        MPI_Abort(MPI_COMM_WORLD, 1);

    /* TODO: Find out the MPI rank and thread ID of each thread and print
     *       out the results. */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        printf("Process %d, thread %d\n", my_id, thread_id);
    }

    /* TODO: Investigate the provided thread support level. */

    MPI_Finalize();
    return 0;
}
