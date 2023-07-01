#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
        int provided, required = MPI_THREAD_MULTIPLE;
        int my_id, thread_id;
        MPI_Init_thread(&argc, &argv, required, &provided);
        assert(required == provided);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
        int recvbuf;

#pragma omp parallel private(thread_id)
        {
                thread_id = omp_get_thread_num();
                MPI_Comm thread_comm;
                MPI_Comm_split(MPI_COMM_WORLD, thread_id, my_id, &thread_comm);
                if (my_id != 0)
                        thread_id = -1;
                MPI_Bcast(
                        &thread_id,
                        1,
                        MPI_INT,
                        0,
                        thread_comm
                );
                printf("Process %d thread id %d\n", my_id, thread_id);
        }
        MPI_Finalize();
}
