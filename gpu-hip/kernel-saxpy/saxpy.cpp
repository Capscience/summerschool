#include <hip/hip_runtime.h>
#include <stdio.h>
#include <math.h>

// TODO: add a device kernel that calculates y = a * x + y
__global__ void saxpy(float *x, float *y, const float a, const int n)
{
	const int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < n)
		y[i] = a * x[i] + y[i];
}

int main(void)
{
    int i;
    const int n = 10000;
    float a = 3.4;
    float x[n], y[n], y_ref[n];
    float *x_, *y_;

    // initialise data and calculate reference values on CPU
    for (i=0; i < n; i++) {
        x[i] = sin(i) * 2.3;
        y[i] = cos(i) * 1.1;
        y_ref[i] = a * x[i] + y[i];
    }

    // TODO: allocate vectors x_ and y_ on the GPU
    hipMalloc(&x_, sizeof(float) * n);
    hipMalloc(&y_, sizeof(float) * n);
    // TODO: copy initial values from CPU to GPU (x -> x_ and y -> y_)
    hipMemcpy(x_, x, sizeof(float) * n, hipMemcpyHostToDevice);
    hipMemcpy(y_, y, sizeof(float) * n, hipMemcpyHostToDevice);

    // TODO: define grid dimensions
    int threads = 256;
    int blocks = n / threads + 1;
    // TODO: launch the device kernel
    saxpy<<<blocks, threads>>>(x_, y_, a, n);

    // TODO: copy results back to CPU (y_ -> y)
    hipMemcpy(x, x_, sizeof(float) * n, hipMemcpyDeviceToHost);
    hipMemcpy(y, y_, sizeof(float) * n, hipMemcpyDeviceToHost);

    // confirm that results are correct
    float error = 0.0;
    float tolerance = 1e-6;
    float diff;
    for (i=0; i < n; i++) {
        diff = abs(y_ref[i] - y[i]);
        if (diff > tolerance)
            error += diff;
    }
    printf("total error: %f\n", error);
    printf("  reference: %f at (42)\n", y_ref[42]);
    printf("     result: %f at (42)\n", y[42]);

    return 0;
}
