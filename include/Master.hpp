#ifndef MASTER_HPP
#define MASTER_HPP

#include <vector>
#include "mastermind.hpp"

/// Class for the master node.
class Master {

public:
    /// Constructor.
    /// \param The number of challengers in total
    Master(Color n_colors, std::size_t n_spots, int n_challengers);

    /// Main loop of the master.
    void main() const;

private:
    /// Creates a random solution and returns it.
    Guess pick_random_solution();

    /// Receives the guesses from all the challengers, strips out the null ones
    /// (containing only zeroes) and return them.
    std::vector<Guess> receive_guesses() const;

    /// Evaluates a guess and broadcast it, along with the evaluation, to all
    /// challengers.
    bool send_evaluation(const Guess& picked_guess) const;

    /// The number of different colors
    const Color _n_colors;

    /// The number of spots in a solution or guess
    const std::size_t _n_spots;

    /// The total number of challengers.
    const int _n_challengers;

    /// The solution of the game.
    const Guess _solution;
};

#endif // MASTER_HPP
