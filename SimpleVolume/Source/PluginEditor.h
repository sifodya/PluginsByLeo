#pragma once

#include "PluginProcessor.h"
#include "LeoBackground.cpp"
#include "LeoSlider.h"
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
    LeoGainSlider& getGainSlider() {return gainSlider;}

    SimpleImageButton learnButtonImage;
private:

    AudioPluginAudioProcessor& processorRef;
    LeoGainSlider gainSlider {gainSliderInfo};//{400, 50, 1};
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    LeoBackground fullPluginTemplate;
    LeoPluginTitle pluginTitle;
    SimpleHyperlinkButton linkButton;

    LeoGainSliderInfo gainSliderInfo
    {
        750,
        750,
        42,
        419,
        BinaryData::NeueHaasDisplayMediu_ttfSize,
        BinaryData::fader_bg_pngSize,
        BinaryData::fader_pngSize,
        BinaryData::NeueHaasDisplayMediu_ttf,
        BinaryData::fader_bg_png,
        BinaryData::fader_png,
        1,
        400,
        50
    };

    float scalarWidth, scalarHeight {1.0f};
    float aspectRatioHeight {80.0f}, aspectRationWidth {23.0f};
    double pluginRatio {368.0f/1281.0f};

    const juce::Displays::Display* screen {nullptr};
    int screenWidth {0}, screenHeight {0}, screenScaleHeight {1}, screenScaleWidth {1};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
