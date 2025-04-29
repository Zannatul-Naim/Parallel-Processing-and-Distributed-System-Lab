# 📘 MPI_Bcast — Explanation and Example
```MPI_Bcast``` is a collective communication operation in MPI that broadcasts data from one process (the root) to all other processes in a communicator.


## 🧠 Key Concepts
__Prototype:__

```c
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

``` 

__Parameters:__

    buffer: Pointer to data (same in all processes).
    count: Number of elements to broadcast.
    datatype: Type of elements (e.g., MPI_INT, MPI_FLOAT).
    root: Rank of the sending process.
    comm: Communicator (usually MPI_COMM_WORLD).

## ✅ Use Case

When one process (say, rank 0) has data, and all others need to receive that same data.

## 🧪 Example Code (in C)

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int number;

    MPI_Init(&argc, &argv);                 // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get total number of processes

    if (rank == 0) {
        number = 100; // Only rank 0 initializes the value
        printf("Process %d broadcasting number %d\n", rank, number);
    }

    // Broadcast from rank 0 to all other processes
    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received number %d\n", rank, number);

    MPI_Finalize(); // Finalize MPI
    return 0;
}
```

## 💡 Output Example (with 4 processes)

```arduino
Process 0 broadcasting number 100
Process 0 received number 100
Process 1 received number 100
Process 2 received number 100
Process 3 received number 100
```
Each process receives the value of number broadcast from rank 0.

