#include <vector>
#include <algorithm>
#include "challenger.hpp"


void challenger_main(int n_challengers, int rank) {
	std::vector<Guess> search_space{get_search_space(n_challengers, rank)};

}

std::vector<Guess> get_search_space(int n_challengers, int rank) {
	std::size_t max_guess{1};
	for (std::size_t i{0}; i < n_spots; ++i) {
		max_guess *= n_colors;
	}

	std::vector<Guess> guesses;
	Guess current_guess;
	std::fill(current_guess.begin(), current_guess.end(), 0);
	for (std::size_t i{0}; i < n_spots; ++i) {
		for (std::size_t j{0}; j < n_colors; ++j) {
			current_guess[i] = j;
			if (is_legal(current_guess))
				guesses.push_back(current_guess);
		}
	}

	std::size_t space_size{guesses.size() / n_challengers};
	std::vector<Guess> search_space(
			guesses.begin() + (rank * space_size),
			guesses.begin() + std::min((rank + 1) * space_size, guesses.size()));
	return search_space
}


bool is_legal(Guess guess) {
	return true;
}
