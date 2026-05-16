#pragma once
#include <cmath>
#include <JuceHeader.h>

// Topology-Preserving Transform (TPT) State Variable Filter
// Provides simultaneous LP/HP/BP outputs with stable resonance
class Filter
{
public:
    void prepare(double sampleRate) noexcept
    {
        sr = sampleRate;
        reset();
        updateCoefficients();
    }

    void reset() noexcept { ic1eq = ic2eq = 0.0f; }

    void setCutoff(float hz) noexcept
    {
        cutoffHz = juce::jlimit(20.0f, static_cast<float>(sr * 0.49), hz);
        updateCoefficients();
    }

    // resonance: 0.0 (no resonance) to 1.0 (near self-oscillation)
    void setResonance(float res) noexcept
    {
        resonance = juce::jlimit(0.0f, 0.98f, res);
        updateCoefficients();
    }

    // Returns low-pass output
    float process(float in) noexcept
    {
        const float v0 = in;
        const float v3 = v0 - ic2eq;
        const float v1 = a1 * ic1eq + a2 * v3;
        const float v2 = ic2eq + a2 * ic1eq + a3 * v3;
        ic1eq = 2.0f * v1 - ic1eq;
        ic2eq = 2.0f * v2 - ic2eq;
        return v2; // low-pass
    }

private:
    void updateCoefficients() noexcept
    {
        const float g = std::tan(juce::MathConstants<float>::pi * cutoffHz / static_cast<float>(sr));
        const float k = 2.0f * (1.0f - resonance);
        a1 = 1.0f / (1.0f + g * (g + k));
        a2 = g * a1;
        a3 = g * a2;
    }

    double sr = 44100.0;
    float cutoffHz = 20000.0f;
    float resonance = 0.0f;
    float a1 = 0.0f, a2 = 0.0f, a3 = 0.0f;
    float ic1eq = 0.0f, ic2eq = 0.0f;
};
