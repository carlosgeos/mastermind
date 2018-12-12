#ifndef MASTER_HPP
#define MASTER_HPP

#include "mastermind.hpp"

void master_main(int n_challengers);

Guess pick_random_solution();


Evaluation evaluate(Guess guess, Guess solution);

#endif // MASTER_HPP
