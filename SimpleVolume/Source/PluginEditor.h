#pragma once

#include "PluginProcessor.h"
#include "LeoSlider.h"
#include "LeoButtons.h"
#include "LeoCanvas.h"
#include "BinaryData.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
juce::Timer
{
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

    void timerCallback() override;
private:
    LeoImageButton learnButtonImage
    {
        BinaryData::learn_normal_png,
        BinaryData::learn_normal_pngSize,
        BinaryData::learn_yellow_png,
        BinaryData::learn_yellow_pngSize,
        BinaryData::learn_red_png,
        BinaryData::learn_red_pngSize,
    };
    LeoHyperLinkbutton linkButton
    {
        BinaryData::NeueHaasDisplayLight_ttf,
        BinaryData::NeueHaasDisplayLight_ttfSize,
        "check other plugins",
        "https://leo-brennauer.com/#minishop"
    };
    AudioPluginAudioProcessor& processorRef;
    LeoGainSlider gainSlider {gainSliderInfo};//{400, 50, 1};
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    LeoCanvas m_pluginBackground {BinaryData::bg_png, BinaryData::bg_pngSize, 0, 0, 368, 1281}; //TODO Check correct Size
    LeoCanvas m_pluginTitle {BinaryData::ueberschrift_png, BinaryData::ueberschrift_pngSize, 46, 102, 280, 158};


    float scalarWidth {1.0f}, scalarHeight {1.0f};
    float aspectRatioHeight {80.0f}, aspectRationWidth {23.0f};
    double pluginRatio {368.0f/1281.0f};

    const juce::Displays::Display* screen {nullptr};
    int screenWidth {0}, screenHeight {0}, screenScaleHeight {1}, screenScaleWidth {1};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
