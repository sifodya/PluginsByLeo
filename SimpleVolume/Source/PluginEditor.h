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
    void makeContentVisible(int height, int width);
    //=============================================================================
    const juce::ImageButton& getLearnButton() {return learnButtonImage;}
    void setLearnButtonValue(bool v) { learnButtonImage.setToggleState(v, juce::dontSendNotification); }
    SimpleImageButton learnButtonImage;


private:
    SimpleSliderLookAndFeel simpleSliderLNF;
    AudioPluginAudioProcessor& processorRef;
    juce::Slider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    LeoBackground fullPluginTemplate;
    LeoPluginTitle pluginTitle;
    SimpleHyperlinkButton linkButton;

    int pluginWidth, pluginHeight, pluginBottom;
    float scalarWidth, scalarHeight ;

    const juce::Displays::Display* screen;
    juce::Rectangle<int> screenArea;
    int screenWidth, screenHeight, screenScaleHeight, screenScaleWidth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
