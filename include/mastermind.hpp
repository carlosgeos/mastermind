#ifndef MASTERMIND_HPP
#define MASTERMIND_HPP

#include <cstddef>
#include <array>

/// A color is just represented by an int.
typedef int Color;

/// The number of different colors
constexpr static Color n_colors{10};

/// The number of spots in a solution or guess
constexpr static std::size_t n_spots{4};

/// Holds a guess or a solution
typedef std::array<Color, n_spots> Guess;

/// Data about the evaluation of a guess
struct Evaluation {
    /// The number of color present in both the guess and the solution, but
    /// in different places
    int color_only;

    /// The number of identical spots in the guess and the solution.
    int perfect;
};

/// Evaluates a guess given the real solution.
Evaluation evaluate(Guess guess, Guess solution);

#endif // MASTERMIND_HPP
