//
// Created by cedri on 15/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleImageButton : public juce::ImageButton
{
public:
    SimpleImageButton()
    {
        normalPNG = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/learn_normal.png");
        overPNG = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/learn_yellow.png");
        downPNG = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/learn_red.png");
        if (normalPNG.existsAsFile())
        {
            normalImage = juce::ImageFileFormat::loadFrom(normalPNG);
        }
        if (overPNG.existsAsFile())
        {
            overImage = juce::ImageFileFormat::loadFrom(overPNG);
        }
        if (downPNG.existsAsFile())
        {
            downImage = juce::ImageFileFormat::loadFrom(downPNG);
        }
        setImages(true,
            true,
            false,
            normalImage,
            1.0f,
            juce::Colour(),
            overImage,
            1.0f,
            juce::Colour(),
            downImage,
            1.0f,
            juce::Colour());
        setToggleable(true);
        setClickingTogglesState(true);
    }
private:
    juce::Image normalImage;
    juce::Image overImage;
    juce::Image downImage;
    juce::File normalPNG;
    juce::File overPNG;
    juce::File downPNG;
};
