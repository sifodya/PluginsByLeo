//
// Created by cedri on 07/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SimpleSliderLookAndFeel()
    {
        fontFile = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/neue-haas-grotesk-display-pro/NeueHaasDisplayMediu.ttf");
        if (fontFile.existsAsFile())
            fontFile.loadFileAsData(fontData);

        fontTypeface = juce::Typeface::createSystemTypefaceFor(fontData.getData(), fontData.getSize());
        LeoFont = juce::Typeface::createSystemTypefaceFor(fontData.getData(), fontData.getSize());

        pngBackground = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/fader bg.png");
        pngThumb = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/SimpleVolume/resources/fader.png");

        if (pngBackground.existsAsFile())
        {
            backgroundImage = juce::ImageFileFormat::loadFrom(pngBackground);
        }
        if (pngThumb.existsAsFile())
        {
            thumbImage = juce::ImageFileFormat::loadFrom(pngThumb);
        }
        setColour(juce::Label::textColourId, juce::Colours::black);
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
        drawLinearSliderThumb(g,x,y,width,height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderBackground(g,x,y,height,width,sliderPos,minSliderPos,maxSliderPos,style,slider);
        createSliderTextBox(slider);
    }

    juce::Font getLabelFont(juce::Label& label) override
    {
        return juce::Font(fontTypeface).withHeight(16.0f);
    }

    juce::Label* SimpleSliderLookAndFeel::createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* label = LookAndFeel_V4::createSliderTextBox(slider);
        label->setColour(juce::Label::textColourId, juce::Colours::black);
        label->setJustificationType(juce::Justification::centred);

        return label;
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        // Transparent background and outline
        g.fillAll(juce::Colours::transparentBlack);

        if (label.isBeingEdited())
        {
            g.setColour(juce::Colours::transparentBlack); // No outline
        }
        else
        {
            g.setColour(juce::Colours::transparentBlack); // Also no outline when not editing
        }

        //g.drawRect(label.getLocalBounds(), 0); // No border width

        g.drawFittedText(label.getText(), label.getLocalBounds(), juce::Justification::centred, 1);
    }

    void drawLinearSliderBackground(juce::Graphics& g,
        int x,
        int y,
        int height,
        int width,
        float /*sliderPos*/,
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const juce::Slider::SliderStyle /*style*/,
        juce::Slider&) override
    {
        if (!backgroundImage.isNull())
        {
            g.drawImage(backgroundImage,
                x, y, width, height,
                0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
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
    juce::File fontFile;
    juce::File pngBackground;
    juce::File pngThumb;
    juce::MemoryBlock fontData;
    juce::Typeface::Ptr fontTypeface;
    juce::Font LeoFont;
    juce::Label *sliderLabel;
};