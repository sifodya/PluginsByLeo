#include "PluginProcessor.h"
#include "PluginEditor.h"
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

    // Get saved size from processor or use default
    int width = processorRef.getEditorWidth();
    int height = processorRef.getEditorHeight();
    
    // If no saved size, use default
    if (width <= 0 || height <= 0)
    {
        const int screenRatio = screenArea.getAspectRatio();
        width = PLUGIN_INITWIDTH;
        height = PLUGIN_INITWIDTH/screenRatio;
    }
    
    // Ensure size is within bounds
    width = juce::jlimit(PLUGIN_MINWIDTH, PLUGIN_MAXWIDTH, width);
    height = width / pluginRatio;
    
    setSize(width, height);
    setResizable(true, false);
    setResizeLimits(PLUGIN_MINWIDTH, PLUGIN_MINWIDTH/pluginRatio, PLUGIN_MAXWIDTH, PLUGIN_MAXWIDTH/pluginRatio);
    getConstrainer()->setFixedAspectRatio(pluginRatio);

    initializeSlider();
    makeContentVisible();

    learnButtonImage.onClick = [this]
    {
        processorRef.setLearnButtonState(learnButtonImage.getToggleState());
    };
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
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
    scalarWidth = static_cast<float>(getWidth())/PLUGIN_WIDTH;
    scalarHeight = static_cast<float>(getHeight())/PLUGIN_HEIGHT;

    // Update the processor with the new size
    processorRef.setEditorSize(getWidth(), getHeight());

    const auto pluginArea = getLocalBounds();
    fullPluginTemplate.setBounds(0, 0, getWidth(), getHeight());
    fullPluginTemplate.resizeFromEditor(pluginArea);

    simpleSliderLNF.editorScalarWidth = scalarWidth;
    simpleSliderLNF.editorScalarHeight = scalarHeight;
    gainSlider.sendLookAndFeelChange();
    pluginTitle.setBounds(scalarWidth * 46, scalarHeight * 102, scalarWidth * 280, scalarHeight * 158);
    pluginTitle.resizeFromEditor(pluginTitle.getBounds());
    gainSlider.setBounds(scalarWidth * 42, scalarHeight * 419, scalarWidth * 750, scalarHeight * 750);
    learnButtonImage.setBounds(scalarWidth * 110, scalarHeight * 365, scalarWidth * 104, scalarHeight * 41);
    linkButton.setBounds(scalarWidth * 34, scalarHeight * 1201, scalarWidth * 300, scalarHeight * 40);
}

void AudioPluginAudioProcessorEditor::initializeSlider()
{
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 400, 50);
    gainSlider.setTextBoxIsEditable(true);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setLookAndFeel(&simpleSliderLNF);
    gainSlider.textFromValueFunction = [](const double value){
        return juce::String(value, 1);
    };
    gainSlider.addMouseListener(this, false);
    //gainSlider.setValue(0.0f);
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    gainSlider.setColour(juce::Slider::textBoxHighlightColourId, juce::Colours::black);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
}

void AudioPluginAudioProcessorEditor::makeContentVisible()
{
    setSize (processorRef.getEditorWidth(), processorRef.getEditorHeight());
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButtonImage);
    addAndMakeVisible(linkButton);
    gainSlider.setDoubleClickReturnValue(true, 0.0f, juce::ModifierKeys::ctrlModifier);
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
