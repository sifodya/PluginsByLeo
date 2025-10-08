//
// Created by cedri on 30/09/2025.
//

#include "Simple_Slider.h"

Simple_Slider::Simple_Slider()
{
    simpleSlider.setSliderStyle(simpleSliderStyle);
    simpleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    simpleSlider.setRange(0.0, 1.0, 0.01);
    simpleSlider.setValue(0.5);

    simpleSlider.setLookAndFeel(&simpleSliderLNF);
}

Simple_Slider::~Simple_Slider()
{
 simpleSlider.setLookAndFeel(nullptr);
}

void Simple_Slider::paint(juce::Graphics& g)
{


    //simpleSliderLabel.setJustificationType(simpleSliderLabelJustification);

}

void Simple_Slider::resized()
{
    //simpleSliderLabel.set
}