#pragma once

#include "SmoothedValue.h"

class DegradeNoise
{
public:
    DegradeNoise() = default;
    ~DegradeNoise() = default;

    void setGain (float newGain)
    {
        gain.setTargetValue(newGain);
    }

    void prepare (float sampleRate)
    {
        gain.reset((double) sampleRate, 0.05);
    }

    inline float processSample(float x)
    {
        return x + ((((float) rand() / (RAND_MAX))) - 0.5f) * gain.getNextValue();
    }

private:
    SmoothedValue<float, ValueSmoothingTypes::Linear> gain;
};
