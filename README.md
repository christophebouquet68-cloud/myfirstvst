# MyFirstVST

A polyphonic subtractive synthesiser VST3 plugin built with JUCE.

## Features

| Block | Details |
|---|---|
| **Oscillator 1 & 2** | Sawtooth, Square, Triangle, Pulse (variable PW) — PolyBLEP band-limited |
| **Sub-oscillator** | One octave below Osc 1, square wave |
| **Noise generator** | White noise |
| **Mixer** | Independent level knobs for Osc1, Osc2, Sub, Noise |
| **VCF** | TPT State Variable Filter — Cutoff (20 Hz–20 kHz) + Resonance |
| **VCA** | Velocity-sensitive amplitude stage |
| **ADSR** | Attack / Decay / Sustain / Release envelope |
| **LFO** | Sine wave — Rate (0.1–20 Hz), Depth, Target: Pitch / Filter / Amplitude |
| **Polyphony** | 8 voices |
| **Formats** | VST3, Standalone |

## Requirements

- macOS 12+ (Monterey or later recommended)
- Xcode 14+
- CMake 3.22+
- Git

## Build

```bash
# 1. Clone with submodule
git clone --recurse-submodules https://github.com/christophebouquet68-cloud/myfirstvst.git
cd myfirstvst

# 2. Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build --config Release

# The VST3 is auto-copied to ~/Library/Audio/Plug-Ins/VST3/
# The Standalone app lands in build/MyFirstVST_artefacts/Release/Standalone/
```

> **Tip:** Open `build/MyFirstVST.xcodeproj` in Xcode if you prefer the IDE workflow.

## Project layout

```
myfirstvst/
├── CMakeLists.txt
├── JUCE/                   ← git submodule (JUCE framework)
└── Source/
    ├── Oscillator.h        ← Band-limited oscillator (PolyBLEP)
    ├── NoiseGenerator.h    ← White noise
    ├── Filter.h            ← TPT State Variable Filter
    ├── Lfo.h               ← LFO
    ├── SynthSound.h        ← JUCE SynthesiserSound
    ├── SynthVoice.h/.cpp   ← Single synth voice
    ├── PluginProcessor.h/.cpp  ← AudioProcessor + parameter tree
    └── PluginEditor.h/.cpp     ← GUI
```

## Pushing this repo to GitHub for the first time

```bash
git init
git add .
git commit -m "Initial commit: MyFirstVST synthesiser"
git remote add origin https://github.com/christophebouquet68-cloud/myfirstvst.git
git branch -M main
git push -u origin main
```
