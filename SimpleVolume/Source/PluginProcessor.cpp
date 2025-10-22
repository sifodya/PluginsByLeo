#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>
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

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

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

    float gainAmplitude = gainParameter->load();
    previousGain = pow(10.0f, gainAmplitude/20.0f);
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
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float gainAmplitude = gainParameter->load();
        float currentGain = pow(10.0f, gainAmplitude/20.0f);

        if (!learnButtonState)
        {
            learnState=IDLE;
            measuredBlocks = 1;
            measuredBlocksRe = 1;
        }

        switch (learnState)
        {
            case IDLE:
                {
                    if(learnButtonState)
                        learnState = LEARNING;
                    if (juce::approximatelyEqual (currentGain, previousGain))
                    {
                        buffer.applyGain (channel, 0, buffer.getNumSamples(), currentGain);
                    }
                    else
                    {
                        buffer.applyGainRamp (channel, 0, buffer.getNumSamples(), previousGain, currentGain);
                        previousGain = currentGain;
                    }
                    break;
                }
        case MEASURING:
            {
                float rmsSum = std::accumulate(rmsValues.begin(), rmsValues.end(), 0.0f);
                float rmsAverage = rmsSum/rmsValues.size();
                    sort(rmsValues.begin(), rmsValues.end());
                    if (rmsValues.size() % 2 != 0)
                        rmsMedian = rmsValues[rmsValues.size()/2];
                    else
                        rmsMedian = (rmsValues[(rmsValues.size()-1)/2] + rmsValues[rmsValues.size()/2])/2;
                    if (abs(rmsAverage-rmsMedian)<learnThreshold)
                    {
                        float valueToSet = 0;
                        if (rmsMedian != 0)
                         valueToSet = targetLoudness - 20.0f * log10(rmsMedian);
                        else
                            valueToSet = targetLoudness;
                        parameters.getParameter("gain")->setValueNotifyingHost(parameters.getParameter("gain")->convertTo0to1(valueToSet));
                        learnState = END;
                        break;
                    }
                learnState = RELEARNING;
                break;
            }
            case LEARNING:
            {
                if (measuredBlocks <= targetMeasurements*getSampleRate()/buffer.getNumSamples())
                {
                    float rms = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
                    cout<<"RMS: "<<rms<<" Measured: "<<measuredBlocks<<" Target: "<<targetMeasurements*getSampleRate()/buffer.getNumSamples()<<endl;
                    rmsValues.push_back(rms);
                    measuredBlocks++;
                }
                    else
                    {
                        learnState = MEASURING;
                    }
                break;
            }
            case END:
            {
                auto* editor = dynamic_cast<AudioPluginAudioProcessorEditor*>(getActiveEditor());
                if (editor)
                {
                    editor->learnButtonImage.setToggleState(false, juce::dontSendNotification);
                    learnButtonState = false;
                    measuredBlocks = 1;
                    learnState = IDLE;
                }
                break;
            }
            case RELEARNING:
            {
                    if (measuredBlocksRe <= (targetMeasurements*getSampleRate()/buffer.getNumSamples())/5)
                    {
                        float rms = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
                        rmsValues.push_back(rms);
                        measuredBlocksRe++;
                    }
                    else
                    {
                        learnState = MEASURING;
                        measuredBlocksRe = 1;
                    }
                break;
            }
        }
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
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
