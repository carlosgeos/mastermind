#include <algorithm>
#include "mastermind.hpp"

Evaluation evaluate(const Guess& guess, const Guess& solution) {
    Evaluation result{0, 0};
    for (std::size_t i{0}; i < guess.size(); ++i) {
        // If the guess had it right at the spot
        if (guess[i] == solution[i])
            ++result.perfect;
        // If the guess spot is in the solution but elsewhere
        else if (std::find(solution.begin(), solution.end(), guess[i]) != solution.end()) {
            ++result.color_only;
        }
    }
    return result;
}
