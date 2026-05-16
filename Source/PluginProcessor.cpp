#include "PluginProcessor.h"
#include "PluginEditor.h"

MyFirstVSTProcessor::MyFirstVSTProcessor()
    : AudioProcessor(BusesProperties()
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    for (int i = 0; i < NUM_VOICES; ++i)
        synth.addVoice(new SynthVoice());

    synth.addSound(new SynthSound());
}

juce::AudioProcessorValueTreeState::ParameterLayout
MyFirstVSTProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Oscillator 1
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "osc1_waveform", "Osc1 Waveform",
        juce::StringArray{"Sawtooth", "Square", "Triangle", "Pulse"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "osc1_octave", "Osc1 Octave", -2, 2, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc1_detune", "Osc1 Detune",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc1_level", "Osc1 Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc1_pw", "Osc1 Pulse Width",
        juce::NormalisableRange<float>(0.05f, 0.95f, 0.01f), 0.5f));

    // Oscillator 2
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "osc2_waveform", "Osc2 Waveform",
        juce::StringArray{"Sawtooth", "Square", "Triangle", "Pulse"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "osc2_octave", "Osc2 Octave", -2, 2, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc2_detune", "Osc2 Detune",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc2_level", "Osc2 Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "osc2_pw", "Osc2 Pulse Width",
        juce::NormalisableRange<float>(0.05f, 0.95f, 0.01f), 0.5f));

    // Sub / Noise
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "sub_level", "Sub Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "noise_level", "Noise Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    // Filter
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filter_cutoff", "Filter Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 20000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filter_resonance", "Filter Resonance",
        juce::NormalisableRange<float>(0.0f, 0.98f, 0.01f), 0.0f));

    // ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "attack",  "Attack",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f), 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "decay",   "Decay",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "sustain", "Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "release", "Release",
        juce::NormalisableRange<float>(0.001f, 10.0f, 0.001f, 0.5f), 0.3f));

    // LFO
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo_rate", "LFO Rate",
        juce::NormalisableRange<float>(0.1f, 20.0f, 0.01f, 0.5f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo_depth", "LFO Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "lfo_target", "LFO Target",
        juce::StringArray{"Pitch", "Filter", "Amplitude"}, 0));

    return { params.begin(), params.end() };
}

void MyFirstVSTProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            v->prepareToPlay(sampleRate, samplesPerBlock);

    updateVoices();
}

void MyFirstVSTProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    updateVoices();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void MyFirstVSTProcessor::updateVoices()
{
    const auto waveformIndex = [](auto* p) -> WaveformType {
        return static_cast<WaveformType>(static_cast<int>(*p));
    };
    const auto lfoTargetIndex = [](auto* p) -> LfoTarget {
        return static_cast<LfoTarget>(static_cast<int>(*p));
    };

    auto* osc1Wf   = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("osc1_waveform"));
    auto* osc1Oct  = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter("osc1_octave"));
    auto* osc2Wf   = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("osc2_waveform"));
    auto* osc2Oct  = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter("osc2_octave"));
    auto* lfoTgt   = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("lfo_target"));

    juce::ADSR::Parameters adsrP;
    adsrP.attack  = *apvts.getRawParameterValue("attack");
    adsrP.decay   = *apvts.getRawParameterValue("decay");
    adsrP.sustain = *apvts.getRawParameterValue("sustain");
    adsrP.release = *apvts.getRawParameterValue("release");

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        auto* v = dynamic_cast<SynthVoice*>(synth.getVoice(i));
        if (v == nullptr) continue;

        // Oscillator 1
        if (osc1Wf)  v->setOsc1Waveform(waveformIndex(osc1Wf));
        if (osc1Oct) v->setOsc1Octave(*osc1Oct);
        v->setOsc1Detune(*apvts.getRawParameterValue("osc1_detune"));
        v->setOsc1Level(*apvts.getRawParameterValue("osc1_level"));
        v->setOsc1PulseWidth(*apvts.getRawParameterValue("osc1_pw"));

        // Oscillator 2
        if (osc2Wf)  v->setOsc2Waveform(waveformIndex(osc2Wf));
        if (osc2Oct) v->setOsc2Octave(*osc2Oct);
        v->setOsc2Detune(*apvts.getRawParameterValue("osc2_detune"));
        v->setOsc2Level(*apvts.getRawParameterValue("osc2_level"));
        v->setOsc2PulseWidth(*apvts.getRawParameterValue("osc2_pw"));

        // Sub / Noise
        v->setSubLevel(*apvts.getRawParameterValue("sub_level"));
        v->setNoiseLevel(*apvts.getRawParameterValue("noise_level"));

        // Filter
        v->setFilterCutoff(*apvts.getRawParameterValue("filter_cutoff"));
        v->setFilterResonance(*apvts.getRawParameterValue("filter_resonance"));

        // ADSR
        v->setAdsrParams(adsrP);

        // LFO
        v->setLfoRate(*apvts.getRawParameterValue("lfo_rate"));
        v->setLfoDepth(*apvts.getRawParameterValue("lfo_depth"));
        if (lfoTgt) v->setLfoTarget(lfoTargetIndex(lfoTgt));
    }
}

void MyFirstVSTProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MyFirstVSTProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyFirstVSTProcessor();
}
