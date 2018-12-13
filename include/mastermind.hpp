#ifndef MASTERMIND_HPP
#define MASTERMIND_HPP

#include <cstddef>
#include <array>

constexpr static int n_colors{10};
constexpr static std::size_t n_spots{4};

typedef int Color;

typedef std::array<Color, n_spots> Guess;

struct Evaluation {
    int color_only;
    int perfect;
};

Evaluation evaluate(Guess guess, Guess solution);

#endif // MASTERMIND_HPP
