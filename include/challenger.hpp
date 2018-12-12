#ifndef CHALLENGER_HPP
#define CHALLENGER_HPP

#include <vector>
#include "mastermind.hpp"

void challenger_main(int n_challengers, int rank);

std::vector<Guess> get_search_space(int n_challengers, int rank);

bool is_legal(Guess guess);

#endif // CHALLENGER_HPP
