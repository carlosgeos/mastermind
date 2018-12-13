#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
#include <vector>

/// Randomly sample an element from a vector.
template <typename T>
T pick_sample(const std::vector<T>& values) {
    std::uniform_int_distribution<> dis(0, values.size() - 1);
    // The random device is created each time, so the randomness is solely
    // dependent on the current time, but it's pseudorandom enough for our purposes.
    static std::random_device rd;
    static std::mt19937 gen(rd());
    T picked_sample{values.at(dis(gen))};
	return picked_sample;
}

#endif // UTILS_HPP
