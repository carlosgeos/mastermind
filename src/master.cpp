#include <random>
#include <iostream>
#include "master.hpp"
#include "mastermind.hpp"

void master_main() {
	Guess solution{pickRandomSolution()};
	std::cout << "Solution: ";
	for (auto& color : solution)
		std::cout << color << " ";
	std::cout << std::endl;
}

Guess pickRandomSolution() {
    // Seed with a real random value, if available
    std::random_device rd;
    std::default_random_engine engine(rd());
	// Create an integer distribution
	std::uniform_int_distribution<int> uniform_dist(0, n_colors - 1);

    Guess solution;
	for (auto& color : solution)
		color = uniform_dist(engine);
	return solution;
}
