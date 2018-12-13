#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
#include <vector>

template <typename T>
T pick_sample(const std::vector<T>& values) {
    std::uniform_int_distribution<> dis(0, values.size() - 1);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    T picked_sample{values.at(dis(gen))};
	return picked_sample;
}

#endif // UTILS_HPP
