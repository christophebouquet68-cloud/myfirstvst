#include "SynthVoice.h"
#include "SynthSound.h"

static float centsToRatio(float cents) noexcept
{
    return std::pow(2.0f, cents / 1200.0f);
}

void SynthVoice::prepareToPlay(double sr, int /*samplesPerBlock*/)
{
    sampleRate = sr;
    osc1.prepare(sr);
    osc2.prepare(sr);
    subOsc.prepare(sr);
    filter.prepare(sr);
    lfo.prepare(sr);
    adsr.setSampleRate(sr);
    adsr.setParameters(adsrParams);
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity,
                            juce::SynthesiserSound*, int)
{
    midiFrequency = static_cast<float>(
        juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    noteVelocity = velocity;
    isPlaying = true;

    osc1.setWaveform(osc1Waveform);
    osc2.setWaveform(osc2Waveform);
    subOsc.setWaveform(WaveformType::Square);

    osc1.reset();
    osc2.reset();
    subOsc.reset();
    lfo.reset();

    updateOscillatorFrequencies();
    adsr.noteOn();
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        adsr.reset();
        clearCurrentNote();
        isPlaying = false;
    }
}

void SynthVoice::updateOscillatorFrequencies(float lfoMod)
{
    // LFO pitch modulation: lfoMod in [-1,1] maps to ±1 semitone * depth
    const float pitchRatio = (lfo.getTarget() == LfoTarget::Pitch)
                             ? std::pow(2.0f, lfoMod / 12.0f)
                             : 1.0f;

    const float f1 = midiFrequency
                     * std::pow(2.0f, static_cast<float>(osc1Octave))
                     * centsToRatio(osc1Detune)
                     * pitchRatio;

    const float f2 = midiFrequency
                     * std::pow(2.0f, static_cast<float>(osc2Octave))
                     * centsToRatio(osc2Detune)
                     * pitchRatio;

    const float fSub = midiFrequency * 0.5f * pitchRatio; // one octave below osc1

    osc1.setFrequency(f1);
    osc2.setFrequency(f2);
    subOsc.setFrequency(fSub);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                  int startSample, int numSamples)
{
    if (!isPlaying && !adsr.isActive())
        return;

    for (int i = 0; i < numSamples; ++i)
    {
        const float lfoSample = lfo.getNextSample();

        // Update oscillator frequencies each sample for pitch LFO
        if (lfo.getTarget() == LfoTarget::Pitch)
            updateOscillatorFrequencies(lfoSample);

        // Mixer
        const float mixed = osc1.getNextSample() * osc1Level
                          + osc2.getNextSample() * osc2Level
                          + subOsc.getNextSample() * subLevel
                          + noiseGen.getNextSample() * noiseLevel;

        // Filter cutoff modulation
        float cutoff = baseCutoff;
        if (lfo.getTarget() == LfoTarget::FilterCutoff)
            cutoff *= std::pow(2.0f, lfoSample * 4.0f); // ±4 octaves of sweep
        filter.setCutoff(juce::jlimit(20.0f, 20000.0f, cutoff));

        // VCF
        const float filtered = filter.process(mixed);

        // VCA: ADSR envelope + velocity
        const float envGain  = adsr.getNextSample();
        float amplitude = (lfo.getTarget() == LfoTarget::Amplitude)
                          ? envGain * (1.0f + lfoSample) * 0.5f
                          : envGain;
        amplitude *= noteVelocity;

        const float out = filtered * amplitude;

        for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
            outputBuffer.addSample(ch, startSample + i, out);
    }

    // Release voice when envelope is done
    if (!adsr.isActive())
    {
        clearCurrentNote();
        isPlaying = false;
    }
}
