//
// Created by cedri on 14/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class LeoPluginTitle final : public juce::Component
{
public:
    LeoPluginTitle()
    {
        titleImage = juce::ImageFileFormat::loadFrom(BinaryData::ueberschrift_png, BinaryData::ueberschrift_pngSize);

        titleBounds.setSize(static_cast<float>(titleImage.getWidth()), static_cast<float>(titleImage.getHeight()));
    }

    void resizeFromEditor(juce::Rectangle<int> const size)
    {
        titleBounds.setSize(static_cast<float>(size.getWidth()), static_cast<float>(size.getHeight()));
    }

    void paint(juce::Graphics& g) override
    {
        if (titleImage.isValid())
            g.drawImage(titleImage, titleBounds);
        else
            g.fillAll(juce::Colours::pink);
    }

    void resized() override
    {

    }
    juce::Image titleImage;
private:
    juce::File pngTitleFile;
    juce::Rectangle<float> titleBounds;
};