/* Main solver routines for heat equation solver */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

#include "heat.h"

/* Exchange the boundary values */
void exchange(field *temperature, parallel_data *parallel)
{
    double *data;
    double *sbuf_up, *sbuf_down, *rbuf_up, *rbuf_down;
    data = temperature->data;

    sbuf_up = data + temperature->ny + 2; // upper data
    rbuf_down = data + (temperature->nx + 1) * (temperature->ny + 2); // lower halo

    MPI_Sendrecv(sbuf_up, temperature->ny + 2, MPI_DOUBLE,
                 parallel->nup, 11,
                 rbuf_down, temperature->ny + 2, MPI_DOUBLE,
                 parallel->ndown, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Send to the down, receive from up
    //sbuf_down = data + temperature->nx * (temperature->ny + 2); // lower data
    //rbuf_up = data; // upper halo
//
    //MPI_Sendrecv(sbuf_down, temperature->ny + 2, MPI_DOUBLE,
                 //parallel->ndown, 12,
                 //rbuf_up, temperature->ny + 2, MPI_DOUBLE,
                 //parallel->nup, 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
    //sbuf_up = (double *)malloc(sizeof(double) * (temperature->ny + 2));
    //sbuf_down = (double *)malloc(sizeof(double) * (temperature->ny + 2));
    //rbuf_up = (double *)malloc(sizeof(double) * (temperature->ny + 2));
    //rbuf_down = (double *)malloc(sizeof(double) * (temperature->ny + 2));
    //data = temperature->data;
//
    //// TODO start: implement halo exchange

    // You need to do a little bit of math to determine correct linear
    // indeces for inner boundaries and ghost layers

    // Send to up, receive from down
    //printf("Rank: %d, nup %d, ndown %d\n", parallel->rank, parallel->nup, parallel->ndown);
    //fflush(stdout);
    //MPI_Barrier(MPI_COMM_WORLD);
//
    //for (int i = 0; i < temperature->ny + 2; i++) {
        //sbuf_up[i] = data[temperature->nx + 2 + i];
    //}
    //MPI_Sendrecv(
        //sbuf_up,
        //temperature->ny + 2,
        //MPI_DOUBLE,
        //parallel->nup,
        //42,
        //rbuf_down,
        //temperature->ny + 2,
        //MPI_DOUBLE,
        //parallel->ndown,
        //42,
        //MPI_COMM_WORLD,
        //MPI_STATUS_IGNORE
    //);

    // Send to down, receive from up
    //for (int i = 0; i < temperature->ny + 2; i++) {
        //sbuf_up[i] = data[(temperature->ny * (temperature->nx + 2)) + i];
    //}
    //MPI_Sendrecv(
        //sbuf_down,
        //temperature->ny + 2,
        //MPI_DOUBLE,
        //parallel->ndown,
        //42,
        //rbuf_up,
        //temperature->ny + 2,
        //MPI_DOUBLE,
        //parallel->nup,
        //42,
        //MPI_COMM_WORLD,
        //MPI_STATUS_IGNORE
    //);

    //free(sbuf_up);
    //free(sbuf_down);
    //free(rbuf_up);
    //free(rbuf_down);
    // TODO end


}

/* Update the temperature values using five-point stencil */
void evolve(field *curr, field *prev, double a, double dt)
{
  double dx2, dy2;
  int nx, ny;
  double *currdata, *prevdata;

  /* HINT: to help the compiler do not access members of structures
   * within OpenACC parallel regions */
  currdata = curr->data;
  prevdata = prev->data;
  nx = curr->nx;
  ny = curr->ny;

  /* Determine the temperature field at next time step
   * As we have fixed boundary conditions, the outermost gridpoints
   * are not updated. */
  dx2 = prev->dx * prev->dx;
  dy2 = prev->dy * prev->dy;
  for (int i = 1; i < nx + 1; i++) {
    for (int j = 1; j < ny + 1; j++) {
            int ind = i * (ny + 2) + j;
            int ip = (i + 1) * (ny + 2) + j;
            int im = (i - 1) * (ny + 2) + j;
	    int jp = i * (ny + 2) + j + 1;
	    int jm = i * (ny + 2) + j - 1;
            currdata[ind] = prevdata[ind] + a * dt *
	      ((prevdata[ip] -2.0 * prevdata[ind] + prevdata[im]) / dx2 +
	       (prevdata[jp] - 2.0 * prevdata[ind] + prevdata[jm]) / dy2);
    }
  }

}
