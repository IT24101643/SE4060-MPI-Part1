#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank;

    MPI_Status status;

    int x[10];
    int y[10];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1)
    {
        for (int i = 0; i < 10; i++)
        {
            x[i] = i * 10;
        }

        int buffer_size =
            10 * sizeof(int) + MPI_BSEND_OVERHEAD;

        void *buffer = malloc(buffer_size);

        MPI_Buffer_attach(buffer, buffer_size);

        MPI_Bsend(
            x,
            10,
            MPI_INT,
            3,
            0,
            MPI_COMM_WORLD
        );

        printf("Rank 1 sent buffered message\n");

        MPI_Buffer_detach(&buffer, &buffer_size);

        free(buffer);
    }

    else if (rank == 3)
    {
        MPI_Recv(
            y,
            10,
            MPI_INT,
            1,
            0,
            MPI_COMM_WORLD,
            &status
        );

        printf("Rank 3 received: ");

        for (int i = 0; i < 10; i++)
        {
            printf("%d ", y[i]);
        }

        printf("\n");
    }

    MPI_Finalize();

    return 0;
}

