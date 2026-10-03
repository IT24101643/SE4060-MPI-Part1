#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;

    const long long N = 10000000;

    long long local_sum = 0;
    long long total_sum = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long chunk = N / size;

    long long start = rank * chunk + 1;
    long long end;

    if (rank == size - 1)
    {
        end = N;
    }
    else
    {
        end = (rank + 1) * chunk;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    for (long long i = start; i <= end; i++)
    {
        local_sum += i;
    }

    MPI_Reduce(
        &local_sum,
        &total_sum,
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

    printf(
        "Process %d: Range %lld - %lld, Partial Sum = %lld\n",
        rank,
        start,
        end,
        local_sum
    );

    if (rank == 0)
    {
        printf("\nTotal Sum = %lld\n", total_sum);
        printf("Execution Time = %.6f seconds\n", max_time);
    }

    MPI_Finalize();

    return 0;
}
