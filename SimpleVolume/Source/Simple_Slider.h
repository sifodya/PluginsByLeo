//
// Created by cedri on 30/09/2025.
//

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "SimpleSliderLookAndFeel.cpp"


class Simple_Slider : public juce::Slider
{
public:
    Simple_Slider();
    ~Simple_Slider() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    SimpleSliderLookAndFeel simpleSliderLNF;
private:

    Slider simpleSlider;
    SliderStyle simpleSliderStyle = LinearVertical;

    juce::Label simpleSliderLabel;
    //juce::Justification simpleSliderLabelJustification = juce::Justification::centred;
};


