#ifndef MASTER_HPP
#define MASTER_HPP

#include <vector>
#include "mastermind.hpp"

class Master {

public:
    Master(int n_challengers);

    void main() const;

private:
    static Guess pick_random_solution();

    std::vector<Guess> receive_guesses() const;

    bool send_evaluation(Guess picked_guess) const;

    const int _n_challengers;
    const Guess _solution;
};

#endif // MASTER_HPP
