//
// Created by vboxuser on 17.07.2026.
//

#include "LeoSlider.h"
#include "LeoMacros.h"
#include "BinaryData.h"
#include "../SimpleVolume/Source/PluginEditor.h"

LeoSliderLNF::LeoSliderLNF(const LeoSliderInfo& info)
{
    cout<<"LNF with Slider Info"<<endl;
    m_fontTypeface = juce::Typeface::createSystemTypefaceFor(info.fontName, info.fontSize);
    m_backgroundImage = juce::ImageFileFormat::loadFrom(info.backImgName, info.backImgSize);
    m_thumbImage = juce::ImageFileFormat::loadFrom(info.thumbImgName, info.thumbImgSize);
    this->setColour(juce::Label::textColourId, juce::Colours::black);
}

LeoSliderLNF::LeoSliderLNF(const LeoGainSliderInfo& LNFGainInfo)
{
    cout<<"LNF with Gain Slider Info"<<endl;
    m_fontTypeface = juce::Typeface::createSystemTypefaceFor(LNFGainInfo.fontName, LNFGainInfo.fontSize);
    m_backgroundImage = juce::ImageFileFormat::loadFrom(LNFGainInfo.backImgName, LNFGainInfo.backImgSize);
    m_thumbImage = juce::ImageFileFormat::loadFrom(LNFGainInfo.thumbImgName, LNFGainInfo.thumbImgSize);
    this->setColour(juce::Label::textColourId, juce::Colours::black);
}

LeoSliderLNF::~LeoSliderLNF()
{
    cout<<"LNF Deconstructor"<<endl;
    /*for (auto it = suffixComponents.begin(); it != suffixComponents.end(); ++it)
    {
        if (auto* label = it.getKey())
            if (auto* suffix = it.getValue())
            {
                label->removeChildComponent(suffix);
            }

    }*/
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

LEO_RETURN LeoSliderLNF::Ini()
{
    cout<<"Ini"<<endl;
    m_fontTypeface = juce::Typeface::findSystemTypeface();
    m_backgroundImage.clear(juce::Rectangle{0,0,m_backgroundImage.getWidth(), m_backgroundImage.getHeight()}, juce::Colours::pink);
    m_thumbImage.clear(juce::Rectangle{0,0,m_thumbImage.getWidth(), m_thumbImage.getHeight()}, juce::Colours::black);
    this->setColour(juce::Label::textColourId, juce::Colours::black);
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

//======================================================================================================
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

    m_leoSliderLNF = std::move(LeoSliderLNF());
    setLookAndFeel(&m_leoSliderLNF);
    setBounds(m_xPadding, m_yPadding, m_width, m_height);

    ++m_numberOfInstances;
}

LeoSlider::LeoSlider(const LeoSliderInfo& leoInfo)
{
    cout<<"LeoSlider, LeoSliderInfo"<<endl;
    m_defaultWidth = leoInfo.width;
    m_defaultHeight = leoInfo.height;
    m_xPadding = leoInfo.xPadding;
    m_yPadding = leoInfo.yPadding;

    m_defaultWidth = leoInfo.width;
    m_defaultHeight = leoInfo.height;
    m_xDefaultPadding = leoInfo.xPadding;
    m_yDefaultPadding = leoInfo.yPadding;

    m_leoSliderLNF = std::move(LeoSliderLNF(leoInfo));
    setLookAndFeel(&m_leoSliderLNF);
    setBounds(m_xPadding, m_yPadding, m_width, m_height);
    ++m_numberOfInstances;
}

LeoSlider::LeoSlider(const LeoGainSliderInfo& leoGainInfo)
{
    cout<<"LeoSlider, LeoGainSliderInfo"<<endl;
    m_defaultWidth = leoGainInfo.width;
    m_defaultHeight = leoGainInfo.height;
    m_xPadding = leoGainInfo.xPadding;
    m_yPadding = leoGainInfo.yPadding;

    m_defaultWidth = leoGainInfo.width;
    m_defaultHeight = leoGainInfo.height;
    m_xDefaultPadding = leoGainInfo.xPadding;
    m_yDefaultPadding = leoGainInfo.yPadding;

    m_leoSliderLNF = std::move(LeoSliderLNF(leoGainInfo));
    setLookAndFeel(&m_leoSliderLNF);
    setBounds(m_xPadding, m_yPadding, m_width, m_height);
    ++m_numberOfInstances;
}

//======================================================================================================
LeoGainSlider::LeoGainSlider(const int& textBoxWidth, const int& textBoxHeight, const int& numOfDecimals)
{
    m_numberOfDecimals = numOfDecimals;
    m_textBoxHeight = textBoxHeight;
    m_textBoxWidth = textBoxWidth;
    mPtr_parentLNF = getLeoLNF();
    mPtr_parentLNF->setLNFfont(BinaryData::NeueHaasDisplayMediu_ttf, BinaryData::NeueHaasDisplayMediu_ttfSize);
    mPtr_parentLNF->setLNFimages(BinaryData::fader_bg_png, BinaryData::fader_bg_pngSize, BinaryData::fader_png, BinaryData::fader_pngSize);

    setTextBoxStyle(TextBoxBelow, false, m_textBoxWidth, m_textBoxHeight);
    setTextBoxIsEditable(true);
    setSliderStyle(LinearVertical);
    setColour(textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(textBoxHighlightColourId, juce::Colours::black);
    setColour(textBoxBackgroundColourId, juce::Colours::transparentBlack);
    setDoubleClickReturnValue(true, 0.0f, juce::ModifierKeys::ctrlModifier);
}

LeoGainSlider::LeoGainSlider(const LeoGainSliderInfo& lGSinfo)
    :LeoSlider(lGSinfo)
{
    m_numberOfDecimals = lGSinfo.numberOfDecimals;
    m_textBoxHeight = lGSinfo.textBoxHeight;
    m_textBoxWidth = lGSinfo.textBoxWidth;

    mPtr_parentLNF = getLeoLNF();

    setTextBoxStyle(TextBoxBelow, false, m_textBoxWidth, m_textBoxHeight);
    setTextBoxIsEditable(true);
    setSliderStyle(LinearVertical);
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
            this->setValue(0.0f, juce::sendNotification);
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