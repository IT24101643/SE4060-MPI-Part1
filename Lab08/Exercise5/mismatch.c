#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank;
    int value = 100;
    int received = 0;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1)
    {
        printf("Rank 1 is sending to Rank 3\n");

        MPI_Ssend(
            &value,
            1,
            MPI_INT,
            3,
            0,
            MPI_COMM_WORLD
        );

        printf("Rank 1 completed send\n");
    }

    else if (rank == 3)
    {
        printf("Rank 3 is waiting for a message from Rank 2\n");

        MPI_Recv(
            &received,
            1,
            MPI_INT,
            2,
            0,
            MPI_COMM_WORLD,
            &status
        );

        printf("Rank 3 received %d\n", received);
    }

    MPI_Finalize();

    return 0;
}
