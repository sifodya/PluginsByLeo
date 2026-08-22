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
    screen = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    if (screen!=nullptr)
    {
        //screenArea = screen->totalArea;
        const auto screenBounds = screen->logicalBounds;
        screenWidth = screenBounds.getWidth();//screenArea.getWidth();
        screenHeight = screenBounds.getHeight();//screenArea.getHeight();
        //cout<<"Screen Bounds: "<<screenBounds.getWidth()<<" "<<screenBounds.getHeight()<<"Screen Area: "<<screenWidth<<" "<<screenHeight<<endl;
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

void AudioPluginAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    if (event.eventComponent == &gainSlider)
        lastSliderValue = gainSlider.getValue();
    captureOffset = event.position.y;
}

void AudioPluginAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (event.eventComponent != &gainSlider)
        return;
    float fineFactor, pixelDelta;
    if (event.mods.isShiftDown())
    {
        if (capture==false)
        {
            captureOffset = event.position.y;
            if (event.eventComponent == &gainSlider)
                lastSliderValue = gainSlider.getValue();
            capture = true;
        }
        fineFactor = 0.2f;
        pixelDelta = (event.position.y-captureOffset) * -1.0f;//-(event.position.y - (event.mouseDownPosition.y - captureOffset));
    }
    else
    {
        if (capture==true)
        {
            captureOffset = event.position.y;
            if (event.eventComponent == &gainSlider)
                lastSliderValue = gainSlider.getValue();
            capture = false;
        }
        fineFactor = 1.0f;
        pixelDelta = (event.position.y-captureOffset) * -1.0f;//-(event.position.y - event.mouseDownPosition.y);
    }
    // Slider height controls default sensitivity
    const float sliderLength = gainSlider.getLookAndFeel().getSliderLayout(gainSlider).sliderBounds.getHeight();

    const float valueDelta = pixelDelta / sliderLength
                       * (gainSlider.getMaximum() - gainSlider.getMinimum())
                       * fineFactor;
    gainSlider.setValue(lastSliderValue + valueDelta, juce::sendNotificationSync);
}

void AudioPluginAudioProcessorEditor::resizeComponent(Component& c) const
{
    const auto bounds = c.getBounds();
    const auto position = c.getPosition();

    const int w = std::lround(static_cast<float>(bounds.getY()) * scalarWidth);
    const int h = std::lround(static_cast<float>(bounds.getHeight()) * scalarHeight);
    const int x = std::lround(static_cast<float>(position.getX()) * scalarWidth);
    const int y = std::lround(static_cast<float>(position.getY()) * scalarHeight);

    c.setBounds(x, y, w, h);
}
