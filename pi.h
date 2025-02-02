#include <stdio.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <cassert>

typedef struct {
  float x;
  float y;
} Point;

Point *rand_num_gen(long int n) {
    Point *r_points = (Point *) malloc(sizeof(Point) * n);
    assert(r_points != NULL);

    for (int i = 0; i < n; i++) {
        r_points[i].x = rand() / (float)RAND_MAX;
        r_points[i].y = rand() / (float)RAND_MAX;
    }

    return r_points;
}

double pi_calc(long int n) {
    
    // MPI Initialization
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long int pt_per_proc = n / size;
    long int remainder = n % size;
    long int l_count = pt_per_proc;

    if (rank < remainder) {
        l_count += 1;
    }

    srand(time(NULL) + rank);

    long int l_result = 0;
    Point *points = rand_num_gen(l_count);  // Fix: Ensure `l_count` is used

    for (long int i = 0; i < l_count; i++) {
        if ((points[i].x * points[i].x + points[i].y * points[i].y) <= 1.0) {  // Fix: Use proper formula for π estimation
            l_result += 1;
        }
    }

    free(points);

    long int g_result = 0;
    MPI_Reduce(&l_result, &g_result, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double estimation = 0.0;

    if (rank == 0) {
        estimation = 4.0 * ((double)g_result / (double)n);  // Fix: Ensure double division
    }

    MPI_Finalize();

    return estimation;
}
