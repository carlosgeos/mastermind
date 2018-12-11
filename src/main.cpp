#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include "main.hpp"

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
        timestamp();
        std::cout << "\n";
        std::cout << "P" << rank << ":  HELLO_MPI - Master process:\n";
        std::cout << "P" << rank << ":    C++/MPI version\n";
        std::cout << "P" << rank << ":    An MPI example program.\n";
        std::cout << "\n";
        std::cout << "P" << rank << ":    The number of processes is " << n_processes << "\n";
        std::cout << "\n";
    }

    double wtime;
    if (rank == 0) {
        wtime = MPI_Wtime();
    }
    std::cout << "P" << rank << ":    'Hello, world!'\n";

    if (rank == 0) {
        wtime = MPI_Wtime() - wtime;
        std::cout << "P" << rank << ":    Elapsed wall clock time = " << wtime << " seconds.\n";
    }

    MPI_Finalize();

    if (rank == 0) {
        std::cout << "\n";
        std::cout << "P" << rank << ":  HELLO_MPI:\n";
        std::cout << "P" << rank << ":    Normal end of execution.\n";
        std::cout << "\n";
        timestamp();
    }
    return 0;
}

void timestamp() {
    constexpr std::size_t buffer_size{64};
    static char time_buffer[buffer_size];
    const struct std::tm *tm_ptr;
    std::time_t now{std::time(nullptr)};
    std::strftime(time_buffer, buffer_size, "%d %B %Y %I:%M:%S %p",  std::localtime(&now));
    std::cout << time_buffer << "\n";
}
