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

    gainSlider.onValueChange = [this]
    {
        std::cout<<"gainSlider changed"<<std::endl;
        if(!processorRef.getButtonStateToSet())
        {
            std::cout<<"gainSlider changed state"<<std::endl;
            learnButtonImage.setToggleState(false, juce::dontSendNotification);
        }
    };

    learnButtonImage.onClick = [this]
    {
        std::cout<<"Clicked"<<std::endl;
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
    if (!processorRef.getButtonStateToSet())
    {
        learnButtonImage.setToggleState(false, juce::dontSendNotification);
    }
    getConstrainer()->checkComponentBounds(this);
    scalarWidth = static_cast<float>(getWidth())/PLUGIN_WIDTH;
    scalarHeight = static_cast<float>(getHeight())/PLUGIN_HEIGHT;

    const auto pluginArea = getLocalBounds();
    m_pluginBackground.setBounds(0, 0, getWidth(), getHeight());
    m_pluginBackground.resizeFromEditor(pluginArea);

    //gainSlider.getLNF()->setScalar(scalarWidth, scalarHeight);
    //gainSlider.sendLookAndFeelChange();
    gainSlider.resizeSlider(scalarWidth, scalarHeight);
    linkButton.resizeButton(scalarWidth, scalarHeight);
    learnButtonImage.resizeButton(scalarWidth, scalarHeight);

    m_pluginTitle.setBounds(scalarWidth * 46, scalarHeight * 102, scalarWidth * 280, scalarHeight * 158);
    m_pluginTitle.resizeFromEditor(m_pluginTitle.getBounds());
    //gainSlider.setBounds(scalarWidth * 42, scalarHeight * 419, scalarWidth * 750, scalarHeight * 750);
    //learnButtonImage.setBounds(scalarWidth * 110, scalarHeight * 365, scalarWidth * 104, scalarHeight * 41);
    //linkButton.setBounds(scalarWidth * 34, scalarHeight * 1201, scalarWidth * 300, scalarHeight * 40);
}

void AudioPluginAudioProcessorEditor::makeContentVisible()
{
    addAndMakeVisible(m_pluginBackground);
    addAndMakeVisible(m_pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
}