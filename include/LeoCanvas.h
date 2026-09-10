//
// Created by cedri on 08/09/2026.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class LeoCanvas : public juce::Component
{
    juce::Image m_image;
    juce::Rectangle<float> m_bounds;
    int m_xDefaultPadding, m_yDefaultPadding, m_defaultWidth, m_defaultHeight;

    public:
    LeoCanvas() = delete;
    LeoCanvas(const void* imageData, const size_t& imageSize,const int xPadding, const int yPadding, const int width, const int height)
    {
        m_defaultHeight = height;
        m_defaultWidth = width;
        m_xDefaultPadding = xPadding;
        m_yDefaultPadding = yPadding;
        m_image = juce::ImageFileFormat::loadFrom(imageData, imageSize);
        m_bounds.setSize(static_cast<float>(m_image.getWidth()), static_cast<float>(m_image.getHeight()));
    }

    void resizeFromEditor(juce::Rectangle<int> const size)
    {
        m_bounds.setSize(static_cast<float>(size.getWidth()), static_cast<float>(size.getHeight()));
    }

    void resizeCanvas(const float scalarWidth, const float scalarHeight)
    {
        const int w = static_cast<int>(lround(static_cast<float>(m_defaultWidth) * scalarWidth));
        const int h = static_cast<int>(lround(static_cast<float>(m_defaultHeight) * scalarHeight));
        const int x = static_cast<int>(lround(static_cast<float>(m_xDefaultPadding) * scalarWidth));
        const int y = static_cast<int>(lround(static_cast<float>(m_yDefaultPadding) * scalarHeight));
        this->setBounds(x, y, w, h);
        m_bounds.setSize(static_cast<float>(getWidth()), static_cast<float>(getHeight()));
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        if (m_image.isValid())
            g.drawImage(m_image, m_bounds);
        else
            g.fillAll(juce::Colours::pink);
    }
    juce::Image& getImage(){return m_image;};
};