#include <iostream>
#include <algorithm>
#include <functional>
#include <mpi.h>
#include "utils.hpp"
#include "Challenger.hpp"


Challenger::Challenger(Color n_colors, std::size_t n_spots, int n_challengers, int rank):
    _n_colors{n_colors},
    _n_spots{n_spots},
    _rank{rank}
{
    set_search_space(n_challengers);
}

void Challenger::main() {
    bool solved{false};
    while (not solved) {
        send_guess();
        std::cout << _rank << ": search space size " << _search_space.size() << std::endl;
        solved = receive_evaluation();

    }
}

void Challenger::set_search_space(int n_challengers) {
    std::size_t max_guess{1};
    for (std::size_t i{0}; i < _n_spots; ++i) {
        max_guess *= _n_colors;
    }

    std::vector<Guess> guesses;
    Guess current(_n_spots, 0);

    for (std::size_t i{0}; i < max_guess; ++i) {
        if (is_legal(current))
            guesses.push_back(current);
        current[_n_spots - 1] += 1;
        std::size_t carry{_n_spots - 1};
        while (current[carry] >= _n_colors and carry > 0) {
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
    for (std::size_t i{0}; i < guess.size(); ++i) {
        for (std::size_t j{i + 1}; j < guess.size(); ++j) {
            if (guess[i] == guess[j])
                return false;
        }
    }
    return true;
}

void Challenger::send_guess() const {
    // Fill with zeroes by default
    Guess guess(_n_spots, 0);

    if(not _search_space.empty())
        guess = pick_sample(_search_space);

    MPI_Gather(guess.data(), _n_spots, MPI_INT, nullptr, _n_spots, MPI_INT, 0, MPI_COMM_WORLD);
}

bool Challenger::receive_evaluation() {
    std::vector<Color> evaluation_data(_n_spots + 2);
    MPI_Bcast(evaluation_data.data(), _n_spots + 2, MPI_INT, 0, MPI_COMM_WORLD);

    Evaluation evaluation{evaluation_data[_n_spots], evaluation_data[_n_spots + 1]};
    Guess evaluated_guess(evaluation_data.begin(), evaluation_data.begin() + _n_spots);

    if (evaluation.perfect == static_cast<int>(_n_spots))
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
    return difference.perfect != evaluation.perfect
            or difference.color_only != evaluation.color_only;
}
