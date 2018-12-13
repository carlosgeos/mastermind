#include <iostream>
#include <algorithm>
#include <functional>
#include <mpi.h>
#include "challenger.hpp"


Challenger::Challenger(int n_challengers, int rank):
	_rank{rank}
{
	set_search_space(n_challengers);
}

void Challenger::main() {
	std::cout << "[" << _rank << "] size of search space: " << _search_space.size() << std::endl;
	bool solved{false};
	while (not solved) {
		send_guess();
		solved = receive_evaluation();
	}
}

void Challenger::set_search_space(int n_challengers) {
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
	_search_space.assign(
			guesses.begin() + (_rank * space_size),
			guesses.begin() + std::min((_rank + 1) * space_size, guesses.size()));

	if (_rank == n_challengers - 1)
		_search_space.insert(_search_space.end(), guesses.end() - guesses.size() % n_challengers, guesses.end());
}


bool Challenger::is_legal(const Guess& guess) {
	for (std::size_t i{0}; i < n_spots; ++i) {
		for (std::size_t j{i + 1}; j < n_spots; ++j) {
			if (guess[i] == guess[j])
				return false;
		}
	}
	return true;
}

void Challenger::send_guess() {
	Guess guess;
	// Fill with zeroes by default
	guess.fill(0);

	if(not _search_space.empty())
		guess = _search_space.front();

	std::cout << "[" << _rank << "] Sending guess ";
	for (auto& color : guess)
		std::cout << color << " ";
	std::cout << std::endl;
	MPI_Gather(guess.data(), n_spots, MPI_INT, nullptr, n_spots, MPI_INT, 0, MPI_COMM_WORLD);
}

bool Challenger::receive_evaluation() {
	std::array<Color, n_spots + 2> evaluation_data;
	MPI_Bcast(evaluation_data.data(), n_spots + 2, MPI_INT, 0, MPI_COMM_WORLD);

	Evaluation evaluation{evaluation_data[n_spots], evaluation_data[n_spots + 1]};
	Guess evaluated_guess;
	std::copy_n(evaluation_data.begin(), n_spots, evaluated_guess.begin());


	std::cout << "[" << _rank << "] Reveived evaluation ";
	for (auto& color : evaluated_guess)
		std::cout << color << " ";
	std::cout << " -> color_only = " << evaluation.color_only
	          << "; perfect = " << evaluation.perfect << std::endl;

	if (evaluation.perfect == n_spots)
		return true;

	// Use erase and std::remove_if to filter out elements of the search space
	// that are not plausible
	_search_space.erase(std::remove_if(
				_search_space.begin(),
				_search_space.end(),
				std::bind(is_not_plausible, std::placeholders::_1, evaluated_guess, evaluation)),
			_search_space.end());

	return false;
}

bool Challenger::is_not_plausible(const Guess& guess, const Guess& evaluated_guess, const Evaluation& evaluation) {
	Evaluation difference{evaluate(guess, evaluated_guess)};
	if (difference.perfect != evaluation.perfect)
		return true;
	else if (difference.color_only != evaluation.color_only)
		return true;
	return false;
}
