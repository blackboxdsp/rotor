/*
  ==============================================================================

    RingModulatorLookAndFeel.h
    Created: 11 Jan 2020 6:40:05pm
    Author:  Matt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================

class RingModulatorLookAndFeel : public LookAndFeel_V4
{
    //==========================================================================

public:
    RingModulatorLookAndFeel();
    ~RingModulatorLookAndFeel();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        Slider& s) override;

    void drawToggleButton(Graphics& g, ToggleButton& b, 
        bool shouldDrawButtonAsHighlighted, 
        bool shouldDrawButtonAsDown) override;

    //==========================================================================

private:
    const Colour inactiveFill = Colour::fromRGBA(31, 31, 31, 0);
    const Colour activeFillStart = Colour::fromRGBA(116, 42, 208, 255);
    const Colour activeFillStop = Colour::fromRGBA(144, 78, 228, 255);
};
