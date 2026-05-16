#pragma once
#include <JuceHeader.h>

class NoiseGenerator
{
public:
    float getNextSample() noexcept
    {
        return rng.nextFloat() * 2.0f - 1.0f;
    }

private:
    juce::Random rng;
};
