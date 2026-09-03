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
    cout<<"Constructor"<<endl;
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
    cout<<"Prepare to play"<<endl;
    // Use this method as the place to do any pre-playback
    // initialisation that you need.
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    gainParameter = parameters.getRawParameterValue("gain");

    const float gainAmplitude = gainParameter->load();
    previousGain = dbToLin(gainAmplitude));

}

void AudioPluginAudioProcessor::releaseResources()
{
    cout<<"Release"<<endl;
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    cout<<"Bus Layout"<<endl;
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
    cout<<"Process Block"<<endl;

    //juce::ignoreUnused (midiMessages);
    if (!midiMessages.isEmpty())
    {
        for (const auto metadata : midiMessages)
        {
            const auto message = metadata.getMessage();
            if (message.isNoteOnOrOff())
                cout<<"NoteOnOrOff"<<endl;
        }
    }

    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

        //Discard Input Channels that cannot get maped to a Output Channel
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const double gainAmplitude = gainParameter->load();
        const double currentGain = dbToLin(gainAmplitude));

        if (!learnButtonState)
        {
            cout<<"Init State to Idle"<<endl;
            learnState=IDLE;
            rmsValues.clear();
            rmsValues.shrink_to_fit();
            measuredBlocks = 1;
            measuredBlocksRe = 1;
        }

        switch (learnState)
        {
        case IDLE:
            {
                cout<<"IDLE"<<endl;
                if(learnButtonState)
                    learnState = LEARNING;
                break;
            }
        case MEASURING:
            {
                cout <<"MEASURING"<<endl;
                 float rmsSum {0};
                    for (auto i {0}; i < rmsValues.size(); i++)
                        rmsSum = rmsSum + rmsValues[i];

                const float rmsAverage = rmsSum/static_cast<float>(rmsValues.size());
                ranges::sort(rmsValues);
                if (rmsValues.size() % 2 != 0)
                    rmsMedian = rmsValues[rmsValues.size()/2];
                else
                    rmsMedian = (rmsValues[(rmsValues.size()-1)/2] + rmsValues[rmsValues.size()/2])/2;
                if (abs(rmsAverage-rmsMedian)<learnThreshold)
                {
                    float valueToSet = 0;
                    if (rmsMedian != 0)
                    {
                        //const auto rmsMedianDb = 20.0f * log10(rmsMedian/0.707f);
                        const auto target = targetLoudness - rmsMedian;
                        //target = target + 3.0f;
                        valueToSet = static_cast<float>(target);
                        cout<<"Value to set DB: "<<target<<" rms Median db :"<<rmsMedian<<" Sum: "<<rmsSum<<endl;
                    }
                    else
                        valueToSet = static_cast<float>(targetLoudness);

                    //cout<<"Value to Set: "<<valueToSet<<" Target: "<<targetLoudnessLin<<" Median: "<<m_rmsMedian<<" Avg: "<<rmsAverage<<endl;
                    parameters.getParameter("gain")->setValueNotifyingHost(parameters.getParameter("gain")->convertTo0to1(valueToSet));
                    learnState = END;
                    break;
                }
                learnState = RELEARNING;
                break;
            }
            case LEARNING:
            {
                cout<<"Learning"<<endl;


                if (!getPlayHead()->getPosition()->getIsPlaying())
                    return;
                cout<<"Measure Start"<<endl;


                if (measuredBlocks <= targetMeasurements * getSampleRate()/buffer.getNumSamples())
                {
                    auto rms = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
                    rms = 20.0f * log10(rms/0.707f);
                    rms = round(rms * 100.0f) / 100.0f;
                    rmsValues.push_back(rms);
                    measuredBlocks++;
                    //cout<<"RMS: "<<rms<<endl;
                }
                else
                    {
                        learnState = MEASURING;
                    }
                    //cout<<"Measured Blocks: "<<measuredBlocks<<endl;
                break;
            }
            case END:
            {
                cout<<"END"<<endl;
                if (const auto editor = dynamic_cast<AudioPluginAudioProcessorEditor*>(getActiveEditor()))
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
                cout<<"Relearning"<<endl;
                    if (measuredBlocksRe <= targetMeasurements * getSampleRate() / buffer.getNumSamples()/5)
                    {
                        float rms = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
                        rms = 20.0f * log10(rms/0.707f);
                        rms = round(rms * 100.0f) / 100.0f;
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
        if (juce::approximatelyEqual (static_cast<float>(currentGain), previousGain))
        {
            buffer.applyGain (channel, 0, buffer.getNumSamples(), currentGain);
        }
        else
        {
            buffer.applyGainRamp (channel, 0, buffer.getNumSamples(), previousGain, currentGain);
            previousGain = currentGain;
        }
    }


}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    cout<<"Has Editor"<<endl;
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
    cout<<"Set State"<<endl;
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
    cout<<"Create Plugin"<<endl;
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters()
{
    cout<<"Create Parameters"<<endl;
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
