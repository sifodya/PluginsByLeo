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

    gainLabel.setJustificationType(juce::Justification::centred);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    gainSlider.setLookAndFeel(&simpleSliderLNF);
    gainSlider.toFront(false);


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

    setSize (fullPluginTemplate.image.getWidth(), fullPluginTemplate.image.getHeight());
    addAndMakeVisible(fullPluginTemplate);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(learnButton);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
}


//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll(juce::Colours::orange);
}

void AudioPluginAudioProcessorEditor::resized()
{
    fullPluginTemplate.setBounds(0,
        0,
        700,
        1500);
    gainLabel.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 120, 100, 20);
    gainSlider.setBounds(0, 0, 100, 200);
    learnButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 + 120, 100, 20);
}
