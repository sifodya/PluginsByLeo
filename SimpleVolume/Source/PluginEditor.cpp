#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
using namespace std;
#define PLUGIN_HEIGHT 1281; //org 1281 neu 981
#define PLUGIN_WIDTH 368 //org 368 neu 283

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
        screenScaleHeight = (screenHeight / 1440) * PLUGIN_HEIGHT;
        screenScaleWidth = (screenWidth / 2560) * PLUGIN_WIDTH;
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
    scalarWidth = static_cast<float>(getWidth())/PLUGIN_WIDTH;
    scalarHeight = static_cast<float>(getHeight())/PLUGIN_HEIGHT;
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
    scalarWidth = static_cast<float>(getWidth())/PLUGIN_WIDTH;
    scalarHeight = static_cast<float>(getHeight())/PLUGIN_HEIGHT;
    const auto pluginArea = getLocalBounds();
    fullPluginTemplate.resizeFromEditor(pluginArea);
    fullPluginTemplate.setBounds(0, 0, getWidth(), getHeight());
    simpleSliderLNF.editorScalarWidth = scalarWidth;
    simpleSliderLNF.editorScalarHeight = scalarHeight;
    gainSlider.sendLookAndFeelChange();
    pluginTitle.setBounds(static_cast<int>(scalarWidth) * 46, static_cast<int>(scalarHeight) * 102, static_cast<int>(scalarWidth) * 280,static_cast<int>(scalarHeight) * 158);
    pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(static_cast<int>(scalarWidth) * 42, static_cast<int>(scalarHeight) * 469, static_cast<int>(scalarWidth) * 750, static_cast<int>(scalarHeight) * 700);
    learnButtonImage.setBounds(static_cast<int>(scalarWidth) * 110, static_cast<int>(scalarHeight) * 365, static_cast<int>(scalarWidth) * 104, static_cast<int>(scalarHeight) * 41);
    linkButton.setBounds(static_cast<int>(scalarWidth) * 34, static_cast<int>(scalarHeight) * 1201, static_cast<int>(scalarWidth) * 300, static_cast<int>(scalarHeight) * 40);
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

void AudioPluginAudioProcessorEditor::makeContentVisible(const int height, const int width)
{
    setSize (width, height);
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}
