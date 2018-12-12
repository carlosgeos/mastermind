#include <random>
#include <iostream>
#include <algorithm>
#include <mpi.h>
#include "master.hpp"
#include "mastermind.hpp"

void master_main(int n_challengers) {
	Guess solution{pick_random_solution()};
	std::cout << "Solution: ";
	for (auto& color : solution)
		std::cout << color << " ";
	std::cout << std::endl;
	bool solved{false};
	int j{0};
	while (j < 3) {
		// Receive message from all challengers
		std::vector<Guess> guesses;
		for (int i{1}; i <= n_challengers; ++i) {
			Guess guess;
			MPI_Recv(guess.data(), n_spots, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			std::cout << "Received guess ";
			for (auto& color : guess)
				std::cout << color << " ";
			std::cout << std::endl;
			guesses.push_back(guess);
		}

		// Pick a random guess
		std::uniform_int_distribution<> dis(0, n_challengers - 1);
	    static std::random_device rd;
	    static std::mt19937 gen(rd());
	    Guess random_guess{guesses.at(dis(gen))};

		Evaluation evaluation{evaluate(random_guess, solution)};
		std::cout << "Broadcasting evaluation of ";
		for (auto& color : random_guess)
			std::cout << color << " ";
		std::cout << std::endl;

		// Broadcast evaluation to all challengers
		std::array<Color, n_spots + 2> evaluation_data;
		for (std::size_t i{0}; i < n_spots; ++i)
			evaluation_data[i] = random_guess[i];
		evaluation_data[n_spots] = evaluation.color_only;
		evaluation_data[n_spots + 1] = evaluation.perfect;
		MPI_Bcast(evaluation_data.data(), n_spots + 2, MPI_INT, 0, MPI_COMM_WORLD);
		++j;
	}
}

Guess pick_random_solution() {
	std::array<Color, n_colors> colors;
	std::iota(colors.begin(), colors.end(), 0);
    std::shuffle(colors.begin(), colors.end(), std::mt19937{std::random_device{}()});
	Guess solution;
    std::copy_n(colors.begin(), n_spots, solution.begin());
	return solution;
}

Evaluation evaluate(Guess guess, Guess solution) {
	Evaluation result{0, 0};
	for (std::size_t i{0}; i < n_spots; ++i) {
		// If the guess had it right at the spot
		if (guess[i] == solution[i])
			++result.perfect;
		// If the guess spot is in the solution but elsewhere
		else if (std::find(solution.begin(), solution.end(), guess[i]) != solution.end()) {
			++result.color_only;
		}
	}
	return result;
}
