//
// Created by vboxuser on 17.07.2026.
//

#include "LeoSlider.h"


/*LeoSliderLNF::LeoSliderLNF(const bool& isDefault)
{
    Ini();
}*/

LeoSliderLNF::~LeoSliderLNF()
{
    //delete m_sliderLabel;
    /*for (auto it = suffixComponents.begin(); it != suffixComponents.end(); ++it)
    {
        if (auto* label = it.getKey())
            if (auto* suffix = it.getValue())
            {
                label->removeChildComponent(suffix);
            }
    }*/
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
    //auto suffix = new FixedSuffixLabel(" dB", m_fontTypeface);
    m_fixedSuffixLabel = std::make_shared<FixedSuffixLabel> (" dB", m_fontTypeface);
    m_fixedSuffixLabel->setInterceptsMouseClicks(false, false);
    label->addAndMakeVisible(*m_fixedSuffixLabel);

    // Create listener & store it so it stays alive
    const auto listener = std::make_shared<SuffixPositionListener>(m_fixedSuffixLabel.get());
    label->addComponentListener(listener.get());

    // Save both suffix and listener
    suffixComponents.set(label, m_fixedSuffixLabel.get());
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