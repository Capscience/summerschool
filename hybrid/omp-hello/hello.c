#include <stdio.h>
#include <omp.h>

int main()
{
	printf("Hello world!\n");
	int omp_rank, omp_size;
#pragma omp parallel private(omp_rank, omp_size)
	{
		omp_rank = omp_get_thread_num();
		omp_size = omp_get_num_threads();
		printf("Thread %d out of %d\n", omp_rank, omp_size);
	}
	return 0;
}
