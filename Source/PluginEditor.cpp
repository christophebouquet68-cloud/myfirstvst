#include "PluginEditor.h"

static const juce::Colour kBg        { 0xff1e1e2e };
static const juce::Colour kPanel     { 0xff2a2a3e };
static const juce::Colour kAccent    { 0xff7c6af7 };
static const juce::Colour kText      { 0xffcdd6f4 };
static const juce::Colour kSubText   { 0xff6c7086 };

MyFirstVSTEditor::MyFirstVSTEditor(MyFirstVSTProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(780, 520);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, kAccent);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, kAccent);
    getLookAndFeel().setColour(juce::Label::textColourId, kText);
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, kPanel);
    getLookAndFeel().setColour(juce::ComboBox::textColourId, kText);
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, kPanel);
    getLookAndFeel().setColour(juce::PopupMenu::textColourId, kText);

    auto& apvts = processor.apvts;

    // ---- Osc 1 ----
    osc1WaveformBox.addItem("Sawtooth", 1);
    osc1WaveformBox.addItem("Square",   2);
    osc1WaveformBox.addItem("Triangle", 3);
    osc1WaveformBox.addItem("Pulse",    4);
    osc1WaveLabel.setText("Waveform", juce::dontSendNotification);
    osc1WaveLabel.setFont(juce::Font(11.0f));
    osc1WaveLabel.setJustificationType(juce::Justification::centred);
    osc1WaveLabel.setColour(juce::Label::textColourId, kText);
    osc1OctaveKnob.setup("Octave");
    osc1DetuneKnob.setup("Detune");
    osc1LevelKnob.setup("Level");
    osc1PwKnob.setup("PW");
    addAndMakeVisible(osc1WaveformBox);
    addAndMakeVisible(osc1WaveLabel);
    addAndMakeVisible(osc1OctaveKnob);
    addAndMakeVisible(osc1DetuneKnob);
    addAndMakeVisible(osc1LevelKnob);
    addAndMakeVisible(osc1PwKnob);

    // ---- Osc 2 ----
    osc2WaveformBox.addItem("Sawtooth", 1);
    osc2WaveformBox.addItem("Square",   2);
    osc2WaveformBox.addItem("Triangle", 3);
    osc2WaveformBox.addItem("Pulse",    4);
    osc2WaveLabel.setText("Waveform", juce::dontSendNotification);
    osc2WaveLabel.setFont(juce::Font(11.0f));
    osc2WaveLabel.setJustificationType(juce::Justification::centred);
    osc2WaveLabel.setColour(juce::Label::textColourId, kText);
    osc2OctaveKnob.setup("Octave");
    osc2DetuneKnob.setup("Detune");
    osc2LevelKnob.setup("Level");
    osc2PwKnob.setup("PW");
    addAndMakeVisible(osc2WaveformBox);
    addAndMakeVisible(osc2WaveLabel);
    addAndMakeVisible(osc2OctaveKnob);
    addAndMakeVisible(osc2DetuneKnob);
    addAndMakeVisible(osc2LevelKnob);
    addAndMakeVisible(osc2PwKnob);

    // ---- Sub / Noise ----
    subLevelKnob.setup("Sub");
    noiseLevelKnob.setup("Noise");
    addAndMakeVisible(subLevelKnob);
    addAndMakeVisible(noiseLevelKnob);

    // ---- Filter ----
    filterCutoffKnob.setup("Cutoff");
    filterResKnob.setup("Resonance");
    addAndMakeVisible(filterCutoffKnob);
    addAndMakeVisible(filterResKnob);

    // ---- ADSR ----
    attackKnob.setup("Attack");
    decayKnob.setup("Decay");
    sustainKnob.setup("Sustain");
    releaseKnob.setup("Release");
    addAndMakeVisible(attackKnob);
    addAndMakeVisible(decayKnob);
    addAndMakeVisible(sustainKnob);
    addAndMakeVisible(releaseKnob);

    // ---- LFO ----
    lfoTargetBox.addItem("Pitch",     1);
    lfoTargetBox.addItem("Filter",    2);
    lfoTargetBox.addItem("Amplitude", 3);
    lfoTargetLabel.setText("Target", juce::dontSendNotification);
    lfoTargetLabel.setFont(juce::Font(11.0f));
    lfoTargetLabel.setJustificationType(juce::Justification::centred);
    lfoTargetLabel.setColour(juce::Label::textColourId, kText);
    lfoRateKnob.setup("Rate");
    lfoDepthKnob.setup("Depth");
    addAndMakeVisible(lfoTargetBox);
    addAndMakeVisible(lfoTargetLabel);
    addAndMakeVisible(lfoRateKnob);
    addAndMakeVisible(lfoDepthKnob);

    // ---- Attachments ----
    osc1WfAttach  = std::make_unique<ComboAttachment>(apvts, "osc1_waveform", osc1WaveformBox);
    osc1OctAttach = std::make_unique<Attachment>(apvts, "osc1_octave",  osc1OctaveKnob.slider);
    osc1DetAttach = std::make_unique<Attachment>(apvts, "osc1_detune",  osc1DetuneKnob.slider);
    osc1LvlAttach = std::make_unique<Attachment>(apvts, "osc1_level",   osc1LevelKnob.slider);
    osc1PwAttach  = std::make_unique<Attachment>(apvts, "osc1_pw",      osc1PwKnob.slider);

    osc2WfAttach  = std::make_unique<ComboAttachment>(apvts, "osc2_waveform", osc2WaveformBox);
    osc2OctAttach = std::make_unique<Attachment>(apvts, "osc2_octave",  osc2OctaveKnob.slider);
    osc2DetAttach = std::make_unique<Attachment>(apvts, "osc2_detune",  osc2DetuneKnob.slider);
    osc2LvlAttach = std::make_unique<Attachment>(apvts, "osc2_level",   osc2LevelKnob.slider);
    osc2PwAttach  = std::make_unique<Attachment>(apvts, "osc2_pw",      osc2PwKnob.slider);

    subLvlAttach   = std::make_unique<Attachment>(apvts, "sub_level",   subLevelKnob.slider);
    noiseLvlAttach = std::make_unique<Attachment>(apvts, "noise_level", noiseLevelKnob.slider);

    cutAttach = std::make_unique<Attachment>(apvts, "filter_cutoff",     filterCutoffKnob.slider);
    resAttach = std::make_unique<Attachment>(apvts, "filter_resonance",  filterResKnob.slider);

    atkAttach = std::make_unique<Attachment>(apvts, "attack",  attackKnob.slider);
    decAttach = std::make_unique<Attachment>(apvts, "decay",   decayKnob.slider);
    susAttach = std::make_unique<Attachment>(apvts, "sustain", sustainKnob.slider);
    relAttach = std::make_unique<Attachment>(apvts, "release", releaseKnob.slider);

    lfoTgtAttach   = std::make_unique<ComboAttachment>(apvts, "lfo_target", lfoTargetBox);
    lfoRateAttach  = std::make_unique<Attachment>(apvts, "lfo_rate",  lfoRateKnob.slider);
    lfoDepthAttach = std::make_unique<Attachment>(apvts, "lfo_depth", lfoDepthKnob.slider);
}

void MyFirstVSTEditor::drawSection(juce::Graphics& g, juce::Rectangle<int> bounds,
                                    const juce::String& title)
{
    g.setColour(kPanel);
    g.fillRoundedRectangle(bounds.toFloat(), 6.0f);
    g.setColour(kAccent);
    g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f), 6.0f, 1.0f);
    g.setColour(kText);
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText(title, bounds.removeFromTop(18), juce::Justification::centred);
}

void MyFirstVSTEditor::paint(juce::Graphics& g)
{
    g.fillAll(kBg);

    // Title
    g.setColour(kAccent);
    g.setFont(juce::Font(20.0f, juce::Font::bold));
    g.drawText("MyFirstVST", getLocalBounds().removeFromTop(36), juce::Justification::centred);

    const int margin = 8;
    const int top    = 40;
    const int w      = getWidth();
    const int h      = getHeight() - top - margin;

    // Row 1: Osc1 | Osc2 | Sub+Noise
    const int row1H  = 160;
    const int oscW   = (w - 4 * margin) * 2 / 5;
    const int mixW   = w - 4 * margin - 2 * oscW;
    drawSection(g, { margin,          top,          oscW, row1H }, "Oscillator 1");
    drawSection(g, { 2 * margin + oscW, top,         oscW, row1H }, "Oscillator 2");
    drawSection(g, { 3 * margin + 2 * oscW, top,     mixW, row1H }, "Mixer");

    // Row 2: Filter | ADSR | LFO
    const int row2Y  = top + row1H + margin;
    const int row2H  = h - row1H - margin;
    const int fltW   = 160;
    const int lfoW   = 180;
    const int adsrW  = w - 4 * margin - fltW - lfoW;
    drawSection(g, { margin,                  row2Y, fltW,  row2H }, "VCF");
    drawSection(g, { 2 * margin + fltW,       row2Y, adsrW, row2H }, "ADSR Envelope");
    drawSection(g, { 3 * margin + fltW + adsrW, row2Y, lfoW, row2H }, "LFO");
}

void MyFirstVSTEditor::resized()
{
    const int margin = 8;
    const int top    = 40;
    const int w      = getWidth();
    const int h      = getHeight() - top - margin;

    const int row1H  = 160;
    const int oscW   = (w - 4 * margin) * 2 / 5;
    const int mixW   = w - 4 * margin - 2 * oscW;

    // ---- Osc 1 layout ----
    {
        auto b = juce::Rectangle<int>{ margin, top, oscW, row1H }.reduced(6, 18);
        const int kw = b.getWidth() / 5;
        osc1WaveLabel.setBounds(b.getX(), b.getY(), kw * 2, 16);
        osc1WaveformBox.setBounds(b.getX(), b.getY() + 16, kw * 2, 24);
        osc1LevelKnob.setBounds(b.getX() + kw * 2, b.getY(), kw, b.getHeight());
        osc1OctaveKnob.setBounds(b.getX() + kw * 3, b.getY(), kw, b.getHeight());
        osc1DetuneKnob.setBounds(b.getX() + kw * 4, b.getY(), kw - 2, b.getHeight());
        osc1PwKnob.setBounds(b.getX(), b.getY() + 50, kw, b.getHeight() - 50);
    }

    // ---- Osc 2 layout ----
    {
        auto b = juce::Rectangle<int>{ 2 * margin + oscW, top, oscW, row1H }.reduced(6, 18);
        const int kw = b.getWidth() / 5;
        osc2WaveLabel.setBounds(b.getX(), b.getY(), kw * 2, 16);
        osc2WaveformBox.setBounds(b.getX(), b.getY() + 16, kw * 2, 24);
        osc2LevelKnob.setBounds(b.getX() + kw * 2, b.getY(), kw, b.getHeight());
        osc2OctaveKnob.setBounds(b.getX() + kw * 3, b.getY(), kw, b.getHeight());
        osc2DetuneKnob.setBounds(b.getX() + kw * 4, b.getY(), kw - 2, b.getHeight());
        osc2PwKnob.setBounds(b.getX(), b.getY() + 50, kw, b.getHeight() - 50);
    }

    // ---- Mixer (Sub + Noise) ----
    {
        auto b = juce::Rectangle<int>{ 3 * margin + 2 * oscW, top, mixW, row1H }.reduced(6, 22);
        const int kw = b.getWidth() / 2;
        subLevelKnob.setBounds(b.getX(),       b.getY(), kw, b.getHeight());
        noiseLevelKnob.setBounds(b.getX() + kw, b.getY(), kw, b.getHeight());
    }

    // Row 2
    const int row2Y  = top + row1H + margin;
    const int row2H  = h - row1H - margin;
    const int fltW   = 160;
    const int lfoW   = 180;
    const int adsrW  = w - 4 * margin - fltW - lfoW;

    // ---- Filter ----
    {
        auto b = juce::Rectangle<int>{ margin, row2Y, fltW, row2H }.reduced(6, 22);
        const int kw = b.getWidth() / 2;
        filterCutoffKnob.setBounds(b.getX(),       b.getY(), kw, b.getHeight());
        filterResKnob.setBounds(b.getX() + kw, b.getY(), kw, b.getHeight());
    }

    // ---- ADSR ----
    {
        auto b = juce::Rectangle<int>{ 2 * margin + fltW, row2Y, adsrW, row2H }.reduced(6, 22);
        const int kw = b.getWidth() / 4;
        attackKnob.setBounds(b.getX(),            b.getY(), kw, b.getHeight());
        decayKnob.setBounds(b.getX() + kw,        b.getY(), kw, b.getHeight());
        sustainKnob.setBounds(b.getX() + kw * 2,  b.getY(), kw, b.getHeight());
        releaseKnob.setBounds(b.getX() + kw * 3,  b.getY(), kw, b.getHeight());
    }

    // ---- LFO ----
    {
        auto b = juce::Rectangle<int>{ 3 * margin + fltW + adsrW, row2Y, lfoW, row2H }.reduced(6, 22);
        const int kw = b.getWidth() / 3;
        lfoRateKnob.setBounds(b.getX(),          b.getY(), kw, b.getHeight() - 40);
        lfoDepthKnob.setBounds(b.getX() + kw,    b.getY(), kw, b.getHeight() - 40);
        lfoTargetLabel.setBounds(b.getX() + kw * 2 + 2, b.getY(), kw - 2, 16);
        lfoTargetBox.setBounds(b.getX() + kw * 2 + 2, b.getY() + 16, kw - 2, 24);
    }
}
