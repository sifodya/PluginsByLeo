//
// Created by cedri on 07/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SimpleSliderLookAndFeel()
    {
        pngBackground = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/fader bg.png");
        if (pngBackground.existsAsFile())
        {
            backgroundImage = juce::ImageFileFormat::loadFrom(pngBackground);
        }
    }
    juce::Image backgroundImage;
    void drawLinearSliderBackground(juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float /*sliderPos*/,
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const juce::Slider::SliderStyle /*style*/,
        juce::Slider&) override
    {
        g.fillAll(juce::Colours::transparentBlack);

        if (!backgroundImage.isNull())
        {
            g.drawImage(backgroundImage,
                x, y, width, height,
                0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
        }
        else
        {
            g.setColour(juce::Colours::orange);
            g.fillRect(x, y, width, height);
        }
    }
    ~SimpleSliderLookAndFeel() override;
private:

    juce::File pngBackground;
};