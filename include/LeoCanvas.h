//
// Created by cedri on 08/09/2026.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class LeoCanvas : public juce::Component
{
    juce::Image m_image;
    juce::Rectangle<float> m_bounds;

    public:
    LeoCanvas() = delete;
    LeoCanvas(const void* imageData, const size_t& imageSize)
    {
        m_image = juce::ImageFileFormat::loadFrom(imageData, imageSize);
        m_bounds.setSize(static_cast<float>(m_image.getWidth()), static_cast<float>(m_image.getHeight()));
    }

    void resizeFromEditor(juce::Rectangle<int> const size)
    {
        m_bounds.setSize(static_cast<float>(size.getWidth()), static_cast<float>(size.getHeight()));
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