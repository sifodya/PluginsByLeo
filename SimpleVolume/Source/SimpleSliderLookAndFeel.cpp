//
// Created by cedri on 07/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

class SimpleSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SimpleSliderLookAndFeel()
    {
        //loading the custom font as a file from disk and validating it
        fontFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Git Repo/PluginsByLeo/PluginsByLeo/neue-haas-grotesk-display-pro/NeueHaasDisplayMediu.ttf");
        if (fontFile.existsAsFile())
            fontFile.loadFileAsData(fontData);

        //make typeface from fontFile and assining it to LeoFont
        fontTypeface = juce::Typeface::createSystemTypefaceFor(fontData.getData(), fontData.getSize());
        LeoFont = juce::Typeface::createSystemTypefaceFor(fontData.getData(), fontData.getSize());

        //loading custom slider images from disk
        pngBackground = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Git Repo/PluginsByLeo/PluginsByLeo/SimpleVolume/resources/fader bg.png");
        pngThumb = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Git Repo/PluginsByLeo/PluginsByLeo/SimpleVolume/resources/fader.png");

        //validating image files
        if (pngBackground.existsAsFile())
        {
            backgroundImage = juce::ImageFileFormat::loadFrom(pngBackground);
        }
        if (pngThumb.existsAsFile())
        {
            thumbImage = juce::ImageFileFormat::loadFrom(pngThumb);
        }
        //setting slider text colour to black
        setColour(juce::Label::textColourId, juce::Colours::black);
    }
    //public images of the slider -> need to be public?
    juce::Image backgroundImage;
    juce::Image thumbImage;
    ///<summary>
    ///overrides the drawLinearSlider function for customisation purposes.
    ///is responsible for calling the functions which are drawing the custom graphics and
    ///the custom text box.
    ///The function gets its member variable values from the slider set in the editor
    ///</summary>
    ///<param></param>
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

    ///<summary>
    ///Overrides the font of the label
    ///using the custom font set in the constructor and setting a size with scalar.
    ///</summary>
    ///<param></param>
    juce::Font getLabelFont(juce::Label& label) override
    {
        return juce::Font(fontTypeface).withHeight(75.0f * editorScalarHeight); //eigentlich 65
    }

    ///<summary>
    ///Overrides and creates a custom text box for the slider
    ///its setting text colour, position transparency of the background and the outline,
    ///as well as the edited colours.
    ///</summary>
    ///<param></param>
    juce::Label* SimpleSliderLookAndFeel::createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* label = LookAndFeel_V4::createSliderTextBox(slider);
        label->setColour(juce::Label::textColourId, juce::Colour(33, 33, 29));
        label->setJustificationType(juce::Justification::centred);
        label->setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        label->setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
        label->setColour(juce::Label::backgroundWhenEditingColourId, juce::Colours::transparentBlack);
        label->setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::transparentBlack);

        return label;
    }

    ///<summary>
    ///Overrides the function that draws the background.
    ///Is set to draw the custom image set in the constructor and scales it.
    ///Also, it checks if the image is not NULL otherwise it will fill the background with a colour.
    ///</summary>
    ///<params></params>
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
                x+118*editorScalarWidth, y, width, height,
                0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillRect(x, y, width, height);
        }
    }

    ///<summary>
    ///Overrides the function that draws the thumb.
    ///Is set to draw the custom image set in the constructor and scales it.
    ///In addition to the scaling it sets the thumb to the correct position on the slider.
    ///Also, it checks if the image is not NULL otherwise it will fill the background with a colour.
    ///</summary>
    ///<params></params>
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
            const int thumbHeight = thumbImage.getHeight()*editorScalarHeight;
            const int thumbWidth = thumbImage.getWidth()*editorScalarWidth;
            float drawX, drawY = 0.0f;
            drawX = x + (width - thumbWidth) / 2.0f;
            drawY = sliderPos - thumbHeight / 2.0f;

            g.drawImage(thumbImage,
                drawX+59*editorScalarWidth,
                drawY,
                thumbWidth,
                thumbHeight,
                0,
                0,
                thumbImage.getWidth(),
                thumbImage.getHeight());

        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillEllipse(x, y, width, height);
        }
    }

    ///<summary>
    ///Overrides the slider layout. Allows to create a new slider layout in this case
    ///to set the text box further below the slider
    ///</summary>
    juce::Slider::SliderLayout getSliderLayout(juce::Slider&) override
{
    juce::Slider::SliderLayout layout;
        layout.sliderBounds = juce::Rectangle<int>(0*editorScalarWidth, 0*editorScalarHeight, 119*editorScalarWidth, 544*editorScalarHeight);
        layout.textBoxBounds = juce::Rectangle<int>(0*editorScalarWidth, 600*editorScalarHeight, 250*editorScalarWidth, 100*editorScalarHeight);

    return layout;
}

    //variables for scaling, set by the editor
    float editorScalarWidth, editorScalarHeight;
    ~SimpleSliderLookAndFeel() override = default;
private:
    juce::File fontFile; //containing the font file
    juce::File pngBackground; //containing the png for the background
    juce::File pngThumb; //containing the png for the thumb
    juce::MemoryBlock fontData; //data block for the font File
    juce::Typeface::Ptr fontTypeface; //typeface to be set by the font file
    juce::Font LeoFont; //complete custom font
    juce::Label *sliderLabel; //custom lable
};