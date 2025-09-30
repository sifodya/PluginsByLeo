#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p),
      gainAttachment (processorRef.getParameters(), "gain", gainSlider),
      learnAttachment (processorRef.getParameters(), "learnButton", learnButton)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    gainLabel.setJustificationType(juce::Justification::centred);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);

    learnButton.setButtonText("Learn Gain");
    learnButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    learnButton.setClickingTogglesState(true);

    learnButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::green);
    learnButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
    learnButton.onClick = [this]()
    {
        //change state of button when it's clicked
        const bool isLearning = learnButton.getToggleState();
        learnButton.setButtonText(isLearning ? "Learning..." : "Learn Gain");
    };

    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(learnButton);
    setSize (400, 400);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::burlywood);
}

void AudioPluginAudioProcessorEditor::resized()
{
    gainLabel.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 120, 100, 20);
    gainSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 100, 100, 200);
    learnButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 + 120, 100, 20);
}
