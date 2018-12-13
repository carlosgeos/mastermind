#include <iostream>
#include <mpi.h>
#include "Master.hpp"
#include "Challenger.hpp"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int n_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &n_processes);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        Master master{n_processes - 1};
        master.main();
    } else {
        Challenger challenger{n_processes - 1, rank - 1};
        challenger.main();
    }

    MPI_Finalize();
}
