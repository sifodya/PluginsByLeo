#pragma once

#include "PluginProcessor.h"
#include "LeoBackground.cpp"
//#include "SimpleSliderLookAndFeel.cpp"
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
    void makeContentVisible();
    //=============================================================================
    juce::ImageButton& getLearnButton() {return learnButtonImage;}
    void setLearnButtonValue(const bool v) { learnButtonImage.setToggleState(v, juce::dontSendNotification); }
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void resizeComponent (Component& c) const;

    SimpleImageButton learnButtonImage;
private:

    LeoGainSlider gainSlider {};
    AudioPluginAudioProcessor& processorRef;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    LeoBackground fullPluginTemplate;
    LeoPluginTitle pluginTitle;
    SimpleHyperlinkButton linkButton;

    float scalarWidth, scalarHeight {1.0f};
    float aspectRatioHeight {80.0f}, aspectRationWidth {23.0f};
    double pluginRatio {368.0f/1281.0f};

    const juce::Displays::Display* screen {nullptr};
    //juce::Rectangle<int> screenArea {};
    int screenWidth {0}, screenHeight {0}, screenScaleHeight {1}, screenScaleWidth {1};
    double lastSliderValue {0.0};
    bool capture {false};
    float captureOffset {0.0f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
