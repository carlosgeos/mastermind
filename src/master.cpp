#include <random>
#include <iostream>
#include <algorithm>
#include "master.hpp"
#include "mastermind.hpp"

void master_main() {
	Guess solution{pick_random_solution()};
	std::cout << "Solution: ";
	for (auto& color : solution)
		std::cout << color << " ";
	std::cout << std::endl;
}

Guess pick_random_solution() {
	std::array<Color, n_colors> colors;
	std::iota(colors.begin(), colors.end(), 0);
    std::shuffle(colors.begin(), colors.end(), std::mt19937{std::random_device{}()});
	Guess solution;
    std::copy_n(colors.begin(), n_spots, solution.begin());
	return solution;
}
