#ifndef CHALLENGER_HPP
#define CHALLENGER_HPP

#include <vector>
#include "mastermind.hpp"

/// Main class for challengers.
class Challenger {

public:
    /// Constructor.
    /// \param The number of challengers in total
    /// \param rank The rank of this challenger, starting from zero (thus not
    /// counting the master).
    Challenger(int n_challengers, int rank);

    /// Main loop function.
    void main();

private:
    /// Assigns to the challenger its search space. This is done by generating
    /// all possible combination of colors, removing all that are not legal
    /// (containing mutliple times the same color) and taking a subset of it,
    /// determined by the rank of the challenger.
    /// \param The number of challengers in total
    void set_search_space(int n_challengers);

    /// Sends a guess randomly chosen from the search space to the master.
    /// If the search space is empty, sends² the special guess containing only
    /// zeroes.
    void send_guess() const;

    /// Receives the evaluation from the master, and updates the search space by
    /// removing all possibilities that are not plausible.
    bool receive_evaluation();

    /// Returns true iff the guess has no duplicate colors.
    static bool is_legal(const Guess& guess);

    /// Returns true iff the guess is not plausible given the evaluation of
    /// evaluated_guess. We consider that the guess is not plausible if its
    /// evaluation with evaluated_guess is different from the given evaluation.
    static bool is_not_plausible(const Guess& guess, const Guess& evaluated_guess, const Evaluation& evaluation);

    /// The rank of this challenger, starting from zero.
    const int _rank;

    /// The set of all plausible guesses so far.
    std::vector<Guess> _search_space;
};

#endif // CHALLENGER_HPP
