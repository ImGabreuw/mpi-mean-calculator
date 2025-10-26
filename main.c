#include <stdio.h>  
#include <stdlib.h>
#include <math.h>  
#include <time.h>

#include "mpi.h"

#define ROOT_PROCESS 0

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <vector_size_per_process>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int N = atoi(argv[1]);

    int current_process, number_of_processes;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &current_process);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    float *vector = (float*)malloc(N * sizeof(float));
    if (vector == NULL) {
        fprintf(stderr, "Process %d: Memory allocation failed\n", current_process);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    srand(time(NULL) + current_process);
    for (int i = 0; i < N; i++) {
        float x_between_0_and_1 = rand() / (float)RAND_MAX;
        vector[i] = x_between_0_and_1;
    }

    float local_sum = 0.0;
    for (int i = 0; i < N; i++) {
        local_sum += vector[i];
    }

    float local_mean = local_sum / N;

    printf("[Process %d] Local sum: %.6f\n", current_process, local_sum);
    printf("[Process %d] Local mean: %.6f\n", current_process, local_mean);

    float global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, ROOT_PROCESS, MPI_COMM_WORLD);

    if (current_process == ROOT_PROCESS) {
        float global_mean = global_sum / (N * number_of_processes);
        printf("[ROOT] Global sum: %.6f\n", global_sum);
        printf("[ROOT] Global mean: %.6f\n", global_mean);
    }

    free(vector);
    MPI_Finalize();
    return EXIT_SUCCESS;
}