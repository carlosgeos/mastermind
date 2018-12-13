#ifndef CHALLENGER_HPP
#define CHALLENGER_HPP

#include <vector>
#include "mastermind.hpp"

void challenger_main(int n_challengers, int rank);

std::vector<Guess> get_search_space(int n_challengers, int rank);

bool is_legal(const Guess& guess);


void send_guess(const std::vector<Guess>& search_space, int rank);

bool receive_evaluation(std::vector<Guess>& search_space, int rank);

bool is_not_plausible(const Guess& guess, const Guess& evaluated_guess, const Evaluation& evaluation);

#endif // CHALLENGER_HPP
