#include "SensorSimulator.hpp"

#include <random>

Measurement SensorSimulator::generate()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> tempDist(20.0f, 35.0f);
    std::uniform_real_distribution<float> humDist(30.0f, 90.0f);

    return {
        tempDist(gen),
        humDist(gen)
    };
}