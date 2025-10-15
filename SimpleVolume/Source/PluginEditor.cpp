#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>



//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      gainAttachment(processorRef.getParameters(),
                     "gain",
                     gainSlider),
      learnAttachment(processorRef.getParameters(),
                      "learnButton",
                      learnButton)
{
    juce::ignoreUnused(processorRef);

    learnButtonImage.onClick = [this]()
    {
    };

    setResizable(true, true);
    setResizeLimits(200, 500, 2000, 5000);

    gainLabel.setJustificationType(juce::Justification::centred);

    initializeSlider();

    initializeButton();

    makeContentVisible();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
}


//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (linkButton.isMouseOver())
        linkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colours::blueviolet);
    else
        linkButton.setColour(juce::HyperlinkButton::textColourId, juce::Colours::black);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto pluginArea = getLocalBounds();
    fullPluginTemplate.resizeFromEditor(pluginArea);
    fullPluginTemplate.setBounds(0,
        0,
        getWidth(),
        getHeight());
    pluginTitle.setBounds(getWidth() / 4.0, getHeight() / 12.81, pluginTitle.titleImage.getWidth(), pluginTitle.titleImage.getHeight());
    gainLabel.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 120, 100, 20);
    gainSlider.setBounds(0, 0, 100, 200);
    learnButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 + 120, 100, 20);
    learnButtonImage.setBounds(getWidth() / 3 - 50, getHeight() / 3+ 120, 100, 20);
    linkButton.setBounds(getWidth() / 2 - 250, getBottom()-75, 400, 50);
}

void AudioPluginAudioProcessorEditor::initializeSlider()
{
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    gainSlider.setTextBoxIsEditable(true);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setLookAndFeel(&simpleSliderLNF);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setValue(0.0f);
}

void AudioPluginAudioProcessorEditor::initializeButton()
{
    learnButton.setButtonText("Learn Gain");
    learnButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    learnButton.setClickingTogglesState(true);
    learnButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::green);
    learnButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
    learnButton.onClick = [this]()
    {
        //change state of button when it's clicked
        const bool isLearning = learnButton.getToggleState();
        learnButton.setButtonText(isLearning ? "Learning..." : "Learn Gain");
    };
}

void AudioPluginAudioProcessorEditor::makeContentVisible()
{
    setSize (fullPluginTemplate.image.getWidth(), fullPluginTemplate.image.getHeight());
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(learnButton);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}
