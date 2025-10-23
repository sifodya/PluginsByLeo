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

    screen = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    if (screen!=nullptr)
    {
        screenArea = screen->totalArea;
        screenWidth = screenArea.getWidth();
        screenHeight = screenArea.getHeight();
        screenScaleHeight = (screenHeight / 1440) * 1281;
        screenScaleWidth = (screenWidth / 2560) * 368;
        cout << "Screen Scale Height: " << screenScaleHeight << "Screen Scale Width: " << screenScaleWidth <<endl;
    }


    learnButtonImage.onClick = [this]()
    {
        processorRef.setLearnButtonState(learnButtonImage.getToggleState());
    };

    setResizable(true, true);
    setResizeLimits(200, 500, 1000, 2500);

    initializeSlider();

    makeContentVisible(screenScaleHeight, screenScaleWidth);
    pluginWidth = getWidth();
    pluginHeight = getHeight();
    pluginBottom = getBottom();
    scalarWidth = getWidth()/368.0f;
    scalarHeight = getHeight()/1281.0f;
    cout << "Plugin height "<<getHeight()<<" Plugin width "<<getWidth()<<endl;
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
    pluginTitle.setBounds(46 * scalarWidth, 102 * scalarHeight, 280 * scalarWidth, 158 * scalarHeight);
    pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(42 * scalarWidth, 469 * scalarHeight, 750 * scalarWidth , 700 * scalarHeight);
    learnButtonImage.setBounds(110 * scalarWidth, 365 * scalarHeight, 104 * scalarWidth, 41 * scalarHeight);
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

void AudioPluginAudioProcessorEditor::makeContentVisible(int height, int width)
{
    setSize (width, height);
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}
