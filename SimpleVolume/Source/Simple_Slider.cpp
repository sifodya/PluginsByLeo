//
// Created by cedri on 30/09/2025.
//

#include "Simple_Slider.h"

Simple_Slider::Simple_Slider()
{
    //setSliderStyle(simpleSliderStyle);
   // setTextBoxStyle(TextBoxBelow, false, 60, 20);

   setLookAndFeel(&simpleSliderLNF);
}

Simple_Slider::~Simple_Slider()
{
 setLookAndFeel(nullptr);
}

void Simple_Slider::paint(juce::Graphics& g)
{


    //simpleSliderLabel.setJustificationType(simpleSliderLabelJustification);

}

void Simple_Slider::resized()
{
    //simpleSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 100, 100, 200);
}