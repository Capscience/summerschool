#include <hip/hip_runtime.h>
#include <stdio.h>
#include <math.h>

// TODO: add a device kernel that copies all elements of a vector
//       using GPU threads in a 2D grid
__global__ void dcopy_(double *x, double *y, int size)
{
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	int stride = gridDim.x * blockDim.x;

	for (; tid < size; tid += stride)
		y[tid] = x[tid];
}

int main(void)
{
    int i, j;
    const int n = 600;
    const int m = 400;
    const int size = n * m;
    double x[size], y[size], y_ref[size];
    double *x_, *y_;

    // initialise data
    for (i=0; i < size; i++) {
        x[i] = (double) i / 1000.0;
        y[i] = 0.0;
    }
    // copy reference values (C ordered)
    for (i=0; i < n; i++) {
        for (j=0; j < m; j++) {
            y_ref[i * m + j] = x[i * m + j];
        }
    }

    // TODO: allocate vectors x_ and y_ on the GPU
    hipMalloc(&x_, sizeof(double) * size);
    hipMalloc(&y_, sizeof(double) * size);
    // TODO: copy initial values from CPU to GPU (x -> x_ and y -> y_)
    hipMemcpyHtoD(x_, x, sizeof(double) * size);
    hipMemcpyHtoD(y_, y, sizeof(double) * size);

    // TODO: define grid dimensions (use 2D grid!)
    dim3 blocks(4, 6);
    dim3 threads(16, 16);

    // TODO: launch the device kernel
	dcopy_<<<blocks, threads>>>(x_, y_, size);

    // TODO: copy results back to CPU (y_ -> y)
	hipMemcpyDtoH(y, y_, sizeof(double) * size);

    // confirm that results are correct
    double error = 0.0;
    for (i=0; i < size; i++) {
        error += abs(y_ref[i] - y[i]);
    }
    printf("total error: %f\n", error);
    printf("  reference: %f at (42,42)\n", y_ref[42 * m + 42]);
    printf("     result: %f at (42,42)\n", y[42 * m + 42]);

    return 0;
}
