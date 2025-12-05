#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    void getEditorSize(const int width, const int height)
    {
        editorWidth = width;
        editorHeight = height;
    }
    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getParameters() {return parameters;}
    void setLearnButtonState(const bool newState) {learnButtonState = newState;}

    int getEditorWidth();
    int getEditorHeight();
    void setEditorSize(int width, int height);

private:
    std::atomic<float>* gainParameter = nullptr;
    float previousGain;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorValueTreeState parameters;
    bool learnButtonState = false;

    std::vector <float> rmsValues;
    int targetMeasurements = 5; //in seconds
    int measuredBlocks = 1;
    int measuredBlocksRe = 1;
    float targetLoudness = -12.0f;
    enum LearnState {IDLE, MEASURING, LEARNING, END, RELEARNING};
    LearnState learnState = IDLE;
    float learnThreshold = 0.5f;
    float rmsMedian = 0.0f;
    int editorWidth = 0;
    int editorHeight = 0;
    juce::ValueTree editorSize;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
