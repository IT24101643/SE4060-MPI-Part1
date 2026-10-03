#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;

    const long long TOTAL_POINTS = 10000000;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long base_points = TOTAL_POINTS / size;
    long long local_points;

    if (rank == size - 1)
    {
        local_points = TOTAL_POINTS - base_points * (size - 1);
    }
    else
    {
        local_points = base_points;
    }

    unsigned int seed = 1234 + rank;

    long long local_inside = 0;

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    for (long long i = 0; i < local_points; i++)
    {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;

        if ((x * x + y * y) <= 1.0)
        {
            local_inside++;
        }
    }

    long long total_inside = 0;

    MPI_Reduce(
        &local_inside,
        &total_inside,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    double end_time = MPI_Wtime();

    double local_time = end_time - start_time;
    double max_time = 0;

    MPI_Reduce(
        &local_time,
        &max_time,
        1,
        MPI_DOUBLE,
        MPI_MAX,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0)
    {
        double pi =
            4.0 * (double)total_inside /
            (double)TOTAL_POINTS;

        printf("Total Trials = %lld\n", TOTAL_POINTS);
        printf("Points Inside Circle = %lld\n", total_inside);
        printf("Estimated Pi = %.10f\n", pi);
        printf("Execution Time = %.6f seconds\n", max_time);
    }

    MPI_Finalize();

    return 0;
}

