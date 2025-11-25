#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
using namespace std;
#define PLUGIN_MINWIDTH 60
#define PLUGIN_MAXWIDTH 500
#define PLUGIN_INITWIDTH 300
#define PLUGIN_WIDTH 368
#define PLUGIN_HEIGHT 1281

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
    }

    int screenRatio = screenArea.getAspectRatio();

    setResizable(true, false);
    setResizeLimits(PLUGIN_MINWIDTH, PLUGIN_MINWIDTH/pluginRatio, PLUGIN_MAXWIDTH, PLUGIN_MAXWIDTH/pluginRatio);
    getConstrainer()->setFixedAspectRatio(pluginRatio);

    initializeSlider();

    makeContentVisible(PLUGIN_INITWIDTH/screenRatio, PLUGIN_INITWIDTH);
    pluginBottom = getBottom();
    scalarWidth = getWidth()/PLUGIN_WIDTH;
    scalarHeight = getHeight()/PLUGIN_HEIGHT;

    learnButtonImage.onClick = [this]()
    {
        processorRef.setLearnButtonState(learnButtonImage.getToggleState());
    };
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
    scalarWidth = getWidth()/PLUGIN_WIDTH;
    scalarHeight = getHeight()/PLUGIN_HEIGHT;

    //------------------------------------------------------------------------------------------------------------------

    /*//auto pluginArea = getLocalBounds();
    //fullPluginTemplate.resizeFromEditor(pluginArea);
    fullPluginTemplate.setBounds(0, 0, getWidth(), getHeight());
    //simpleSliderLNF.editorScalarWidth = scalarWidth;
    //simpleSliderLNF.editorScalarHeight = scalarHeight;
    //gainSlider.sendLookAndFeelChange();
    pluginTitle.setBounds(46, 102, 280, 158);
    //pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(42, 469, 750, 700);
    learnButtonImage.setBounds(110, 365, 104, 41);
    linkButton.setBounds(34, 1201, 300, 40);
    oldPluginWidth = pluginWidth;
    oldPluginHeight = pluginHeight;*/
    //------------------------------------------------------------------------------------------------------------------
    auto pluginArea = getLocalBounds();
    fullPluginTemplate.resizeFromEditor(pluginArea);
    fullPluginTemplate.setBounds(0, 0, getWidth(), getHeight());

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
    cout<<width<<" "<<height<<endl;
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}


