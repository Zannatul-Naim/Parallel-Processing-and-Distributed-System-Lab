#include <stdio.h>
#include<stdlib.h>
#include<mpi.h>

void display() {

}

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // printf("rank: %d size: %d\n", rank, size);

    int K = 100; // no of total matrix
    int M = 50; // no of row for matrix A
    int N = 50; // no of row for matrix B and no of col for matrix A
    int P = 50; // no of col for matrix B

    /*
    int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
    */

    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&P, 1, MPI_INT, 0, MPI_COMM_WORLD);


    if(K%size) {
        printf("Number of matrices must be divisible by the number of processes.\n");
        MPI_Finalize();
        return 1;
    }

    int A[K][M][N];
    int B[K][N][P];
    int C[K][M][P];

    // initialize the matrices in the root process

    if(rank == 0) {

        for(int k = 0; k < K; k++) {
            for(int m = 0; m < M; m++) {
                for(int n = 0; n < N; n++) {
                    A[k][m][n] = rand()%100;
                }
            }

            for(int n = 0; n < N; n++) {
                for(int p = 0; p < P; p++) {
                    B[k][n][p] = rand()%100;
                }
            } 
        } 
    }

    // Buffer to store portion of the matrices assigned to each process
    int localA[K/size][M][N];
    int localB[K/size][N][P];
    int localC[K/size][M][P];

    /*
    int MPI_Scatter(
        const void *sendbuf, int sendcount, MPI_Datatype sendtype,
        void *recvbuf, int recvcount, MPI_Datatype recvtype,
        int root, MPI_Comm comm);

    */

    MPI_Scatter(A, (K/size)*M*N, MPI_INT, localA, (K/size)*M*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, (K/size)*N*P, MPI_INT, localB, (K/size)*N*P, MPI_INT, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    // matrix multiplication

    for(int k = 0; k < (K/size); k++) {
        for(int i = 0; i < M; i++) {
            for(int j = 0; j < P; j++) {
                localC[k][i][j] = 0;
                for(int l = 0; l < N; l++) {
                    localC[k][i][j] += (localA[k][i][l] * localB[k][l][j]) % 100;
                }
                localC[k][i][j] = localC[k][i][j] % 100;
            }
        }
    }

    double end_time = MPI_Wtime();

    /*
    int MPI_Gather(
        const void *sendbuf, int sendcount, MPI_Datatype sendtype,
        void *recvbuf, int recvcount, MPI_Datatype recvtype,
        int root, MPI_Comm comm);

    */

    MPI_Gather(localC, (K/size)*M*P, MPI_INT, C, (K/size)*M*P, MPI_INT, 0, MPI_COMM_WORLD);

    // barrier to synchronize all processes before timing starts

    MPI_Barrier(MPI_COMM_WORLD);

    double time_taken = end_time - start_time;
    printf("Process %d: Time taken = %f mili seconds\n", rank, time_taken*1000);

    MPI_Finalize();

    return 0;
}