#ifndef CHALLENGER_HPP
#define CHALLENGER_HPP

#include <vector>
#include "mastermind.hpp"

class Challenger {

public:
	Challenger(int n_challengers, int rank);

	void main();

private:
	void set_search_space(int n_challengers);

	void send_guess() const;

	bool receive_evaluation();

	static bool is_legal(const Guess& guess);

	static bool is_not_plausible(const Guess& guess, const Guess& evaluated_guess, const Evaluation& evaluation);

	const int _rank;
	std::vector<Guess> _search_space;
};

#endif // CHALLENGER_HPP
