#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    juce::ignoreUnused(processorRef);

    p.onTrigger += resized();

    screen = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    if (screen!=nullptr)
    {
        const auto screenBounds = screen->logicalBounds;
        screenWidth = screenBounds.getWidth();
        screenHeight = screenBounds.getHeight();
    }

    getConstrainer()->setFixedAspectRatio(pluginRatio);
    setResizeLimits(PLUGIN_MINWIDTH, PLUGIN_MINWIDTH/pluginRatio, PLUGIN_MAXWIDTH, PLUGIN_MAXWIDTH/pluginRatio);
    setResizable(false, true);

    int width = processorRef.getEditorWidth();
    int height = processorRef.getEditorHeight();
    makeContentVisible();
    gainSlider.addMouseListener(this, false);

    learnButtonImage.onClick = [this]
    {
        processorRef.setLearnButtonState(learnButtonImage.getToggleState());
        gainSlider.setLearnState(learnButtonImage.getToggleState());
    };

    if (width <= 0 || height <= 0)
    {
        const float screenRatio = screenWidth/3840.0f;
        width = PLUGIN_INITWIDTH * screenRatio;
        height = width/pluginRatio;
        setSize(width, height);
    }
    else
        setSize(width, height);

}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.setEditorSize(getWidth(), getHeight());
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

    const auto pluginArea = getLocalBounds();
    fullPluginTemplate.setBounds(0, 0, getWidth(), getHeight());
    fullPluginTemplate.resizeFromEditor(pluginArea);

    gainSlider.getLNF()->setScalar(scalarWidth, scalarHeight);
    gainSlider.sendLookAndFeelChange();

    pluginTitle.setBounds(scalarWidth * 46, scalarHeight * 102, scalarWidth * 280, scalarHeight * 158);
    pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(scalarWidth * 42, scalarHeight * 419, scalarWidth * 750, scalarHeight * 750);
    learnButtonImage.setBounds(scalarWidth * 110, scalarHeight * 365, scalarWidth * 104, scalarHeight * 41);
    linkButton.setBounds(scalarWidth * 34, scalarHeight * 1201, scalarWidth * 300, scalarHeight * 40);
}

void AudioPluginAudioProcessorEditor::makeContentVisible()
{
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}

void AudioPluginAudioProcessorEditor::onProc::onEvent()
{

}
