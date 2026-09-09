#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "LeoEvents.h"

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
    enum LearnState {IDLE, MEASURING, LEARNING, END, RELEARNING};
    juce::ValueTree editorSize {"EditorSize", {},
        {
            {"Group", {{"name", "lastSize"}},
                {
                {"Property", {{"id", "editorWidth"}, {"value", 0}}},
                    {"Property", {{"id", "editorHeight"}, {"value", 0}}}
                }
            }
        }
    };
    //------------------------------------------------------------------------------
public:
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

protected:
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

public:
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    bool hasEditor() const override;

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

    //Utility
    //==============================================================================
    void setLearnButtonState(const bool newState) {learnButtonState = newState;}
    int getEditorWidth() const {return editorWidth;};
    int getEditorHeight() const {return editorHeight;};
    void setEditorSize(int width, int height);
    bool getButtonStateToSet () const {return learnButtonToSet;}
    std::atomic<float>* getGainParameter() const {return gainParameter;};
    void getEditorSize(const int width, const int height)
    {
        editorWidth = width;
        editorHeight = height;
    }
    bool getLearnStateIdle() const {if (m_learnState == IDLE || m_learnState == END) return true;  return false;};
    bool getLearnStateMeasuring() const {if (m_learnState == MEASURING) return true;  return false;};

    //==============================================================================
    void learnGain(int channel, const juce::AudioBuffer<float>& buffer);

    //------------------------------------------------------------------------------
private:
    std::atomic<float>* gainParameter = nullptr;
    float previousGain {0};
    double targetLoudnessLin {pow(10.0f, -18.0f/20.0f)}, targetLoudness {-18.0f};
    float learnThreshold = 0.5f;
    float rmsMedian {0.0f};

    bool learnButtonState {false};
    bool learnButtonToSet {true};

    int targetMeasurements {5}; //in seconds
    int measuredBlocks {1}, measuredBlocksRe {1};
    int editorWidth {0}, editorHeight {0};

    std::vector <float> rmsValues;

    LearnState m_learnState {IDLE};

    juce::AudioProcessorValueTreeState parameters;
    //==============================================================================
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorEditor* createEditor() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
