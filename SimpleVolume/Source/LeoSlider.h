//
// Created by vboxuser on 17.07.2026.
//

#pragma once

//#include "SimpleSliderLookAndFeel.cpp"
#include "LeoExceptions.h"
#include "BinaryData.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <utility>

using namespace LeoExceptions;

class FixedSuffixLabel final : public juce::Component
{
public:
    FixedSuffixLabel(juce::String  s, juce::Typeface::Ptr  tf)
        : suffix(std::move(s)), typeface(std::move(tf))
    {}

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour::fromRGB(33, 33, 29));

        juce::Font suffixFont(typeface);
        suffixFont.setHeight(getHeight() * 0.75f);   // scale as you like

        g.setFont(suffixFont);
        g.drawFittedText(suffix, getLocalBounds(), juce::Justification::centredRight, 1);
    }

private:
    juce::String suffix;
    juce::Typeface::Ptr typeface;
};

//====================================================================
class SuffixPositionListener final: public juce::ComponentListener
{
public:
    explicit SuffixPositionListener(juce::Component* suffixComp)
        : suffix(suffixComp)
    {}

    void componentMovedOrResized(juce::Component& c, bool, bool) override
    {
        auto b = c.getLocalBounds();
        suffix->setBounds(b.removeFromRight(350));
    }

private:
    juce::Component* suffix;
};
//====================================================================

class LeoSliderLNF : public juce::LookAndFeel_V4
{
public:
    LeoSliderLNF(){Ini(false);};
    ~LeoSliderLNF() override;

    void drawLinearSlider(juce::Graphics& g,
        const int x,
        const int y,
        const int width,
        const int height,
        const float sliderPos,
        const float minSliderPos,
        const float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        drawLinearSliderThumb(g,x,y,width,height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderBackground(g,x,y,height,width,sliderPos,minSliderPos,maxSliderPos,style,slider);
        createSliderTextBox(slider);
    }

    juce::Font getLabelFont(juce::Label& label) override
    {
        return {juce::FontOptions{m_fontTypeface->getName(),m_fontTypeface->getStyle(), 70.0f * m_editorScalarHeight }};//.withTypeface(m_fontTypeface).withHeight(75.0f * m_editorScalarHeight).withStyle(m_fontTypeface->getStyle())}; // 65
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override;

    void drawLinearSliderBackground(juce::Graphics& g,
        const int x,
        const int y,
        const int height,
        const int width,
        float /*sliderPos*/,
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const juce::Slider::SliderStyle /*style*/,
        juce::Slider&) override
    {
        if (!m_backgroundImage.isNull())
        {
            g.drawImage(m_backgroundImage,
                x + m_editorScalarWidth * 118, y, width, height,
                0, 0, m_backgroundImage.getWidth(), m_backgroundImage.getHeight());
        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillRect(x, y, width, height);
        }
    }

    void drawLinearSliderThumb(juce::Graphics& g,
        const int x,
        const int y,
        const int width,
        const int height,
        const float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        if (!m_thumbImage.isNull())
        {
            const int thumbHeight = m_thumbImage.getHeight()*m_editorScalarHeight;
            const int thumbWidth = m_thumbImage.getWidth()*m_editorScalarWidth;
            float drawY = 0.0f;
            const float drawX = x + (width - thumbWidth) / 2.0f;
            drawY = sliderPos - thumbHeight / 2.0f;

            g.drawImage(m_thumbImage,
                static_cast<int>(drawX)+59*m_editorScalarWidth,
                static_cast<int>(drawY),
                thumbWidth,
                thumbHeight,
                0,
                0,
                m_thumbImage.getWidth(),
                m_thumbImage.getHeight());

        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillEllipse(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
        }
    }

    juce::Slider::SliderLayout getSliderLayout(juce::Slider&) override
    {
        juce::Slider::SliderLayout layout;
        layout.sliderBounds = juce::Rectangle<int>(0 * m_editorScalarWidth, 50 * m_editorScalarHeight,
                                                   119 * m_editorScalarWidth, 544 * m_editorScalarHeight);
        layout.textBoxBounds = juce::Rectangle<int>(-100 * m_editorScalarWidth, 640 * m_editorScalarHeight,
                                                    350 * m_editorScalarWidth, 100 * m_editorScalarHeight);

        return layout;
    }

    LEO_RETURN setScalar(const float& scalarWidth, const float& scalarHeight) {m_editorScalarWidth = scalarWidth; m_editorScalarHeight = scalarHeight; return LEO_SUCCESS;}
    LEO_RETURN setLNFfont(const void* fontData, const size_t& fontSize);
    LEO_RETURN setLNFimages(const void* bgImageData, const size_t& bgImageSize, const void* thumbImageData, const size_t& thumbImageSize);

    LEO_RETURN Ini(const bool& useBinaryData);
private:
    juce::Image m_backgroundImage;
    juce::Image m_thumbImage;
    juce::File m_fontFile;
    juce::File m_pngBackground;
    juce::File m_pngThumb;
    juce::MemoryBlock m_fontData;
    juce::Typeface::Ptr m_fontTypeface;
    juce::Label * m_sliderLabel;
    float m_editorScalarWidth, m_editorScalarHeight;
    juce::HashMap<juce::Label*, std::shared_ptr<SuffixPositionListener>> suffixListeners;
    juce::HashMap<juce::Label*, juce::Component*> suffixComponents;
};



class LeoSlider : public juce::Slider
{

public:
    LeoSlider() : LeoSlider{750, 750, 42, 419} {};
    LeoSlider(const int& widthOrHeight, const bool& widthTrue){widthTrue?LeoSlider(widthOrHeight,750, 42, 419):LeoSlider(750,widthOrHeight, 42, 419);};
    //LeoSlider(const int& width, const int& height) : LeoSlider{width, height, 42, 419} {};
    LeoSlider(const bool& xPaddingTrue, const int& padding){ xPaddingTrue?LeoSlider(750, 750, padding, 419):LeoSlider(750, 750, 42, padding);};
    LeoSlider(const int& xPadding, const int& yPadding) :LeoSlider{750, 750, xPadding, yPadding} {};
    LeoSlider(const int& width, const int& height, const int& xPadding, const int& yPadding);
    ~LeoSlider() override {setLookAndFeel(nullptr);};
    std::array<int, 2> getPadding() const {return std::array<int, 2>{m_xPadding,m_yPadding};};
    int getXPadding() const {return m_xPadding;};
    int getYPadding() const {return m_yPadding;};
    LEO_RETURN setPadding(const std::array<int, 2>& padding) {m_xPadding = padding[0]; m_yPadding = padding[1]; return LEO_SUCCESS;};
    LEO_RETURN setXPadding(const int& xPadding) {m_xPadding = xPadding; return LEO_SUCCESS;};
    LEO_RETURN setYPadding(const int& yPadding) {m_yPadding = yPadding; return LEO_SUCCESS;};
    LeoSliderLNF* getLeoLNF()
    {
        LeoSliderLNF* p_LeoSliderLNF = &m_leoSliderLNF;
        return p_LeoSliderLNF;
    };
private:
    int m_width {750};
    int m_height {750};
    int m_xPadding {42};
    int m_yPadding {419};
    LeoSliderLNF m_leoSliderLNF{};
};

//------------------------------------------------------------------------------------------------------------------





class LeoGainSlider final : public LeoSlider
{
    public:
    LeoGainSlider():LeoGainSlider(400, 50, 1){};
    LeoGainSlider(const int& textBoxWidth, const int& textBoxHeight, const int& numOfDecimals);
    juce::String getTextFromValue(double value) override
    {
        return juce::String(value, m_numberOfDecimals);
    };
    LeoSliderLNF* getLNF() const {return mPtr_parentLNF;};
private:
    int m_numberOfDecimals {1};
    int m_textBoxWidth {400};
    int m_textBoxHeight {50};
    LeoSliderLNF* mPtr_parentLNF;
};


