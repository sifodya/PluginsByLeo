//
// Created by cedri on 15/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class SimpleImageButton final: public juce::ImageButton
{
public:
    SimpleImageButton()
    {
        normalImage = juce::ImageFileFormat::loadFrom(BinaryData::learn_normal_png, BinaryData::learn_normal_pngSize);
        overImage = juce::ImageFileFormat::loadFrom(BinaryData::learn_yellow_png, BinaryData::learn_yellow_pngSize);
        downImage = juce::ImageFileFormat::loadFrom(BinaryData::learn_red_png, BinaryData::learn_red_pngSize);

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
