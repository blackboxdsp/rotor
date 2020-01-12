/*
  ==============================================================================

    RingModulatorLookAndFeel.h
    Created: 11 Jan 2020 6:40:05pm
    Author:  Matt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class RingModulatorLookAndFeel : public LookAndFeel_V4
{
public:
    RingModulatorLookAndFeel();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        Slider& s) override;
};
