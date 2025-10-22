#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
using namespace std;


//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      gainAttachment(processorRef.getParameters(),
                     "gain",
                     gainSlider)
{
    juce::ignoreUnused(processorRef);

    learnButtonImage.onClick = [this]()
    {
        processorRef.setLearnButtonState(learnButtonImage.getToggleState());
    };

    setResizable(true, true);
    setResizeLimits(200, 500, 1000, 2500);

    initializeSlider();
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::orange);
    gainSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::red);
    makeContentVisible();
    pluginWidth = getWidth();
    pluginHeight = getHeight();
    pluginBottom = getBottom();
    scalarWidth = getWidth()/368.0f;
    scalarHeight = getHeight()/1281.0f;
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
    fullPluginTemplate.setBounds(0, 0, getWidth(), getHeight());
    scalarWidth = getWidth()/368.0f;
    scalarHeight = getHeight()/1281.0f;
    simpleSliderLNF.editorScalarWidth = scalarWidth;
    simpleSliderLNF.editorScalarHeight = scalarHeight;
    gainSlider.sendLookAndFeelChange();
    pluginTitle.setBounds(44 * scalarWidth, 100 * scalarHeight, 280 * scalarWidth, 158 * scalarHeight);
    pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(42 * scalarWidth, 470 * scalarHeight, 750 * scalarWidth , 700 * scalarHeight);
    learnButtonImage.setBounds(109 * scalarWidth, 365 * scalarHeight, 104 * scalarWidth, 41 * scalarHeight);
    linkButton.setBounds(34 * scalarWidth, 1201 * scalarHeight, 300 * scalarWidth, 40 * scalarHeight);
}

void AudioPluginAudioProcessorEditor::initializeSlider()
{
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 400, 50);
    gainSlider.setTextBoxIsEditable(true);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setLookAndFeel(&simpleSliderLNF);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setValue(0.0f);
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    gainSlider.setColour(juce::Slider::textBoxHighlightColourId, juce::Colours::black);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
}

void AudioPluginAudioProcessorEditor::makeContentVisible()
{
    setSize (fullPluginTemplate.image.getWidth(), fullPluginTemplate.image.getHeight());
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}
