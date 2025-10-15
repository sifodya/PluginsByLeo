#pragma once

#include "PluginProcessor.h"
#include "LeoBackground.cpp"
#include "SimpleSliderLookAndFeel.cpp"
#include "LeoPluginTitle.cpp"
#include "SimpleImageButton.cpp"
#include "SimpleHyperlinkButton.cpp"
//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void initializeSlider();
    void initializeButton();
    void makeContentVisible();

private:
    SimpleSliderLookAndFeel simpleSliderLNF;
    AudioPluginAudioProcessor& processorRef;
    juce::Label gainLabel {"GainLabel", "Gain"};
    juce::Slider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    juce::TextButton learnButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment learnAttachment;
    LeoBackground fullPluginTemplate;
    LeoPluginTitle pluginTitle;
    SimpleImageButton learnButtonImage;
    SimpleHyperlinkButton linkButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
