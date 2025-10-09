#pragma once

#include "PluginProcessor.h"
#include "LeoBackground.cpp"
#include "SimpleSliderLookAndFeel.cpp"
//#include "Simple_Slider.h"
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
    SimpleSliderLookAndFeel simpleSliderLNF;
    AudioPluginAudioProcessor& processorRef;
    //juce::AudioProcessorValueTreeState& parameters;
    juce::Label gainLabel {"GainLabel", "Gain"};
    juce::Slider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    juce::TextButton learnButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment learnAttachment;
    LeoBackground fullPluginTemplate;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
