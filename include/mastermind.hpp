#ifndef MASTERMIND_HPP
#define MASTERMIND_HPP

#include <cstddef>
#include <array>

constexpr static std::size_t n_colors{10};
constexpr static std::size_t n_spots{4};

typedef int Color;

typedef std::array<Color, n_spots> Guess;

struct Evaluation {
	std::size_t colorOnly;
	std::size_t perfect;
};

#endif // MASTERMIND_HPP
