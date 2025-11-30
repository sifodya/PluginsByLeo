//
// Created by cedri on 15/10/2025.
//
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"


class SimpleHyperlinkButton final : public juce::HyperlinkButton
{
public:
    SimpleHyperlinkButton()
    {

        customTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::NeueHaasDisplayLight_ttf, BinaryData::NeueHaasDisplayLight_ttfSize);
        const auto buttonFont = juce::Font(juce::FontOptions{}.withTypeface(customTypeface));
        setButtonText("check other plugins");
        setURL(juce::URL("https://leo-brennauer.com/#minishop"));
        setFont(buttonFont, true, juce::Justification::centred);
        setColour(textColourId, juce::Colours::black);
    }
private:
    juce::MemoryBlock fontData;
    juce::File fontFile;
    juce::LookAndFeel_V4 SimpleHyperlinkButtonLAF;
    juce::Typeface::Ptr customTypeface;
    };

