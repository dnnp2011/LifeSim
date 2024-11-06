#pragma once

#include <iostream>
#include <random>


/**
* Why does this Random class implementation cause the program to freeze in ECSManager::ECSManager()?
*/
// template<typename T>
// class Random {
// public:
//     static T generate(T min, T max) {
//         if (min > max)
//             std::swap(min, max);
//
//         if constexpr (std::is_integral_v<T>) {
//             std::uniform_int_distribution<T> distribution(min, max);
//
//             return distribution(generator);
//         } else if constexpr (std::is_floating_point_v<T>) {
//             std::uniform_real_distribution<T> distribution(min, max);
//
//             return distribution(generator);
//         } else if constexpr (std::is_enum_v<T>) {
//             std::uniform_int_distribution<int> distribution(static_cast<int>(min), static_cast<int>(max));
//
//             return static_cast<T>(distribution(generator));
//         }
//     }
//
// private:
//     static std::mt19937_64 generator;
// };
//
// template<typename T>
// inline std::mt19937_64 Random<T>::generator(std::random_device{}());


/**
 * Generate Random Numeric Values<int, float, double, ShapeType>
 *
 * This works, but it's probably quite inefficient as a new instance is instantiated multiple times.
 * I had silent failures when using static members, so I opted for this approach.
 */
template<typename T>
class Random {
public:
    Random(): random_device(std::random_device{}), generator(random_device()) {}

    T generate(T min, T max) {
        if (min > max)
            std::swap(min, max);

        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> distribution(min, max);

            return distribution(generator);
        } else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> distribution(min, max);

            return distribution(generator);
        } else if constexpr (std::is_enum_v<T>) {
            std::uniform_int_distribution<int> distribution(static_cast<int>(min), static_cast<int>(max));

            return static_cast<T>(distribution(generator));
        }
    }

private:
    std::random_device random_device;
    std::mt19937_64 generator;
};

template class Random<int>;
template class Random<float>;
template class Random<double>;
template class Random<ShapeType>;


/**
 * Compare Random performance with RandomNumberGenerator
 */
class RandomNumberGenerator {
public:
    virtual ~RandomNumberGenerator() = default; // Virtual destructor for proper cleanup

    virtual int generateInt(int min, int max) = 0;

    virtual float generateFloat(float min, float max) = 0;

    virtual double generateDouble(double min, double max) = 0;

    virtual ShapeType generateShapeType(ShapeType min, ShapeType max) = 0;
};

class UniformRandomGenerator : public RandomNumberGenerator {
private:
    std::mt19937_64 generator;
    std::uniform_int_distribution<int> intDistribution;
    std::uniform_real_distribution<float> floatDistribution;
    std::uniform_real_distribution<double> doubleDistribution;

public:
    explicit UniformRandomGenerator(int seed = static_cast<int>(time(nullptr))) : generator(seed) {} // Constructor for seeding the generator

    int generateInt(int min, int max) override {
        intDistribution.param(std::uniform_int_distribution<int>::param_type(min, max));

        return intDistribution(generator);
    }

    float generateFloat(float min, float max) override {
        floatDistribution.param(std::uniform_real_distribution<float>::param_type(min, max));

        return floatDistribution(generator);
    }

    double generateDouble(double min, double max) override {
        doubleDistribution.param(std::uniform_real_distribution<double>::param_type(min, max));

        return doubleDistribution(generator);
    }

    ShapeType generateShapeType(ShapeType min = static_cast<ShapeType>(0), ShapeType max = ShapeType::COUNT) override {
        intDistribution.param(std::uniform_int_distribution<int>::param_type(static_cast<int>(min), static_cast<int>(max)));

        return static_cast<ShapeType>(intDistribution(generator));
    }
};
