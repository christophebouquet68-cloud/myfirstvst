cmake_minimum_required(VERSION 3.22)
project(MyFirstVST VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE as subdirectory (git submodule at JUCE/)
add_subdirectory(JUCE)

juce_add_plugin(MyFirstVST
    COMPANY_NAME            "MyCompany"
    PLUGIN_MANUFACTURER_CODE Mcmp
    PLUGIN_CODE             Mfv1
    IS_SYNTH                TRUE
    NEEDS_MIDI_INPUT        TRUE
    NEEDS_MIDI_OUTPUT       FALSE
    IS_MIDI_EFFECT          FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    FORMATS                 VST3 Standalone
    PRODUCT_NAME            "MyFirstVST"
    VST3_CATEGORIES         "Instrument" "Synth"
)

target_sources(MyFirstVST
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/SynthVoice.cpp
)

target_compile_definitions(MyFirstVST
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

juce_generate_juce_header(MyFirstVST)

target_link_libraries(MyFirstVST
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
