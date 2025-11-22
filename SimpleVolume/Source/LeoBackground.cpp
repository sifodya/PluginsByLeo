//
// Created by cedri on 05/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

#include "BinaryData.h"
/// <summary>
/// This class is used to create a background image for the plugin. It is called in PluginEditor.
/// Further it references the bounds of the plugin parent window and is therefore responsible for
/// the entire resize calculations.
/// </summary>
class LeoBackground final : public juce::Component

{
public:
    LeoBackground()
    {
        ///<summary>
        ///This function is used to load the background image from the resources folder.
        ///In addition it checks if the file exists and loads it into the image container.
        ///Then it sets the bounds of the background image to the size of the image.
        ///</summary>
        image = juce::ImageFileFormat::loadFrom(BinaryData::bg_png, BinaryData::bg_pngSize);

    backgroundBounds.setSize(static_cast<float>(image.getWidth()), static_cast<float>(image.getHeight()));
    }
/// <summary>
/// This function is used by the PluginEditor to pass on the values of the resized parent window.
/// This is done in order to resize the background image to the size of the parent window.
/// </summary>
/// <param name="size"></param>
    void resizeFromEditor(juce::Rectangle<int> const size)
    {
        backgroundBounds.setSize(static_cast<float>(size.getWidth()), static_cast<float>(size.getHeight()));
    }
/// <summary>
/// This function is used to paint the background image to the bounds of this component
/// </summary>
/// <param name="g"></param>
    void paint(juce::Graphics& g) override
    {
        if (image.isValid())
            g.drawImage(image, backgroundBounds);
        else
            g.fillAll(juce::Colours::pink);
    }
    //========================================================================================
    juce::Image image; //image container for png | is public to be accessed by PluginEditor to get the size
private:
    //========================================================================================
    juce::File pngFile; //Holds the pngFile
    juce::Rectangle<float> backgroundBounds; //Holds the bounds of the background image
};