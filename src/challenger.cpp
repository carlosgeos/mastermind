#include <iostream>
#include <algorithm>
#include <mpi.h>
#include "challenger.hpp"


void challenger_main(int n_challengers, int rank) {
	std::vector<Guess> search_space{get_search_space(n_challengers, rank)};
	std::cout << "[" << rank << "] size of search space: " << search_space.size() << std::endl;
	bool solved{false};
	int i{0};
	while (i < 3) {
		send_guess(search_space);
		receive_evaluation(search_space);
		++i;
	}
}

std::vector<Guess> get_search_space(int n_challengers, int rank) {
	std::size_t max_guess{1};
	for (std::size_t i{0}; i < n_spots; ++i) {
		max_guess *= n_colors;
	}

	std::vector<Guess> guesses;
	Guess current;
	std::fill(current.begin(), current.end(), 0);

	for (std::size_t i{0}; i < max_guess; ++i) {
		if (is_legal(current))
			guesses.push_back(current);
		current[n_spots - 1] += 1;
		std::size_t carry{n_spots - 1};
		while(current[carry] >= n_colors and carry > 0) {
			current[carry] = 0;
			--carry;
			current[carry] += 1;
		}
	}

	std::size_t space_size{guesses.size() / n_challengers};
	std::vector<Guess> search_space(
			guesses.begin() + (rank * space_size),
			guesses.begin() + std::min((rank + 1) * space_size, guesses.size()));

	if (rank == n_challengers - 1)
		search_space.insert(search_space.end(), guesses.end() - guesses.size() % n_challengers, guesses.end());

	return search_space;
}


bool is_legal(const Guess& guess) {
	for (std::size_t i{0}; i < n_spots; ++i) {
		for (std::size_t j{i + 1}; j < n_spots; ++j) {
			if (guess[i] == guess[j])
				return false;
		}
	}
	return true;
}

void send_guess(const std::vector<Guess>& search_space) {
	if(not search_space.empty()) {
		const Guess& guess{search_space.front()};
		std::cout << "Sending guess ";
		for (auto& color : guess)
			std::cout << color << " ";
		std::cout << std::endl;
		MPI_Send(guess.data(), n_spots, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
}

void receive_evaluation(std::vector<Guess>& search_space) {
	std::array<Color, n_spots + 2> evaluation;
	MPI_Bcast(evaluation.data(), n_spots + 2, MPI_INT, 0, MPI_COMM_WORLD);
	std::cout << "Reveived evaluation ";
	for (auto& color : evaluation)
		std::cout << color << " ";
	std::cout << std::endl;

}
