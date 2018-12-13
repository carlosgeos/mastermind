#include <iostream>
#include <algorithm>
#include <functional>
#include <mpi.h>
#include "challenger.hpp"


void challenger_main(int n_challengers, int rank) {
	std::vector<Guess> search_space{get_search_space(n_challengers, rank)};
	std::cout << "[" << rank << "] size of search space: " << search_space.size() << std::endl;
	bool solved{false};
	while (not solved) {
		send_guess(search_space, rank);
		solved = receive_evaluation(search_space, rank);
	}
}

std::vector<Guess> get_search_space(int n_challengers, int rank) {
	std::size_t max_guess{1};
	for (std::size_t i{0}; i < n_spots; ++i) {
		max_guess *= n_colors;
	}

	std::vector<Guess> guesses;
	Guess current;
	current.fill(0);

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

void send_guess(const std::vector<Guess>& search_space, int rank) {
	Guess guess;
	// Fill with zeroes by default
	guess.fill(0);

	if(not search_space.empty())
		guess = search_space.front();

	std::cout << "[" << rank << "] Sending guess ";
	for (auto& color : guess)
		std::cout << color << " ";
	std::cout << std::endl;
	MPI_Gather(guess.data(), n_spots, MPI_INT, nullptr, n_spots, MPI_INT, 0, MPI_COMM_WORLD);
}

bool receive_evaluation(std::vector<Guess>& search_space, int rank) {
	std::array<Color, n_spots + 2> evaluation_data;
	MPI_Bcast(evaluation_data.data(), n_spots + 2, MPI_INT, 0, MPI_COMM_WORLD);

	Evaluation evaluation{evaluation_data[n_spots], evaluation_data[n_spots + 1]};
	Guess evaluated_guess;
	std::copy_n(evaluation_data.begin(), n_spots, evaluated_guess.begin());


	std::cout << "[" << rank << "] Reveived evaluation ";
	for (auto& color : evaluated_guess)
		std::cout << color << " ";
	std::cout << " -> color_only = " << evaluation.color_only
	          << "; perfect = " << evaluation.perfect << std::endl;

	if (evaluation.perfect == n_spots)
		return true;
		
	// Use erase and std::remove_if to filter out elements of the search space
	// that are not plausible
	search_space.erase(std::remove_if(
				search_space.begin(),
				search_space.end(),
				std::bind(is_not_plausible, std::placeholders::_1, evaluated_guess, evaluation)),
			search_space.end());

	return false;
}

bool is_not_plausible(const Guess& guess, const Guess& evaluated_guess, const Evaluation& evaluation) {
	Evaluation difference{evaluate(guess, evaluated_guess)};
	if (difference.perfect != evaluation.perfect)
		return true;
	else if (difference.color_only != evaluation.color_only)
		return true;
	return false;
}
