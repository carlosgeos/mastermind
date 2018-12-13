#include <iostream>
#include <mpi.h>
#include "Master.hpp"
#include "Challenger.hpp"

int main(int argc, char *argv[]) {
    int ierr{MPI_Init(&argc, &argv)};
    if (ierr != 0) {
        std::cout << "\n";
        std::cout << "HELLO_MPI - Fatal error!\n";
        std::cout << "MPI_Init returned nonzero ierr.\n";
        return 1;
    }

    int n_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &n_processes);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "Running on " << n_processes << " processes" << std::endl;
        Master master{n_processes - 1};
        master.main();
    } else {
        Challenger challenger{n_processes - 1, rank - 1};
        challenger.main();
    }

    MPI_Finalize();
}
