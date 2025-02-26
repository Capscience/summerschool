/* Heat equation solver in 2D. */

#include <string>
#include <iostream>
#include <iomanip>

#include "heat.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char **argv)
{

    const int image_interval = 100;    // Image output interval

    int nsteps;                 // Number of time steps
    Field current, previous;    // Current and previous temperature fields

    initialize(argc, argv, current, previous, nsteps);

    // Output the initial field
    write_field(current, 0);

    auto average_temp = average(current);
    std::cout << "Simulation parameters: " 
              << "rows: " << current.nx_full << " columns: " << current.ny_full
              << " time steps: " << nsteps << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Average temperature at start: " << average_temp << std::endl;

    const double a = 0.5;     // Diffusion constant 
    auto dx2 = current.dx * current.dx;
    auto dy2 = current.dy * current.dy;
    // Largest stable time step 
    auto dt = dx2 * dy2 / (2.0 * a * (dx2 + dy2));

    //Get the start time stamp 
    auto start_clock = omp_get_wtime();
    
    auto *currdata = current.temperature.data();
    auto *prevdata = previous.temperature.data();
    auto data_size = current.temperature.size();
#pragma omp target enter data map(to:currdata[0:data_size],prevdata[0:data_size])
    // Time evolve
    for (int iter = 1; iter <= nsteps; iter++) {
        evolve(current, previous, a, dt, currdata, prevdata);
        if (iter % image_interval == 0) {
#pragma omp target update to(currdata[0:current.temperature.size()])
            write_field(current, iter);
        }
        // Swap current field so that it will be used
        // as previous for next iteration step
        std::swap(current, previous);
    }
#pragma omp target exit data map(from:currdata[0:data_size],prevdata[0:data_size])
    auto stop_clock = omp_get_wtime();

    // Average temperature for reference 
    average_temp = average(previous);

    std::cout << "Iteration took " << (stop_clock - start_clock)
              << " seconds." << std::endl;
    std::cout << "Average temperature: " << average_temp << std::endl;
    if (1 == argc) {
        std::cout << "Reference value with default arguments: " 
                  << 59.281239 << std::endl;
    }

    
    // Output the final field
    write_field(previous, nsteps);

    return 0;
}
