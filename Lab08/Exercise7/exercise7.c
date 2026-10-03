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

    if (size < 2)
    {
        if (rank == 0)
        {
            printf("Run the program with at least 2 processes.\n");
        }

        MPI_Finalize();
        return 0;
    }

    long long points_per_worker =
        TOTAL_POINTS / (size - 1);

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    if (rank != 0)
    {
        unsigned int seed = 1234 + rank;

        long long local_inside = 0;
        long long local_points = points_per_worker;

        if (rank == size - 1)
        {
            local_points =
                TOTAL_POINTS -
                points_per_worker * (size - 2);
        }

        for (long long i = 0; i < local_points; i++)
        {
            double x =
                (double)rand_r(&seed) / RAND_MAX;

            double y =
                (double)rand_r(&seed) / RAND_MAX;

            if ((x * x + y * y) <= 1.0)
            {
                local_inside++;
            }
        }

        int buffer_size =
            sizeof(long long) + MPI_BSEND_OVERHEAD;

        void *buffer = malloc(buffer_size);

        MPI_Buffer_attach(buffer, buffer_size);

        MPI_Bsend(
            &local_inside,
            1,
            MPI_LONG_LONG,
            0,
            0,
            MPI_COMM_WORLD
        );

        printf(
            "Rank %d sent buffered result\n",
            rank
        );

        MPI_Buffer_detach(&buffer, &buffer_size);

        free(buffer);
    }
    else
    {
        long long total_inside = 0;

        MPI_Status status;

        for (int i = 1; i < size; i++)
        {
            long long received_inside;

            MPI_Recv(
                &received_inside,
                1,
                MPI_LONG_LONG,
                MPI_ANY_SOURCE,
                0,
                MPI_COMM_WORLD,
                &status
            );

            printf(
                "Received buffered result from Rank %d\n",
                status.MPI_SOURCE
            );

            total_inside += received_inside;
        }

        double pi =
            4.0 * (double)total_inside /
            (double)TOTAL_POINTS;

        double end_time = MPI_Wtime();

        printf("Total Trials = %lld\n", TOTAL_POINTS);
        printf(
            "Points Inside Circle = %lld\n",
            total_inside
        );
        printf("Estimated Pi = %.10f\n", pi);
        printf(
            "Execution Time = %.6f seconds\n",
            end_time - start_time
        );
    }

    MPI_Finalize();

    return 0;
}
