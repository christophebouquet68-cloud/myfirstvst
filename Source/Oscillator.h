#pragma once
#include <cmath>

enum class WaveformType { Sawtooth = 0, Square, Triangle, Pulse };

// Band-limited oscillator using PolyBLEP correction
class Oscillator
{
public:
    void prepare(double sampleRate) noexcept
    {
        sr = sampleRate;
        reset();
    }

    void reset() noexcept { phase = 0.0f; }

    void setFrequency(float hz) noexcept
    {
        phaseDelta = hz / static_cast<float>(sr);
    }

    void setWaveform(WaveformType t) noexcept { waveform = t; }
    void setPulseWidth(float pw) noexcept { pulseWidth = juce::jlimit(0.05f, 0.95f, pw); }

    float getNextSample() noexcept
    {
        float sample = 0.0f;

        switch (waveform)
        {
            case WaveformType::Sawtooth:
                sample = 2.0f * phase - 1.0f;
                sample -= polyBlep(phase, phaseDelta);
                break;

            case WaveformType::Square:
                sample = phase < 0.5f ? 1.0f : -1.0f;
                sample += polyBlep(phase, phaseDelta);
                sample -= polyBlep(std::fmod(phase + 0.5f, 1.0f), phaseDelta);
                break;

            case WaveformType::Triangle:
            {
                // Integrate square wave for triangle
                float sq = phase < 0.5f ? 1.0f : -1.0f;
                sq += polyBlep(phase, phaseDelta);
                sq -= polyBlep(std::fmod(phase + 0.5f, 1.0f), phaseDelta);
                triAccum += 4.0f * phaseDelta * sq;
                triAccum *= 0.999f; // DC leak
                sample = triAccum;
                break;
            }

            case WaveformType::Pulse:
                sample = phase < pulseWidth ? 1.0f : -1.0f;
                sample += polyBlep(phase, phaseDelta);
                sample -= polyBlep(std::fmod(phase + (1.0f - pulseWidth), 1.0f), phaseDelta);
                // Compensate DC offset
                sample -= (2.0f * pulseWidth - 1.0f);
                break;
        }

        phase += phaseDelta;
        if (phase >= 1.0f) phase -= 1.0f;

        return sample;
    }

private:
    static float polyBlep(float t, float dt) noexcept
    {
        if (t < dt)
        {
            t /= dt;
            return t + t - t * t - 1.0f;
        }
        if (t > 1.0f - dt)
        {
            t = (t - 1.0f) / dt;
            return t * t + t + t + 1.0f;
        }
        return 0.0f;
    }

    double sr = 44100.0;
    float phase = 0.0f;
    float phaseDelta = 0.0f;
    float pulseWidth = 0.5f;
    float triAccum = 0.0f;
    WaveformType waveform = WaveformType::Sawtooth;
};
