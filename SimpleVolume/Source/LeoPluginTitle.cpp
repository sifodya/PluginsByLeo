//
// Created by cedri on 14/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

class LeoPluginTitle : public juce::Component
{
public:
    LeoPluginTitle()
    {
    pngTitleFile = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/ueberschrift.png");
        if (pngTitleFile.existsAsFile())
            titleImage = juce::ImageFileFormat::loadFrom(pngTitleFile);
        titleBounds.setSize(titleImage.getWidth(), titleImage.getHeight());
    }

    void LeoPluginTitle::resizeFromEditor(juce::Rectangle<int> size)
    {
        titleBounds.setSize(size.getWidth(), size.getHeight());
    }

    void LeoPluginTitle::paint(juce::Graphics& g) override
    {
        if (titleImage.isValid())
            g.drawImage(titleImage, titleBounds);
        else
            g.fillAll(juce::Colours::pink);
    }

    void LeoPluginTitle::resized() override
    {

    }
    juce::Image titleImage;
private:
    juce::File pngTitleFile;
    juce::Rectangle<float> titleBounds;
};