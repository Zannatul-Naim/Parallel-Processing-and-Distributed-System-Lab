# 📘 MPI_Scatter — Explanation and Example
```MPI_Scatter``` is used to distribute chunks of an array (or data) from one root process to all processes in the communicator, including itself.

## 🧠 Key Concepts

__Prototype:__

```c
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
```
__Parameters:__

    sendbuf: Address of the send buffer (significant only at root).
    sendcount: Number of elements sent to each process.
    sendtype: Type of send data.
    recvbuf: Address of the receive buffer (valid in all processes).
    recvcount: Number of elements each process receives.
    recvtype: Type of receive data.
    root: Rank of the sending process.
    comm: Communicator (e.g., MPI_COMM_WORLD).

## ✅ Use Case
When root process has an array and wants to distribute it evenly among all processes.

## 🧪 Example Code (in C)

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int data[4];  // Only rank 0 will use this
    int recv_num;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get total number of processes

    if (size != 4) {
        if (rank == 0) printf("Run this program with 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        // Initialize data array at root
        data[0] = 10;
        data[1] = 20;
        data[2] = 30;
        data[3] = 40;
        printf("Root process has data: ");
        for (int i = 0; i < 4; i++) printf("%d ", data[i]);
        printf("\n");
    }

    // Scatter one element to each process
    MPI_Scatter(data, 1, MPI_INT, &recv_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received %d\n", rank, recv_num);

    MPI_Finalize();
    return 0;
}
```

## 💡 Output Example (4 processes)

```arduino
Root process has data: 10 20 30 40
Process 0 received 10
Process 1 received 20
Process 2 received 30
Process 3 received 40
```

Each process gets one unique element from the root's array.