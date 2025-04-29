# 📘 MPI_Gather — Explanation and Example
```MPI_Gather``` collects data from all processes in a communicator and gathers it into an array at the root process.

## 🧠 Key Concepts
__Prototype:__

```c
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
```

__Parameters:__

    sendbuf: Address of send buffer (each process sends from here).
    sendcount: Number of elements each process sends.
    sendtype: Type of elements sent.
    recvbuf: Address of receive buffer (only used at root).
    recvcount: Number of elements received from each process.
    recvtype: Type of receive elements.
    root: Rank of root process.
    comm: Communicator (e.g., MPI_COMM_WORLD).

## ✅ Use Case
Each process has a value, and the root wants to collect all these values in an array.

## 🧪 Example Code (in C)

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int send_num;
    int recv_data[4]; // Only root will use this

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get total number of processes

    if (size != 4) {
        if (rank == 0) printf("Run this program with 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    // Each process sets its own value
    send_num = (rank + 1) * 100;

    printf("Process %d sending %d\n", rank, send_num);

    // Gather all values at root process
    MPI_Gather(&send_num, 1, MPI_INT, recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints gathered data
    if (rank == 0) {
        printf("Root process received data: ");
        for (int i = 0; i < 4; i++) printf("%d ", recv_data[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
```

## 💡 Output Example (4 processes)

```arduino
Process 0 sending 100
Process 1 sending 200
Process 2 sending 300
Process 3 sending 400
Root process received data: 100 200 300 400
```
Each process sends its number, and only the root collects them into an array.