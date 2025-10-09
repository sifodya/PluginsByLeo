//
// Created by cedri on 07/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>
#include "iostream"
class SimpleSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SimpleSliderLookAndFeel()
    {
        pngBackground = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/fader bg.png");
        pngThumb = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/fader.png");
        if (pngBackground.existsAsFile())
        {
            backgroundImage = juce::ImageFileFormat::loadFrom(pngBackground);
            std::cout<<"loaded backgroundImage"<<std::endl;
        }
        if (pngThumb.existsAsFile())
        {
            thumbImage = juce::ImageFileFormat::loadFrom(pngThumb);
            std::cout<<"loaded thumbImage"<<std::endl;
        }
    }
    juce::Image backgroundImage;
    juce::Image thumbImage;
    void drawLinearSlider(juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        drawLinearSliderBackground(g,x,y,width,height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb(g,x,y,width,height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
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
        std::cout<<"Draw Background"<<std::endl;
        if (!backgroundImage.isNull())
        {
            g.drawImage(backgroundImage,
                x, y, width, height,
                0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
            std::cout<<"Background is not null"<<std::endl;
        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillRect(x, y, width, height);
        }
    }
    void drawLinearSliderThumb(juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        std::cout<<"Draw Thumb"<<std::endl;
        if (!thumbImage.isNull())
        {
            const int thumbHeight = thumbImage.getHeight();
            const int thumbWidth = thumbImage.getWidth();
            float drawX, drawY = 0.0f;

            drawX = x + (width - thumbWidth) / 2.0f;
            drawY = sliderPos - thumbHeight / 2.0f;

            g.drawImage(thumbImage,
                drawX,
                drawY,
                thumbWidth,
                thumbHeight,
                0,
                0,
                thumbWidth,
                thumbHeight);
        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillEllipse(x, y, width, height);
        }
    }
    ~SimpleSliderLookAndFeel() override = default;
private:

    juce::File pngBackground;
    juce::File pngThumb;
};