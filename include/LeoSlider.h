//
// Created by vboxuser on 17.07.2026.
//

#pragma once

#include "LeoExceptions.h"
#include "PluginProcessor.h"

#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <utility>
#include <cmath>

using namespace LeoExceptions;
using std::cout, std::endl, std::lround, std::vector;
using fvector = std::vector<float>;

struct LeoSliderInfo
{
    const int width, height;
    const int xPadding, yPadding;
    const size_t fontSize, backImgSize, thumbImgSize;
    const void* fontName;
    const void* backImgName;
    const void* thumbImgName;
};

struct LeoGainSliderInfo : LeoSliderInfo
{
    int numberOfDecimals;
    int textBoxWidth;
    int textBoxHeight;
};

class FixedSuffixLabel final : public juce::Component
{
public:
    FixedSuffixLabel(juce::String  s, juce::Typeface::Ptr  tf)
        : suffix(std::move(s)), typeface(std::move(tf))
    {}

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour::fromRGB(33, 33, 29));

        juce::Font suffixFont(juce::FontOptions{typeface});
        suffixFont.setHeight(static_cast<float>(getHeight()) * 0.75f);   // scale as you like

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
    LeoSliderLNF(){Ini();cout<<"Empty LNF const"<<endl;};
    LeoSliderLNF(const LeoSliderInfo& info);
    explicit LeoSliderLNF (const LeoGainSliderInfo& LNFGainInfo);
    //copy constr
    LeoSliderLNF(const LeoSliderLNF& other):
        m_backgroundImage{other.m_backgroundImage},
        m_thumbImage{other.m_thumbImage},
        m_fontFile{other.m_fontFile},
        m_pngBackground{other.m_pngBackground},
        m_pngThumb {other.m_pngThumb},
        m_fontData{other.m_fontData},
        m_fontTypeface{other.m_fontTypeface},
        //m_sliderLabel{other.m_sliderLabel},
        m_editorScalarWidth{other.m_editorScalarWidth},
        m_editorScalarHeight{other.m_editorScalarHeight}
    {};
    //move const
    LeoSliderLNF(LeoSliderLNF&& other) noexcept:
        m_backgroundImage{std::move(other.m_backgroundImage)},
        m_thumbImage{std::move(other.m_thumbImage)},
        m_fontFile{std::move(other.m_fontFile)},
        m_pngBackground{std::move(other.m_pngBackground)},
        m_pngThumb {std::move(other.m_pngThumb)},
        m_fontData{std::move(other.m_fontData)},
        m_fontTypeface{std::move(other.m_fontTypeface)},
        //m_sliderLabel{other.m_sliderLabel},
        m_editorScalarWidth{other.m_editorScalarWidth},
        m_editorScalarHeight{other.m_editorScalarHeight}
    {
        suffixListeners.clear();
        for (auto i = other.suffixListeners.begin(); i != other.suffixListeners.end(); ++i)
            suffixListeners.set(i.getKey(), i.getValue());

        suffixComponents.clear();
        for (auto i = other.suffixComponents.begin(); i != other.suffixComponents.end(); ++i)
            suffixComponents.set(i.getKey(), i.getValue());

        other.m_editorScalarWidth = 0.0f;
        other.m_editorScalarHeight = 0.0f;
    };
    //assigment
    LeoSliderLNF& operator=(const LeoSliderLNF& other)
    {
        if (&other != this)
        {
            LeoSliderLNF tmp(other);
            std::swap(*this, tmp);
        }
        return *this;
    }
    //move
    LeoSliderLNF& operator=(LeoSliderLNF&& other) noexcept
    {
        if (this != &other)
        {
            std::swap(m_backgroundImage, other.m_backgroundImage);
            std::swap(m_thumbImage, other.m_thumbImage);
            std::swap(m_fontFile, other.m_fontFile);
            std::swap(m_pngBackground, other.m_pngBackground);
            std::swap(m_pngThumb, other.m_pngThumb);
            std::swap(m_fontData, other.m_fontData);
            std::swap(m_fontTypeface, other.m_fontTypeface);
            std::swap(m_editorScalarWidth, other.m_editorScalarWidth);
            std::swap(m_editorScalarHeight, other.m_editorScalarHeight);
            suffixListeners.swapWith(other.suffixListeners);
            suffixComponents.swapWith(other.suffixComponents);
        }
        return *this;
    };
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

    juce::Font getLabelFont(juce::Label& /*label*/) override
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
                lround(static_cast<float>(x) + m_editorScalarWidth * 118),
                y,
                width,
                height,
                0,
                0,
                m_backgroundImage.getWidth(),
                m_backgroundImage.getHeight());
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
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const juce::Slider::SliderStyle /*style*/,
        juce::Slider& /*slider*/) override
    {
        if (!m_thumbImage.isNull())
        {
            const int thumbHeight = lround(static_cast<float>(m_thumbImage.getHeight()) * m_editorScalarHeight);
            const int thumbWidth = lround(static_cast<float>(m_thumbImage.getWidth()) * m_editorScalarWidth);
            const float drawX = static_cast<float>(x) + (static_cast<float>(width) - static_cast<float>(thumbWidth)) / 2.0f;
            const float drawY = sliderPos - static_cast<float>(thumbHeight) / 2.0f;

            g.drawImage(m_thumbImage,
                lround(drawX + 59 * m_editorScalarWidth),
                lround(drawY),
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

        const int sliderX = lround(0 * m_editorScalarWidth);
        const int sliderY = lround(50 * m_editorScalarHeight);
        const int sliderW = lround(119 * m_editorScalarWidth);
        const int sliderH = lround(544 * m_editorScalarHeight);

        const int textX = lround(-100 * m_editorScalarWidth);
        const int textY = lround(640 * m_editorScalarHeight);
        const int textW = lround(350 * m_editorScalarWidth);
        const int textH = lround(100 * m_editorScalarHeight);

        layout.sliderBounds = juce::Rectangle<int>(sliderX, sliderY, sliderW, sliderH);
        layout.textBoxBounds = juce::Rectangle<int>(textX, textY, textW, textH);

        return layout;
    }

    LEO_RETURN setScalar(const float& scalarWidth, const float& scalarHeight) {m_editorScalarWidth = scalarWidth; m_editorScalarHeight = scalarHeight; return LEO_SUCCESS;}
    LEO_RETURN setLNFfont(const void* fontData, const size_t& fontSize);
    LEO_RETURN setLNFimages(const void* bgImageData, const size_t& bgImageSize, const void* thumbImageData, const size_t& thumbImageSize);

    LEO_RETURN Ini();

    /*LeoSliderLNF operator = (this*, LeoSliderLNF& other)
    {

    };*/
private:
    juce::Image m_backgroundImage;
    juce::Image m_thumbImage;
    juce::File m_fontFile;
    juce::File m_pngBackground;
    juce::File m_pngThumb;
    juce::MemoryBlock m_fontData;
    juce::Typeface::Ptr m_fontTypeface;
    //juce::Label* m_sliderLabel {nullptr};
    float m_editorScalarWidth {0}, m_editorScalarHeight {0};
    juce::HashMap<juce::Label*, std::shared_ptr<SuffixPositionListener>> suffixListeners;
    juce::HashMap<juce::Label*, juce::Component*> suffixComponents;
};



class LeoSlider : public juce::Slider
{

public:
    LeoSlider() : LeoSlider{750, 750, 42, 419} {};
    LeoSlider(const int& widthOrHeight, const bool& widthTrue){widthTrue?LeoSlider(widthOrHeight,750, 42, 419):LeoSlider(750,widthOrHeight, 42, 419);};
    LeoSlider(const bool& xPaddingTrue, const int& padding){ xPaddingTrue?LeoSlider(750, 750, padding, 419):LeoSlider(750, 750, 42, padding);};
    LeoSlider(const int& xPadding, const int& yPadding) :LeoSlider{750, 750, xPadding, yPadding} {};
    LeoSlider(const int& width, const int& height, const int& xPadding, const int& yPadding);
    explicit LeoSlider(const LeoSliderInfo& leoInfo);
    explicit LeoSlider(const LeoGainSliderInfo& leoGainInfo);
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
    LEO_RETURN resizeSlider (const float scalarWidth, const float scalarHeight)
    {
        const int w = lround(static_cast<float>(m_defaultWidth) * scalarWidth);
        const int h = lround(static_cast<float>(m_defaultHeight) * scalarHeight);
        const int x = lround(static_cast<float>(m_xDefaultPadding) * scalarWidth);
        const int y = lround(static_cast<float>(m_yDefaultPadding) * scalarHeight);

        this->setBounds(x, y, w, h);
        return LEO_SUCCESS;
    }
private:
    int m_width {750},  m_height {750},  m_xPadding {42},  m_yPadding {419};
    int m_defaultWidth {0}, m_defaultHeight {0}, m_xDefaultPadding {0}, m_yDefaultPadding {0};
    static unsigned int m_numInstances;

    LeoSliderLNF m_leoSliderLNF {};
};

//------------------------------------------------------------------------------------------------------------------

class LeoGainSlider final : public LeoSlider
{
    public:
    //LeoGainSlider():LeoGainSlider(400, 50, 1){};
    LeoGainSlider() = delete;
    LeoGainSlider(const int& textBoxWidth, const int& textBoxHeight, const int& numOfDecimals);
    explicit LeoGainSlider(const LeoGainSliderInfo& lGSinfo);
    juce::String getTextFromValue(const double value) override
    {
        return {value, m_numberOfDecimals};
    };
    LeoSliderLNF* getLNF() const {return mPtr_parentLNF;};
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void setLearnState(const bool& state) {m_learnButtonState = state;};
private:
    int m_numberOfDecimals {1};
    int m_textBoxWidth {400};
    int m_textBoxHeight {50};
    int m_targetMeasurements {5}; //in seconds
    int m_measuredBlocks {1};
    int m_measuredBlocksRe {1};

    double m_lastSliderValue {0.0};
    double m_targetLoudness {-18.0f};
    float m_captureOffset {0.0f};
    float m_learnThreshold {0.5f};
    float m_rmsMedian {0.0f};
    float m_previousGain {0};

    bool m_capture {false};
    bool m_learnButtonState {false};

    enum LearnState {IDLE, MEASURING, LEARNING, END, RELEARNING};
    LearnState m_learnState {IDLE};

    fvector m_rmsValues;

    LeoSliderLNF* mPtr_parentLNF;
    //AudioPluginAudioProcessor* m_processorRef;
};