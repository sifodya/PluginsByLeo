//
// Created by cedri on 15/10/2025.
//
#include <iostream>
#include <juce_gui_basics/juce_gui_basics.h>


class SimpleHyperlinkButton : public juce::HyperlinkButton
{
public:
    SimpleHyperlinkButton()
    {
        fontFile = juce::File::getSpecialLocation(juce::File::userMusicDirectory).getChildFile("PluginsByLeo/neue-haas-grotesk-display-pro/NeueHaasDisplayLight.ttf");
        if (fontFile.existsAsFile())
            fontFile.loadFileAsData(fontData);

        buttonFont = juce::Typeface::createSystemTypefaceFor(fontData.getData(), fontData.getSize());
        setButtonText("check other plugins");
        setURL(juce::URL("https://leo-brennauer.com"));
        setFont(buttonFont, true, juce::Justification::centred);
        setColour(juce::HyperlinkButton::textColourId, juce::Colours::black);
    }
private:
    juce::Font buttonFont;
    juce::MemoryBlock fontData;
    juce::File fontFile;
    juce::LookAndFeel_V4 SimpleHyperlinkButtonLAF;
    };

