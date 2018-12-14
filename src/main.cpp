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

    Color n_colors{10};
    std::size_t n_spots{4};

    if (rank == 0) {
        Master master{n_colors, n_spots, n_processes - 1};
        master.main();
    } else {
        Challenger challenger{n_colors, n_spots, n_processes - 1, rank - 1};
        challenger.main();
    }

    MPI_Finalize();
}
