#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p),
      processorRef (p),
      parameters(vts)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    gainLabel.setText ("Gain", juce::dontSendNotification);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);
    gainAttachment.reset (new AudioProcessorValueTreeState::SliderAttachment (parameters, "gain", gainSlider));
    learnButton.setButtonText("Learn Gain");
    addAndMakeVisible(learnButton);
    learnAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(parameters, "learnButton", learnButton));
    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
