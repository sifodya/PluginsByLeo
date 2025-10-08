#pragma once

#include "PluginProcessor.h"
#include "LeoBackground.cpp"
#include "Simple_Slider.h"
//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    //juce::AudioProcessorValueTreeState& parameters;
    juce::Label gainLabel {"GainLabel", "Gain"};
    Simple_Slider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    juce::TextButton learnButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment learnAttachment;
    LeoBackground fullPluginTemplate;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
