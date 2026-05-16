#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

// Reusable labelled knob
struct LabelledKnob : public juce::Component
{
    juce::Slider slider { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Label  label;

    LabelledKnob() { addAndMakeVisible(slider); addAndMakeVisible(label); }

    void setup(const juce::String& name)
    {
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setFont(juce::Font(11.0f));
    }

    void resized() override
    {
        auto r = getLocalBounds();
        label.setBounds(r.removeFromBottom(16));
        slider.setBounds(r);
    }
};

class MyFirstVSTEditor : public juce::AudioProcessorEditor
{
public:
    explicit MyFirstVSTEditor(MyFirstVSTProcessor&);
    ~MyFirstVSTEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MyFirstVSTProcessor& processor;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    // Osc 1
    juce::ComboBox osc1WaveformBox;
    LabelledKnob   osc1OctaveKnob, osc1DetuneKnob, osc1LevelKnob, osc1PwKnob;
    juce::Label    osc1WaveLabel;

    // Osc 2
    juce::ComboBox osc2WaveformBox;
    LabelledKnob   osc2OctaveKnob, osc2DetuneKnob, osc2LevelKnob, osc2PwKnob;
    juce::Label    osc2WaveLabel;

    // Sub / Noise / Mixer
    LabelledKnob subLevelKnob, noiseLevelKnob;

    // Filter
    LabelledKnob filterCutoffKnob, filterResKnob;

    // ADSR
    LabelledKnob attackKnob, decayKnob, sustainKnob, releaseKnob;

    // LFO
    juce::ComboBox lfoTargetBox;
    LabelledKnob   lfoRateKnob, lfoDepthKnob;
    juce::Label    lfoTargetLabel;

    // Attachments
    std::unique_ptr<ComboAttachment> osc1WfAttach, osc2WfAttach, lfoTgtAttach;
    std::unique_ptr<Attachment> osc1OctAttach, osc1DetAttach, osc1LvlAttach, osc1PwAttach;
    std::unique_ptr<Attachment> osc2OctAttach, osc2DetAttach, osc2LvlAttach, osc2PwAttach;
    std::unique_ptr<Attachment> subLvlAttach, noiseLvlAttach;
    std::unique_ptr<Attachment> cutAttach, resAttach;
    std::unique_ptr<Attachment> atkAttach, decAttach, susAttach, relAttach;
    std::unique_ptr<Attachment> lfoRateAttach, lfoDepthAttach;

    void drawSection(juce::Graphics& g, juce::Rectangle<int> bounds, const juce::String& title);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyFirstVSTEditor)
};
