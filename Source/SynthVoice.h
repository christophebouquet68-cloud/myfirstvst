#pragma once
#include <JuceHeader.h>
#include "Oscillator.h"
#include "NoiseGenerator.h"
#include "Filter.h"
#include "Lfo.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    // Called once before playback begins
    void prepareToPlay(double sampleRate, int /*samplesPerBlock*/);

    // --- Oscillator 1 ---
    void setOsc1Waveform(WaveformType t)    { osc1Waveform = t; }
    void setOsc1Octave(int oct)             { osc1Octave = oct; }
    void setOsc1Detune(float cents)         { osc1Detune = cents; }
    void setOsc1Level(float lvl)            { osc1Level = lvl; }
    void setOsc1PulseWidth(float pw)        { osc1.setPulseWidth(pw); }

    // --- Oscillator 2 ---
    void setOsc2Waveform(WaveformType t)    { osc2Waveform = t; }
    void setOsc2Octave(int oct)             { osc2Octave = oct; }
    void setOsc2Detune(float cents)         { osc2Detune = cents; }
    void setOsc2Level(float lvl)            { osc2Level = lvl; }
    void setOsc2PulseWidth(float pw)        { osc2.setPulseWidth(pw); }

    // --- Sub / Noise ---
    void setSubLevel(float lvl)             { subLevel = lvl; }
    void setNoiseLevel(float lvl)           { noiseLevel = lvl; }

    // --- Filter ---
    void setFilterCutoff(float hz)          { baseCutoff = hz; }
    void setFilterResonance(float res)      { filter.setResonance(res); }

    // --- ADSR ---
    void setAdsrParams(const juce::ADSR::Parameters& p) { adsrParams = p; adsr.setParameters(p); }

    // --- LFO ---
    void setLfoRate(float hz)               { lfo.setRate(hz); }
    void setLfoDepth(float d)               { lfo.setDepth(d); }
    void setLfoTarget(LfoTarget t)          { lfo.setTarget(t); }

    // SynthesiserVoice interface
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                         int startSample, int numSamples) override;

private:
    void updateOscillatorFrequencies(float lfoMod = 0.0f);

    // DSP objects
    Oscillator      osc1, osc2, subOsc;
    NoiseGenerator  noiseGen;
    Filter          filter;
    juce::ADSR      adsr;
    juce::ADSR::Parameters adsrParams;
    Lfo             lfo;

    // Voice state
    double sampleRate    = 44100.0;
    float  midiFrequency = 440.0f;
    float  noteVelocity  = 1.0f;
    bool   isPlaying     = false;

    // Mixer levels (0..1)
    float osc1Level  = 0.8f;
    float osc2Level  = 0.0f;
    float subLevel   = 0.0f;
    float noiseLevel = 0.0f;

    // Oscillator params
    WaveformType osc1Waveform = WaveformType::Sawtooth;
    WaveformType osc2Waveform = WaveformType::Sawtooth;
    int   osc1Octave  = 0;
    int   osc2Octave  = 0;
    float osc1Detune  = 0.0f; // cents
    float osc2Detune  = 0.0f; // cents

    // Filter state
    float baseCutoff = 20000.0f;
};
