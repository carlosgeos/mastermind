#ifndef MASTERMIND_HPP
#define MASTERMIND_HPP

#include <cstddef>
#include <array>

/// A color is just represented by an int.
typedef int Color;

/// Holds a guess or a solution
typedef std::vector<Color> Guess;

/// Data about the evaluation of a guess
struct Evaluation {
    /// The number of color present in both the guess and the solution, but
    /// in different places
    int color_only;

    /// The number of identical spots in the guess and the solution.
    int perfect;
};

/// Evaluates a guess given the real solution.
Evaluation evaluate(const Guess& guess, const Guess& solution);

#endif // MASTERMIND_HPP
