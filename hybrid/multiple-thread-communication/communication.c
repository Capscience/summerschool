#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
	int provided, required = MPI_THREAD_MULTIPLE;
	int my_id;
	int threads;
#pragma omp parallel
	{
		 threads = omp_get_num_threads();
	}
	printf("Threads: %d\n", threads);
	MPI_Init_thread(&argc, &argv, required, &provided);
	assert(required == provided);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	int *comms = (int *)malloc(sizeof(int) * threads);
	for (int i = 0; i < threads; i++) {
		MPI_Comm comm;
		MPI_Comm_split(MPI_COMM_WORLD, i, my_id, &comm);
		printf("Comm id: %d\n", comm);
		printf("Rank id: %d\n", my_id);
		comms[i] = (int)comm;
	}
	exit(0);
	MPI_Barrier(MPI_COMM_WORLD);

#pragma omp parallel
	{
		int thread_id = omp_get_thread_num();
		int thread_comm = comms[thread_id];
		MPI_Barrier(MPI_COMM_WORLD);
		int message;
		if (my_id != 0)
			message = -1;
		else
			message = thread_id;

		printf("Comm id: %d\n", thread_comm);


		MPI_Bcast(
			&message,
			1,
			MPI_INT,
			0,
			(MPI_Comm)thread_comm
		);
		printf("Process %d thread id %d\n", my_id, message);
		fflush(stdout);
	}
	free(comms);
	MPI_Finalize();
}
