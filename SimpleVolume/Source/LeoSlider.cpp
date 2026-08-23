//
// Created by vboxuser on 17.07.2026.
//

#include "LeoSlider.h"
#include "LeoMacros.h"
#include "BinaryData.h"
#include "PluginEditor.h"

/*LeoSliderLNF::LeoSliderLNF(const bool& isDefault)
{
    Ini();
}*/

LeoSliderLNF::~LeoSliderLNF()
{
    cout<<"LNF Deconstructor"<<endl;
    for (auto it = suffixComponents.begin(); it != suffixComponents.end(); ++it)
    {
        if (auto* label = it.getKey())
            if (auto* suffix = it.getValue())
            {
                label->removeChildComponent(suffix);
            }

    }
    //delete m_sliderLabel;
}

juce::Label* LeoSliderLNF::createSliderTextBox(juce::Slider& slider)
{
    juce::Label* label = LookAndFeel_V4::createSliderTextBox(slider);
    label->setColour(juce::Label::textColourId, juce::Colour(33, 33, 29));
    label->setJustificationType(juce::Justification::centred);
    label->onEditorShow = [label]
    {
        if (auto* editor = label->getCurrentTextEditor())
        {
            editor->setJustification(juce::Justification::centred);
        }
    };
    label->setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    label->setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
    label->setColour(juce::Label::backgroundWhenEditingColourId, juce::Colours::transparentBlack);
    label->setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::transparentBlack);
    // Remove JUCE’s suffix behaviour (important)
    slider.setTextValueSuffix("");

    // Create fixed “ dB” component
    auto suffix = new FixedSuffixLabel(" dB", m_fontTypeface);
    suffix->setInterceptsMouseClicks(false, false);
    label->addAndMakeVisible(*suffix);

    // Create listener & store it so it stays alive
    const auto listener = std::make_shared<SuffixPositionListener>(suffix);
    label->addComponentListener(listener.get());

    // Save both suffix and listener
    suffixComponents.set(label, suffix);
    suffixListeners.set(label, listener);

    // Force at least one layout now
    listener->componentMovedOrResized(*label, true, true);

    return label;
}

LEO_RETURN LeoSliderLNF::Ini(const bool& useBinaryData)
{
    if (useBinaryData)
    {
        m_fontTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::NeueHaasDisplayMediu_ttf, BinaryData::NeueHaasDisplayMediu_ttfSize);
        m_backgroundImage = juce::ImageFileFormat::loadFrom(BinaryData::fader_bg_png, BinaryData::fader_bg_pngSize);
        m_thumbImage = juce::ImageFileFormat::loadFrom(BinaryData::fader_png, BinaryData::fader_pngSize);
        this->setColour(juce::Label::textColourId, juce::Colours::black);
    }
    {
        m_fontTypeface = juce::Typeface::findSystemTypeface();
        this->setColour(juce::Label::textColourId, juce::Colours::black);
    }
    return LEO_SUCCESS;
}

LEO_RETURN LeoSliderLNF::setLNFfont(const void* fontData, const size_t& fontSize)
{
    m_fontTypeface = juce::Typeface::createSystemTypefaceFor(fontData, fontSize);
    setColour(juce::Label::textColourId, juce::Colours::black);
    return LEO_SUCCESS;
}
LEO_RETURN LeoSliderLNF::setLNFimages(const void* bgImageData, const size_t& bgImageSize, const void* thumbImageData, const size_t& thumbImageSize)
{
    m_backgroundImage = juce::ImageFileFormat::loadFrom(bgImageData, bgImageSize);
    m_thumbImage = juce::ImageFileFormat::loadFrom(thumbImageData, thumbImageSize);
    return LEO_SUCCESS;
}

LeoSlider::LeoSlider(const int& width, const int& height, const int& xPadding, const int& yPadding)
{
    m_width = width;
    m_height = height;
    m_xPadding = xPadding;
    m_yPadding = yPadding;

    m_defaultWidth = width;
    m_defaultHeight = height;
    m_xDefaultPadding = xPadding;
    m_yDefaultPadding = yPadding;

    setLookAndFeel(&m_leoSliderLNF);
    setBounds(m_xPadding, m_yPadding, m_width, m_height);
}

LeoGainSlider::LeoGainSlider(const int& textBoxWidth, const int& textBoxHeight, const int& numOfDecimals)
{
    mPtr_parentLNF = getLeoLNF();
    mPtr_parentLNF->setLNFfont(BinaryData::NeueHaasDisplayMediu_ttf, BinaryData::NeueHaasDisplayMediu_ttfSize);
    mPtr_parentLNF->setLNFimages(BinaryData::fader_bg_png, BinaryData::fader_bg_pngSize, BinaryData::fader_png, BinaryData::fader_pngSize);

    m_numberOfDecimals = numOfDecimals;

    setTextBoxStyle(TextBoxBelow, false, textBoxWidth, textBoxHeight);
    setTextBoxIsEditable(true);
    setSliderStyle(LinearVertical);
    /*textFromValueFunction = [this](const double value){
        return juce::String(value, m_numberOfDecimals);
    };
    //addMouseListener(this, false);
    setNumDecimalPlacesToDisplay(m_numberOfDecimals);*/
    //gainSlider.setValue(0.0f);
    setColour(textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(textBoxHighlightColourId, juce::Colours::black);
    setColour(textBoxBackgroundColourId, juce::Colours::transparentBlack);
    setDoubleClickReturnValue(true, 0.0f, juce::ModifierKeys::ctrlModifier);
}

void LeoGainSlider::mouseDown(const juce::MouseEvent& event)
{
    if (event.eventComponent == this)
    {
        m_lastSliderValue = this->getValue();
        if (event.mods.isCtrlDown())
            this->setValue(0.0f, juce::dontSendNotification);
    }
    m_captureOffset = event.position.y;
}

void LeoGainSlider::mouseDrag(const juce::MouseEvent& event)
{
    if (event.eventComponent != this)
        return;
    float fineFactor, pixelDelta;
    if (event.mods.isShiftDown())
    {
        if (m_capture==false)
        {
            m_captureOffset = event.position.y;
            if (event.eventComponent == this)
                m_lastSliderValue = this->getValue();
            m_capture = true;
        }
        fineFactor = 0.2f;
        pixelDelta = (event.position.y-m_captureOffset) * -1.0f;//-(event.position.y - (event.mouseDownPosition.y - captureOffset));
    }
    else
    {
        if (m_capture==true)
        {
            m_captureOffset = event.position.y;
            if (event.eventComponent == this)
                m_lastSliderValue = this->getValue();
            m_capture = false;
        }
        fineFactor = 1.0f;
        pixelDelta = (event.position.y-m_captureOffset) * -1.0f;//-(event.position.y - event.mouseDownPosition.y);
    }
    // Slider height controls default sensitivity
    const float sliderLength = this->getLookAndFeel().getSliderLayout(*this).sliderBounds.getHeight();

    const float valueDelta = pixelDelta / sliderLength
                       * (this->getMaximum() - this->getMinimum())
                       * fineFactor;
    this->setValue(m_lastSliderValue + valueDelta, juce::sendNotificationSync);
}

LEO_RETURN LeoGainSlider::DSP(juce::AudioBuffer<float>& buffer, const int& totalNumInputChannels, const int& totalNumOutputChannels, AudioPluginAudioProcessor* processor)
{
    const auto& apvts = processor->getParameters();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const double gainAmplitude = processor->getGainParameter()->load();
        const double currentGain = dbToLin(gainAmplitude));

        if (!m_learnButtonState)
        {
            m_learnState=IDLE;
            m_rmsValues.clear();
            m_measuredBlocks = 1;
            m_measuredBlocksRe = 1;
        }

        switch (m_learnState)
        {
            case IDLE:
                {
                    if(m_learnButtonState)
                        m_learnState = LEARNING;
                    break;
                }
        case MEASURING:
            {
                 float rmsSum {0};
                    for (auto i {0}; i < m_rmsValues.size(); i++)
                        rmsSum = rmsSum + m_rmsValues[i];
                    float rmsAverage = rmsSum/m_rmsValues.size();
                    sort(m_rmsValues.begin(), m_rmsValues.end());
                    if (m_rmsValues.size() % 2 != 0)
                        m_rmsMedian = m_rmsValues[m_rmsValues.size()/2];
                    else
                        m_rmsMedian = (m_rmsValues[(m_rmsValues.size()-1)/2] + m_rmsValues[m_rmsValues.size()/2])/2;
                    if (abs(rmsAverage-m_rmsMedian)<m_learnThreshold)
                    {
                        float valueToSet = 0;
                        if (m_rmsMedian != 0)
                        {
                            //const auto rmsMedianDb = 20.0f * log10(rmsMedian/0.707f);
                            auto target = m_targetLoudness - m_rmsMedian;
                            //target = target + 3.0f;
                            valueToSet = target;
                            cout<<"Value to set DB: "<<target<<" rms Median db :"<<m_rmsMedian<<" Sum: "<<rmsSum<<endl;
                        }
                        else
                            valueToSet = m_targetLoudness;

                        //cout<<"Value to Set: "<<valueToSet<<" Target: "<<targetLoudnessLin<<" Median: "<<m_rmsMedian<<" Avg: "<<rmsAverage<<endl;
                        apvts.getParameter("gain")->setValueNotifyingHost(apvts.getParameter("gain")->convertTo0to1(valueToSet));
                        m_learnState = END;
                        break;
                    }
                m_learnState = RELEARNING;
                break;
            }
            case LEARNING:
            {

                    cout<<"Learning"<<endl;
                    if (!processor->getPlayHead()->getPosition()->getIsPlaying())
                        return LEO_PLAYHEAD_STOP;
                    cout<<"Measure Start"<<endl;
                if (m_measuredBlocks <= m_targetMeasurements * processor->getSampleRate()/buffer.getNumSamples())
                {
                    auto rms = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
                    rms = 20.0f * log10(rms/0.707f);
                    rms = round(rms * 100.0f) / 100.0f;
                    m_rmsValues.push_back(rms);
                    m_measuredBlocks++;
                    //cout<<"RMS: "<<rms<<endl;
                }
                else
                    {
                        m_learnState = MEASURING;
                    }
                    //cout<<"Measured Blocks: "<<measuredBlocks<<endl;
                break;
            }
            case END:
            {
                if (const auto editor = dynamic_cast<AudioPluginAudioProcessorEditor*>(getParentComponent()))
                {
                    editor->learnButtonImage.setToggleState(false, juce::dontSendNotification);
                    m_learnButtonState = false;
                    m_measuredBlocks = 1;
                    m_learnState = IDLE;
                }
                break;
            }
            case RELEARNING:
            {
                    if (m_measuredBlocksRe <= m_targetMeasurements * processor->getSampleRate() / buffer.getNumSamples()/5)
                    {
                        float rms = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
                        rms = 20.0f * log10(rms/0.707f);
                        rms = round(rms * 100.0f) / 100.0f;
                        m_rmsValues.push_back(rms);
                        m_measuredBlocksRe++;
                    }
                    else
                    {
                        m_learnState = MEASURING;
                        m_measuredBlocksRe = 1;
                    }
                break;
            }
        }
        if (juce::approximatelyEqual (static_cast<float>(currentGain), m_previousGain))
        {
            buffer.applyGain (channel, 0, buffer.getNumSamples(), currentGain);
        }
        else
        {
            buffer.applyGainRamp (channel, 0, buffer.getNumSamples(), m_previousGain, currentGain);
            m_previousGain = currentGain;
        }
    }
    return LEO_SUCCESS;
}
