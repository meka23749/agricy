#pragma once

struct Measurement
{
    float temperature;
    float humidity;
};

class SensorSimulator
{
public:
    Measurement generate();
};