#include <stdio.h>

#define NX 1024000

int main(void)
{
	double vecA[NX], vecB[NX], vecC[NX];

	/* Initialization of the vectors */
	for (int i = 0; i < NX; i++) {
		vecA[i] = 1.0 / ((double) (NX - i));
		vecB[i] = vecA[i] * vecA[i];
	}

	// TODO: Implement vector addition vecC = vecA + vecB and use OpenMP
	//       for computing it in the device
//#pragma omp target
//#pragma omp teams
//#pragma omp distribute
	for (int i = 0; i < NX; i++) {
		vecC[i] = vecA[i] + vecB[i];
	}

	double sum = 0.0;
	/* Compute the check value */
	for (int i = 0; i < NX; i++) {
		sum += vecC[i];
	}
	printf("Reduction sum: %18.16f\n", sum);

	return 0;
}
