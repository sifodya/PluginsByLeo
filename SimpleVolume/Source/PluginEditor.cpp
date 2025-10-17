#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>



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
    };

    setResizable(true, true);
    setResizeLimits(200, 500, 2000, 5000);

    initializeSlider();

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
    std::cout << "Width: " << getWidth() << " Height: " << getHeight() << std::endl;
    scalarWidth = getWidth()/368.0f;
    scalarHeight = getHeight()/1281.0f;
    std::cout<<"Scalar Width: "<<scalarWidth<<" Scalar Height: "<<scalarHeight<<std::endl;
    pluginTitle.setBounds(getWidth() / 2 - pluginTitle.titleImage.getWidth() / 2, getHeight() / 12.81, pluginTitle.titleImage.getWidth()*scalarWidth, pluginTitle.titleImage.getHeight()*scalarHeight);
    pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(getWidth() / 2 - (simpleSliderLNF.backgroundImage.getWidth()/2) - 82, 470, 750, 700);
    learnButtonImage.setBounds(getWidth() / 2 - learnButtonImage.getWidth() / 2 - 23, 365, 104, 41);
    linkButton.setBounds(getWidth() / 2 - linkButton.getWidth()/2, getBottom() - 80, 300, 40);
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
