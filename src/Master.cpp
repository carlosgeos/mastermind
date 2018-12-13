#include <random>
#include <iostream>
#include <algorithm>
#include <mpi.h>
#include "Master.hpp"
#include "mastermind.hpp"

Master::Master(int n_challengers):
    _n_challengers{n_challengers},
    _solution{pick_random_solution()}
{
}

void Master::main() const {
    std::cout << "[M] Solution: ";
    for (auto& color : _solution)
        std::cout << color << " ";
    std::cout << std::endl;

    bool solved{false};
    while (not solved) {
        // Receive message from all challengers
        std::vector<Guess> guesses{receive_guesses()};

        if (not guesses.empty()) {
            // Pick a random guess
            std::uniform_int_distribution<> dis(0, guesses.size() - 1);
            static std::random_device rd;
            static std::mt19937 gen(rd());
            Guess random_guess{guesses.at(dis(gen))};

            solved = send_evaluation(random_guess);
        }
    }
}

Guess Master::pick_random_solution() {
    std::array<Color, n_colors> colors;
    std::iota(colors.begin(), colors.end(), 0);
    std::shuffle(colors.begin(), colors.end(), std::mt19937{std::random_device{}()});
    Guess solution;
    std::copy_n(colors.begin(), n_spots, solution.begin());
    return solution;
}

std::vector<Guess> Master::receive_guesses() const {
    std::vector<Guess> guesses(_n_challengers + 1);
    Guess dummy_guess;
    dummy_guess.fill(0);
    // Receive guesses from all challengers
    MPI_Gather(dummy_guess.data(), n_spots, MPI_INT, guesses.data(), n_spots, MPI_INT, 0, MPI_COMM_WORLD);

    // Transform into guesses, filtering out zero guesses (meaning the search
    // space of the challenger is empty)
    std::vector<Guess> res;
    for(const auto& guess : guesses) {
        // If the guess is not all zeroes
        if (std::count(guess.begin(), guess.end(), 0) != n_spots)
            res.push_back(guess);
    }
    return res;
}

bool Master::send_evaluation(Guess picked_guess) const {
    Evaluation evaluation{evaluate(picked_guess, _solution)};
    std::cout << "[M] Broadcasting evaluation of ";
    for (auto& color : picked_guess)
        std::cout << color << " ";
    std::cout << " => color_only = " << evaluation.color_only
              << "; perfect = " << evaluation.perfect << std::endl;

    // Broadcast evaluation to all challengers
    std::array<Color, n_spots + 2> evaluation_data;
    std::copy(picked_guess.begin(), picked_guess.end(), evaluation_data.begin());
    evaluation_data[n_spots] = evaluation.color_only;
    evaluation_data[n_spots + 1] = evaluation.perfect;
    MPI_Bcast(evaluation_data.data(), n_spots + 2, MPI_INT, 0, MPI_COMM_WORLD);

    return evaluation.perfect == n_spots;
}
