#pragma once
#include <cmath>
#include <JuceHeader.h>

enum class LfoTarget { Pitch = 0, FilterCutoff, Amplitude };

class Lfo
{
public:
    void prepare(double sampleRate) noexcept
    {
        sr = sampleRate;
        reset();
    }

    void reset() noexcept { phase = 0.0f; }

    void setRate(float hz) noexcept
    {
        phaseDelta = hz / static_cast<float>(sr);
    }

    void setDepth(float d) noexcept { depth = juce::jlimit(0.0f, 1.0f, d); }
    void setTarget(LfoTarget t) noexcept { target = t; }
    LfoTarget getTarget() const noexcept { return target; }

    // Returns modulation value in [-1, 1] scaled by depth
    float getNextSample() noexcept
    {
        const float out = depth * std::sin(juce::MathConstants<float>::twoPi * phase);
        phase += phaseDelta;
        if (phase >= 1.0f) phase -= 1.0f;
        return out;
    }

private:
    double sr = 44100.0;
    float phase = 0.0f;
    float phaseDelta = 0.0f;
    float depth = 0.0f;
    LfoTarget target = LfoTarget::Pitch;
};
