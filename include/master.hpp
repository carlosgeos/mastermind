#ifndef MASTER_HPP
#define MASTER_HPP

#include <vector>
#include "mastermind.hpp"

void master_main(int n_challengers);

Guess pick_random_solution();

std::vector<Guess> receive_guesses(int n_challengers);

bool send_evaluation(Guess picked_guess, Guess solution);

#endif // MASTER_HPP
