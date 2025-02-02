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
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long int ppc = n / size;
    long int rem = n % size;
    long int lp = ppc;
    if (rank < rem) {
        lp += 1;
    }

    srand(time(NULL) + rank);

    long int local = 0;
    for (long int i = 0; i < lp; i++) {
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        if ((x * x + y * y) <= 1.0) {
            local++;
        }
    }

    long int global = 0;
    MPI_Reduce(&local, &global, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi_estimate = 0.0;
    if (rank == 0) {
        pi_estimate = 4.0 * ((double)global / (double)n);
    }

    return pi_estimate;
}
