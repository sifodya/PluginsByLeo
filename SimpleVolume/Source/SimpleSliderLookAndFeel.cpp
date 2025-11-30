//
// Created by cedri on 07/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>

#include <utility>
#include "BinaryData.h"

class SimpleSliderLookAndFeel final: public juce::LookAndFeel_V4
{
public:
    SimpleSliderLookAndFeel()
    {


        //make typeface from fontFile and assigning it to LeoFont
        fontTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::NeueHaasDisplayMediu_ttf, BinaryData::NeueHaasDisplayMediu_ttfSize);

        //loading custom slider images from disk

        backgroundImage = juce::ImageFileFormat::loadFrom(BinaryData::fader_bg_png, BinaryData::fader_bg_pngSize);
        thumbImage = juce::ImageFileFormat::loadFrom(BinaryData::fader_png, BinaryData::fader_pngSize);

        //setting slider text colour to black
        setColour(juce::Label::textColourId, juce::Colours::black);
    }
    //public images of the slider -> need to be public?
    juce::Image backgroundImage;
    juce::Image thumbImage;
    /// <summary>
    /// overrides the drawLinearSlider function for customisation purposes.
    /// is responsible for calling the functions which are drawing the custom graphics and
    /// the custom text box.
    /// The function gets its member variable values from the slider set in the editor
    /// </summary>
    /// <param></param>
    void drawLinearSlider(juce::Graphics& g,
        const int x,
        const int y,
        const int width,
        const int height,
        const float sliderPos,
        const float minSliderPos,
        const float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        drawLinearSliderThumb(g,x,y,width,height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderBackground(g,x,y,height,width,sliderPos,minSliderPos,maxSliderPos,style,slider);
        createSliderTextBox(slider);
    }

    /// <summary>
    /// Overrides the font of the label
    /// using the custom font set in the constructor and setting a size with scalar.
    /// </summary>
    /// <param></param>
    juce::Font getLabelFont(juce::Label& label) override
    {
        return juce::Font(fontTypeface).withHeight(75.0f * editorScalarHeight); //eigentlich 65
    }

    /// <summary>
    /// Overrides and creates a custom text box for the slider
    /// its setting text colour, position transparency of the background and the outline,
    /// as well as the edited colours.
    /// </summary>
    /// <param></param>
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* label = LookAndFeel_V4::createSliderTextBox(slider);
        label->setColour(juce::Label::textColourId, juce::Colour(33, 33, 29));
        label->setJustificationType(juce::Justification::centred);
        label->onEditorShow = [label]
        {
            if (auto* editor = label->getCurrentTextEditor())
            {
                editor->setJustification(juce::Justification::centred);
            }
        };
        label->setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        label->setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
        label->setColour(juce::Label::backgroundWhenEditingColourId, juce::Colours::transparentBlack);
        label->setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::transparentBlack);
        // Remove JUCE’s suffix behaviour (important)
        slider.setTextValueSuffix("");

        // Create fixed “ dB” component
        auto* suffix = new FixedSuffixLabel(" dB", fontTypeface);
        suffix->setInterceptsMouseClicks(false, false);
        label->addAndMakeVisible(suffix);

        // Create listener & store it so it stays alive
        const auto listener = std::make_shared<SuffixPositionListener>(suffix);
        label->addComponentListener(listener.get());

        // Save both suffix and listener
        suffixComponents.set(label, suffix);
        suffixListeners.set(label, listener);

        // Force at least one layout now
        listener->componentMovedOrResized(*label, true, true);

        return label;
    }


    /// <summary>
    /// Overrides the function that draws the background.
    /// Is set to draw the custom image set in the constructor and scales it.
    /// Also, it checks if the image is not NULL otherwise it will fill the background with a colour.
    /// </summary>
    /// <params></params>
    void drawLinearSliderBackground(juce::Graphics& g,
        const int x,
        const int y,
        const int height,
        const int width,
        float /*sliderPos*/,
        float /*minSliderPos*/,
        float /*maxSliderPos*/,
        const juce::Slider::SliderStyle /*style*/,
        juce::Slider&) override
    {
        if (!backgroundImage.isNull())
        {
            g.drawImage(backgroundImage,
                x + editorScalarWidth * 118, y, width, height,
                0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
        }
        else
        {
            g.setColour(juce::Colours::violet);
            g.fillRect(x, y, width, height);
        }
    }

    /// <summary>
    /// Overrides the function that draws the thumb.
    /// Is set to draw the custom image set in the constructor and scales it.
    /// In addition to the scaling it sets the thumb to the correct position on the slider.
    /// Also, it checks if the image is not NULL otherwise it will fill the background with a colour.
    /// </summary>
    /// <params></params>
    void drawLinearSliderThumb(juce::Graphics& g,
        const int x,
        const int y,
        const int width,
        const int height,
        const float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        if (!thumbImage.isNull())
        {
            const int thumbHeight = thumbImage.getHeight()*editorScalarHeight;
            const int thumbWidth = thumbImage.getWidth()*editorScalarWidth;
            float drawY = 0.0f;
            const float drawX = x + (width - thumbWidth) / 2.0f;
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

    /// <summary>
    /// Overrides the slider layout. Allows to create a new slider layout in this case
    /// to set the text box further below the slider
    /// </summary>
    juce::Slider::SliderLayout getSliderLayout(juce::Slider&) override
{
    juce::Slider::SliderLayout layout;
        layout.sliderBounds = juce::Rectangle<int>(0 * editorScalarWidth, 50 * editorScalarHeight,
                                                   119 * editorScalarWidth, 544 * editorScalarHeight);
        layout.textBoxBounds = juce::Rectangle<int>(-100 * editorScalarWidth, 640 * editorScalarHeight,
                                                    350 * editorScalarWidth, 100 * editorScalarHeight);

    return layout;
}

    //variables for scaling, set by the editor
    float editorScalarWidth, editorScalarHeight;
    ~SimpleSliderLookAndFeel() override
    {
        for (auto it = suffixComponents.begin(); it != suffixComponents.end(); ++it)
        {
            if (auto* label = it.getKey())
                if (auto* suffix = it.getValue())
                    label->removeChildComponent(suffix);
        }
    }
private:
    juce::File fontFile; //containing the font file
    juce::File pngBackground; //containing the png for the background
    juce::File pngThumb; //containing the png for the thumb
    juce::MemoryBlock fontData; //data block for the font File
    juce::Typeface::Ptr fontTypeface; //typeface to be set by the font file
    //juce::Font LeoFont; //complete custom font
    juce::Label *sliderLabel; //custom label

    //------------------------------------------------------------------------------------------------------------------
    class FixedSuffixLabel final : public juce::Component
    {
    public:
        FixedSuffixLabel(juce::String  s, juce::Typeface::Ptr  tf)
            : suffix(std::move(s)), typeface(std::move(tf))
        {}

        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colour::fromRGB(33, 33, 29));

            juce::Font suffixFont(typeface);
            suffixFont.setHeight(getHeight() * 0.75f);   // scale as you like

            g.setFont(suffixFont);
            g.drawFittedText(suffix, getLocalBounds(), juce::Justification::centredRight, 1);
        }

    private:
        juce::String suffix;
        juce::Typeface::Ptr typeface;
    };

    //====================================================================
    class SuffixPositionListener final: public juce::ComponentListener
    {
    public:
        explicit SuffixPositionListener(juce::Component* suffixComp)
            : suffix(suffixComp)
        {}

        void componentMovedOrResized(juce::Component& c, bool, bool) override
        {
            auto b = c.getLocalBounds();
            suffix->setBounds(b.removeFromRight(350));
        }

    private:
        juce::Component* suffix;
    };
    //====================================================================
    juce::HashMap<juce::Label*, std::shared_ptr<SuffixPositionListener>> suffixListeners;
    juce::HashMap<juce::Label*, juce::Component*> suffixComponents;

};

