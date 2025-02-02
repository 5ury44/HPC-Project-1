#include <stdio.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <cassert>

double pi_calc(long int n) {
    
    // MPI Initialization
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Work Distribution
    long int points_per_proc = n / size;
    long int remainder = n % size;
    long int local_points = points_per_proc + (rank < remainder ? 1 : 0);

    // Random Seed Initialization
    srand(time(NULL) + rank);

    // Monte Carlo Simulation
    long int local_count = 0;
    for (long int i = 0; i < local_points; i++) {
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        if ((x * x + y * y) <= 1.0) {
            local_count++;
        }
    }

    // Reduce Local Counts to Get Global Count
    long int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Compute π Estimation
    double pi_estimate = 0.0;
    if (rank == 0) {
        pi_estimate = 4.0 * ((double)global_count / (double)n);
    }

    return pi_estimate;
}
