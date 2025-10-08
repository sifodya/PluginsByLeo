//
// Created by cedri on 05/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

class LeoBackground : public juce::Component

{
public:
    LeoBackground()
    {
        pngFile = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/full.png");
        if (pngFile.existsAsFile())
        {
            image = juce::ImageFileFormat::loadFrom(pngFile);
        }
    backgroundBounds.setSize(image.getWidth(), image.getHeight());

    }

    void LeoBackground::paint(juce::Graphics& g) override
    {
        if (image.isValid())
        {
            g.drawImage(image, backgroundBounds);
        }
        else
        {
            g.fillAll(juce::Colours::pink);
        }
    }
    void LeoBackground::resized() override
    {
    }
    juce::Image image;
private:
    juce::File pngFile;
    std::unique_ptr<juce::Drawable> pngDrawable;
    juce::Rectangle<float> backgroundBounds;
};