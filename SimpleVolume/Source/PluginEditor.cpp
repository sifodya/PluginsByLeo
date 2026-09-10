#include "PluginEditor.h"
#include "PluginProcessor.h"

using  std::cout, std::endl;

#define PLUGIN_MINWIDTH 60
#define PLUGIN_MAXWIDTH 500
#define PLUGIN_INITWIDTH 300
#define PLUGIN_WIDTH 368
#define PLUGIN_HEIGHT 1281
#define NUMBER_OF_DECIMALS 1

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      gainAttachment(processorRef.getParameters(),
                     "gain",
                     gainSlider)
{
    startTimer(500);
    screen = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    if (screen!=nullptr)
    {
        const auto screenBounds = screen->logicalBounds;
        screenWidth = static_cast<int>(screenBounds.getWidth());
        screenHeight = static_cast<int>(screenBounds.getHeight());
    }

    getConstrainer()->setFixedAspectRatio(pluginRatio);
    setResizeLimits(PLUGIN_MINWIDTH, PLUGIN_MINWIDTH/pluginRatio, PLUGIN_MAXWIDTH, PLUGIN_MAXWIDTH/pluginRatio);
    setResizable(false, true);

    int width = processorRef.getEditorWidth();
    int height = processorRef.getEditorHeight();
    makeContentVisible();
    gainSlider.addMouseListener(this, false);

    /*gainSlider.onValueChange = [this]
    {
        if(!processorRef.getButtonStateToSet())
            learnButtonImage.setToggleState(false, juce::dontSendNotification);
    };*/


    learnButtonImage.onClick = [this]
    {
        processorRef.setLearnButtonState(learnButtonImage.getToggleState());
        gainSlider.setLearnState(learnButtonImage.getToggleState());
    };

    if (width <= 0 || height <= 0)
    {
        const float screenRatio = static_cast<float>(screenWidth) / 3840.0f;
        width = static_cast<int>(PLUGIN_INITWIDTH * screenRatio);
        height = width/pluginRatio;
        setSize(width, height);
    }
    else
        setSize(width, height);

}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.setEditorSize(getWidth(), getHeight());
    stopTimer();
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
    getConstrainer()->checkComponentBounds(this);
    scalarWidth = static_cast<float>(getWidth())/PLUGIN_WIDTH;
    scalarHeight = static_cast<float>(getHeight())/PLUGIN_HEIGHT;

    gainSlider.resizeSlider(scalarWidth, scalarHeight);
    linkButton.resizeButton(scalarWidth, scalarHeight);
    learnButtonImage.resizeButton(scalarWidth, scalarHeight);
    m_pluginBackground.resizeCanvas(scalarWidth, scalarHeight);
    m_pluginTitle.resizeCanvas(scalarWidth, scalarHeight);
}

void AudioPluginAudioProcessorEditor::makeContentVisible()
{
    addAndMakeVisible(m_pluginBackground);
    addAndMakeVisible(m_pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    if (!processorRef.getButtonStateToSet())
        learnButtonImage.setToggleState(false, juce::dontSendNotification);
}
