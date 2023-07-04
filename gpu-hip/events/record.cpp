#include <cstdio>
#include <time.h>
#include <hip/hip_runtime.h>

/* A simple GPU kernel definition */
__global__ void kernel(int *d_a, int n_total)
{
	const int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if(idx < n_total)
		d_a[idx] = idx;
}

/* The main function */
int main(){

	// Problem size
	constexpr int n_total = 4194304; // pow(2, 22);

	// Device grid sizes
	constexpr int blocksize = 256;
	constexpr int gridsize = (n_total - 1 + blocksize) / blocksize;

	// Allocate host and device memory
	int *a, *d_a;
	const int bytes = n_total * sizeof(int);
	hipHostMalloc((void**)&a, bytes); // host pinned
	hipMalloc((void**)&d_a, bytes);   // device pinned

	// Create events
	hipEvent_t kernel_start_event;
	hipEvent_t kernel_finished_event;
	hipEvent_t memcpy_start_event;
	hipEventCreate(&kernel_start_event);
	hipEventCreate(&kernel_finished_event);
	hipEventCreate(&memcpy_start_event);

	// Create stream
	hipStream_t stream;
	hipStreamCreate(&stream);

	// Start timed GPU kernel and device-to-host copy
	clock_t start_kernel_clock = clock();
	hipEventRecord(kernel_start_event, stream);
	kernel<<<gridsize, blocksize, 0, stream>>>(d_a, n_total);

	clock_t start_d2h_clock = clock();
	hipEventRecord(memcpy_start_event);
	hipMemcpyAsync(a, d_a, bytes, hipMemcpyDeviceToHost, stream);

	clock_t stop_clock = clock();
	hipEventRecord(kernel_finished_event);
	hipStreamSynchronize(stream);

	// Exctract elapsed timings from event recordings
	float elapsed_kernel;
	float elapsed_mem;
	float elapsed_total;
	hipEventElapsedTime(&elapsed_kernel, kernel_start_event, memcpy_start_event);
	hipEventElapsedTime(&elapsed_mem, memcpy_start_event, kernel_finished_event);
	hipEventElapsedTime(&elapsed_total, kernel_start_event, kernel_finished_event);

	// Check that the results are right
	int error = 0;
	for(int i = 0; i < n_total; ++i){
		if(a[i] != i)
			error = 1;
	}

	// Print results
	if(error)
		printf("Results are incorrect!\n");
	else
		printf("Results are correct!\n");

	// Print event timings
	printf("Event timings:\n");

	// Print clock timings
	printf("clock_t timings:\n");
	printf("  %.3f ms - kernel\n", 1e3 * (double)(start_d2h_clock - start_kernel_clock) / CLOCKS_PER_SEC);
	printf("  %.3f ms - device to host copy\n", 1e3 * (double)(stop_clock - start_d2h_clock) / CLOCKS_PER_SEC);
	printf("  %.3f ms - total time\n", 1e3 * (double)(stop_clock - start_kernel_clock) / CLOCKS_PER_SEC);

	printf("Event timings:\n");
	printf("  %.3f ms - kernel\n", elapsed_kernel);
	printf("  %.3f ms - device to host copy\n", elapsed_mem);
	printf("  %.3f ms - total time\n", elapsed_total);
	// Destroy Stream
	hipStreamDestroy(stream);

	// Destroy events

	// Deallocations
	hipFree(d_a); // Device
	hipHostFree(a); // Host
}
