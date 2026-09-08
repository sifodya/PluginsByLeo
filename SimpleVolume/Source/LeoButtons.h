//
// Created by cedri on 08/09/2026.
//
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


class LeoHyperLinkbutton final : public juce::HyperlinkButton
{
    juce::Typeface::Ptr m_typeface;
    int m_defaultWidth{300}, m_defaultHeight{40}, m_xDefaultPadding{34}, m_yDefaultPadding{1201};
    public:
    LeoHyperLinkbutton()
    {
        m_typeface = juce::Typeface::findSystemTypeface();
        const auto buttonFont = juce::Font(juce::FontOptions{m_typeface});
        setButtonText("Text missing");
        setURL(juce::URL("www.google.com"));
        setColour(textColourId, juce::Colours::black);
        setBounds(m_xDefaultPadding, m_yDefaultPadding, m_defaultWidth, m_defaultHeight);
    }
    LeoHyperLinkbutton(const void* fontData, const size_t& fontDataSize, const std::string& buttonText, const juce::URL& buttonURL, const int width, const int height, const int xPadding, const int yPadding)
    {
        m_defaultHeight = height;
        m_defaultWidth = width;
        m_xDefaultPadding = xPadding;
        m_yDefaultPadding = yPadding;
        m_typeface = juce::Typeface::createSystemTypefaceFor(fontData, fontDataSize);
        const auto buttonFont = juce::Font(juce::FontOptions{m_typeface});
        setButtonText(buttonText);
        setURL(buttonURL);
        setColour(textColourId, juce::Colours::black);
    }
    LeoHyperLinkbutton(const void* fontData, const size_t& fontDataSize, const std::string& buttonText, const std::string& buttonURL):LeoHyperLinkbutton(fontData, fontDataSize, buttonText, juce::URL(buttonURL), 300, 40, 34, 1201){}
    void resizeButton(const float scalarWidth, const float scalarHeight)
    {
        const int w = static_cast<int>(lround(static_cast<float>(m_defaultWidth) * scalarWidth));
        const int h = static_cast<int>(lround(static_cast<float>(m_defaultHeight) * scalarHeight));
        const int x = static_cast<int>(lround(static_cast<float>(m_xDefaultPadding) * scalarWidth));
        const int y = static_cast<int>(lround(static_cast<float>(m_yDefaultPadding) * scalarHeight));
        this->setBounds(x, y, w, h);
    }
    void setTypeface(const juce::Typeface::Ptr& typeface){m_typeface = typeface; repaint();}
    void setTypefaceByData(const void* fontData, const size_t& fontSize){m_typeface = juce::Typeface::createSystemTypefaceFor(fontData, fontSize); repaint();}
    void setText(const std::string& buttonText){setButtonText(buttonText); repaint();}
    void setButtonURL(const std::string& buttonURL){setURL(juce::URL(buttonURL));}
    void setTextColour(const juce::Colour colour){setColour(textColourId, colour); repaint();}
};

class LeoImageButton final : public juce::ImageButton
{
    juce::Image m_normalImage, m_overImage, m_downImage;
    int m_defaultWidth{104}, m_defaultHeight{41}, m_xDefaultPadding{110}, m_yDefaultPadding{365};

public:
    LeoImageButton()
    {
        m_normalImage.clear(juce::Rectangle(0,0,getWidth(), getHeight()), juce::Colours::pink);
        m_overImage.clear(juce::Rectangle(0,0,getWidth(), getHeight()), juce::Colours::pink);
        m_downImage.clear(juce::Rectangle(0,0,getWidth(), getHeight()), juce::Colours::pink);

        setImages(
            true,
            true,
            false,
            m_normalImage,
            1.0f,
            juce::Colour(),
            m_overImage,
            1.0f,
            juce::Colour(),
            m_downImage,
            1.0f,
            juce::Colour());
        setToggleable(true);
        setClickingTogglesState(true);
        setBounds(m_xDefaultPadding, m_yDefaultPadding, m_defaultWidth, m_defaultHeight);
    }
    LeoImageButton(const void* normalData, const size_t& normalSize, const void* overData, const size_t& overSize, const void* downData, const size_t& downSize):LeoImageButton(normalData, normalSize, overData, overSize, downData, downSize, 104, 41, 110, 365){}
    LeoImageButton(const void* normalData, const size_t& normalSize, const void* overData, const size_t& overSize, const void* downData, const size_t& downSize, const int width, const int height, const int xPadding, const int yPadding)
    {
        m_normalImage = juce::ImageFileFormat::loadFrom(normalData, normalSize);
        m_overImage = juce::ImageFileFormat::loadFrom(overData, overSize);
        m_downImage = juce::ImageFileFormat::loadFrom(downData, downSize);
        m_defaultHeight = height;
        m_defaultWidth = width;
        m_xDefaultPadding = xPadding;
        m_yDefaultPadding = yPadding;


        setImages(
            true,
            true,
            false,
            m_normalImage,
            1.0f,
            juce::Colour(),
            m_overImage,
            1.0f,
            juce::Colour(),
            m_downImage,
            1.0f,
            juce::Colour());
        setToggleable(true);
        setClickingTogglesState(true);
        setBounds(m_xDefaultPadding, m_yDefaultPadding, m_defaultWidth, m_defaultHeight);
    }
    void setImagesByData(const void* normalData, const size_t& normalSize, const void* overData, const size_t& overSize, const void* downData, const size_t& downSize)
    {
        m_normalImage = juce::ImageFileFormat::loadFrom(normalData, normalSize);
        m_overImage = juce::ImageFileFormat::loadFrom(overData, overSize);
        m_downImage = juce::ImageFileFormat::loadFrom(downData, downSize);

        setImages(
            true,
            true,
            false,
            m_normalImage,
            1.0f,
            juce::Colour(),
            m_overImage,
            1.0f,
            juce::Colour(),
            m_downImage,
            1.0f,
            juce::Colour());
        repaint();
    }
    void setImagesByFile (const juce::Image& normal, const juce::Image& over, const juce::Image& down)
    {
        setImages(
            true,
            true,
            false,
            normal,
            1.0f,
            juce::Colour(),
            over,
            1.0f,
            juce::Colour(),
            down,
            1.0f,
            juce::Colour());
        repaint();
    }
    void resizeButton(const float scalarWidth, const float scalarHeight)
    {
        const int w = static_cast<int>(lround(static_cast<float>(m_defaultWidth) * scalarWidth));
        const int h = static_cast<int>(lround(static_cast<float>(m_defaultHeight) * scalarHeight));
        const int x = static_cast<int>(lround(static_cast<float>(m_xDefaultPadding) * scalarWidth));
        const int y = static_cast<int>(lround(static_cast<float>(m_yDefaultPadding) * scalarHeight));
        this->setBounds(x, y, w, h);
    }
};