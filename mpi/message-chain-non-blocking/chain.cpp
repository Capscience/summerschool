#include <cstdio>
#include <vector>
#include <mpi.h>

void print_ordered(double t);

int main(int argc, char *argv[])
{
    int i, myid, ntasks;
    constexpr int size = 10000000;
    std::vector<int> message(size);
    std::vector<int> receiveBuffer(size);
    MPI_Status status;

    double t0, t1;

    int source, destination;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // Initialize buffers
    for (i = 0; i < size; i++) {
        message[i] = myid;
        receiveBuffer[i] = -1;
    }

    // TODO: Set source and destination ranks
    source = myid - 1;
    destination = myid + 1;
    // TODO: Treat boundaries with MPI_PROC_NULL
    if (source < 0)
        source = MPI_PROC_NULL;
    if (destination > ntasks - 1)
        destination = MPI_PROC_NULL;

    // Start measuring the time spent in communication
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    // TODO: Send messages
    MPI_Request sendrequest;

    MPI_Send_init(
            message.data(),
            size,
            MPI_INT,
            destination,
            4,
            MPI_COMM_WORLD,
            &sendrequest
    );
    // MPI_Wait(&request, MPI_STATUS_IGNORE);

    printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           myid, size, myid + 1, destination);

    // TODO: Receive messages
    MPI_Request recvrequest;
    MPI_Recv_init(&receiveBuffer[0],
            size,
            MPI_INT,
            source,
            4,
            MPI_COMM_WORLD,
            &recvrequest
    );
    
    MPI_Request requests[] = {sendrequest, recvrequest};
    MPI_Startall(2, requests);

    MPI_Waitall(2, requests, MPI_STATUS_IGNORE);
    MPI_Request_free(&requests[0]);
    MPI_Request_free(&requests[1]);

    printf("Receiver: %d. first element %d.\n",
           myid, receiveBuffer[0]);

    // Finalize measuring the time and print it out
    t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    fflush(stdout);

    print_ordered(t1 - t0);

    MPI_Finalize();
    return 0;
}

void print_ordered(double t)
{
    int i, rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (rank == 0) {
        printf("Time elapsed in rank %2d: %6.3f\n", rank, t);
        for (i = 1; i < ntasks; i++) {
            MPI_Recv(&t, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Time elapsed in rank %2d: %6.3f\n", i, t);
        }
    } else {
        MPI_Send(&t, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
}
