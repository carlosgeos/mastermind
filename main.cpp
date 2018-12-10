# include <cstdlib>
# include <ctime>
# include <iomanip>
# include <iostream>
# include <mpi.h>

void timestamp();

int main(int argc, char *argv[]) {
    int ierr{MPI_Init(&argc, &argv)};

    if (ierr != 0) {
        std::cout << "\n";
        std::cout << "HELLO_MPI - Fatal error!\n";
        std::cout << "MPI_Init returned nonzero ierr.\n";
        return 1;
    }

    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0) {
        timestamp();
        std::cout << "\n";
        std::cout << "P" << id << ":  HELLO_MPI - Master process:\n";
        std::cout << "P" << id << ":    C++/MPI version\n";
        std::cout << "P" << id << ":    An MPI example program.\n";
        std::cout << "\n";
        std::cout << "P" << id << ":    The number of processes is " << p << "\n";
        std::cout << "\n";
    }

    double wtime;
    if (id == 0) {
        wtime = MPI_Wtime();
    }
    std::cout << "P" << id << ":    'Hello, world!'\n";

    if (id == 0) {
        wtime = MPI_Wtime() - wtime;
        std::cout << "P" << id << ":    Elapsed wall clock time = " << wtime << " seconds.\n";
    }

    MPI_Finalize();

    if (id == 0) {
        std::cout << "\n";
        std::cout << "P" << id << ":  HELLO_MPI:\n";
        std::cout << "P" << id << ":    Normal end of execution.\n";
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
