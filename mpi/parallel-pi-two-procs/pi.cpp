#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 840000;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Computing approximation to pi with N=%d\n", n);

    int istart = 1 + rank * n / size;
    int istop = (rank + 1) * n / size;

    printf("Rank: %i\n", rank);
    printf("Start: %i\n", istart);
    printf("Stop: %i\n", istop);

    double pi = 0.0;
    for (int i=istart; i <= istop; i++) {
        double x = (i - 0.5) / n;
        pi += 1.0 / (1.0 + x*x);
    }

    double receive_buffer;

    MPI_Reduce(
        &pi,
        &receive_buffer,
        1,
        MPI_DOUBLE,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {
        receive_buffer *= 4.0 / n;
        printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", receive_buffer, M_PI);
    }


    MPI_Finalize();
}
