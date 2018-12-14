#include <random>
#include <iostream>
#include <algorithm>
#include <mpi.h>
#include "Master.hpp"
#include "utils.hpp"
#include "mastermind.hpp"

Master::Master(Color n_colors, std::size_t n_spots, int n_challengers):
    _n_colors{n_colors},
    _n_spots{n_spots},
    _n_challengers{n_challengers},
    _solution{pick_random_solution()}
{
}

void Master::main() const {
    bool solved{false};
    int i{0};
    while (not solved) {
        // Receive message from all challengers
        std::vector<Guess> guesses{receive_guesses()};

        if (not guesses.empty()) {
            // Pick a random guess
            Guess random_guess{pick_sample(guesses)};
            solved = send_evaluation(random_guess);
        }
        i++;
    }
    std::cout << "Turns played: " << i << std::endl;
}

Guess Master::pick_random_solution() {
    std::vector<Color> colors(_n_colors);
    std::iota(colors.begin(), colors.end(), 0);
    std::shuffle(colors.begin(), colors.end(), std::mt19937{std::random_device{}()});
    Guess solution(colors.begin(), colors.begin() + _n_spots);


    return solution;
}

std::vector<Guess> Master::receive_guesses() const {
    std::vector<Color> colors((_n_challengers + 1) * _n_spots);
    Guess dummy_guess(_n_spots, 0);
    // Receive guesses from all challengers
    MPI_Gather(dummy_guess.data(), _n_spots, MPI_INT, colors.data(), _n_spots, MPI_INT, 0, MPI_COMM_WORLD);
    // Transform into guesses, filtering out zero guesses (meaning the search
    // space of the challenger is empty)
    std::vector<Guess> res;
    for(int i{0}; i < _n_challengers + 1; ++i) {
        Guess guess;
        std::copy_n(colors.begin() + (i * _n_spots), _n_spots, std::back_inserter(guess));
        // If the guess is not all zeroes
        if (std::count(guess.begin(), guess.end(), 0) != static_cast<int>(_n_spots))
            res.push_back(guess);
    }
    return res;
}

bool Master::send_evaluation(const Guess& picked_guess) const {
    Evaluation evaluation{evaluate(picked_guess, _solution)};
    std::cout << "Sending" << "\n";
    std::cout << "[M] Broadcasting evaluation of ";
    for (auto& color : picked_guess)
        std::cout << color << " ";
    std::cout << "\n * color_only = " << evaluation.color_only
              << "\n * perfect    = " << evaluation.perfect << std::endl;

    // Broadcast evaluation to all challengers
    std::vector<Color> evaluation_data(picked_guess.begin(), picked_guess.end());
    evaluation_data.push_back(evaluation.color_only);
    evaluation_data.push_back(evaluation.perfect);
    MPI_Bcast(evaluation_data.data(), evaluation_data.size(), MPI_INT, 0, MPI_COMM_WORLD);

    return evaluation.perfect == static_cast<int>(_n_spots);
}
