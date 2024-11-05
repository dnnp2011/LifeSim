#pragma once

#include <iostream>
#include <random>


template<typename T> class Random {
public:
    static T generate(T min, T max) {
        if (min > max)
            std::swap(min, max);

        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> distribution(min, max);

            return distribution(generator);
        } else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> distribution(min, max);

            return distribution(generator);
        }
    }

private:
    static std::mt19937 generator;
};

template<typename T> std::mt19937 Random<T>::generator(std::random_device{}());

template class Random<int>;
template class Random<float>;
template class Random<double>;
