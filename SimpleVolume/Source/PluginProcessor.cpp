#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>
#include "LeoMacros.h"
using namespace std;

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters (*this, nullptr, "Parameters", createParameters())
{
    previousGain = 0;
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() = default;

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need.
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    gainParameter = parameters.getRawParameterValue("gain");

    const float gainAmplitude = gainParameter->load();
    previousGain = dbToLin(gainAmplitude));

}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainInputChannelSet().getAmbisonicOrder() != -1)
        return false;



    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    //juce::ignoreUnused (midiMessages);
    if (!midiMessages.isEmpty())
    {
        cout<<"Num of MidiEvents: "<<midiMessages.getNumEvents()<<endl;
        //auto midiIt = midiMessages.findNextSamplePosition(midiMessages.getFirstEventTime());
    }

    juce::ScopedNoDenormals noDenormals;
    //const auto totalNumInputChannels  = getTotalNumInputChannels();
    //const auto totalNumOutputChannels = getTotalNumOutputChannels();

    return;
    if (auto* currentEditor = dynamic_cast<AudioPluginAudioProcessorEditor*>(getActiveEditor()))
    {
        currentEditor->getGainSlider().DSP(buffer, this);
    }


}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    cout<<"Editor created"<<endl;
    return new AudioPluginAudioProcessorEditor (*this);
}

void AudioPluginAudioProcessor::setEditorSize(int width, int height)
{

    if (width > 0 && height > 0)
    {
        editorWidth = width;
        editorHeight = height;
        cout<<"set Editor Size Height: "<<editorWidth<<" "<<editorHeight<<endl;
        // Update the editor size in the state
        editorSize.setProperty("editorWidth", editorWidth, nullptr);
        editorSize.setProperty("editorHeight", editorHeight, nullptr);
    }
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save the current editor size to the state
    auto state = parameters.copyState();

    state.setProperty("editorWidth", editorWidth, nullptr);
    state.setProperty("editorHeight", editorHeight, nullptr);
    
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState)
    {
        auto newState = juce::ValueTree::fromXml (*xmlState);

        parameters.replaceState(newState);

        editorWidth = newState.getProperty("editorWidth");
        editorHeight = newState.getProperty("editorHeight");
    }
}

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters()
{
    return{
    make_unique<juce::AudioParameterFloat>("gain",
        "Gain",
        -39.21f,
        15.21f,
        0.0f),
    make_unique<juce::AudioParameterBool>("learnButton",
        "Learn Button",
        false)
        };
}
