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
public:

    RingModulatorLookAndFeel();
    ~RingModulatorLookAndFeel();

    //==========================================================================

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        Slider& s) override;

    void drawToggleButton(Graphics& g, ToggleButton& b, 
        bool shouldDrawButtonAsHighlighted, 
        bool shouldDrawButtonAsDown) override;

    void setFontSize(int newSize);

private:
    //==========================================================================

    // COLOURS
    const Colour inactiveFill = Colour::fromRGBA(255, 255, 255, 63);
    const Colour activeFill = Colour::fromRGBA(167, 98, 255, 255);
    const Colour activeFillStart = Colour::fromRGBA(116, 42, 208, 255);
    const Colour activeFillStop = Colour::fromRGBA(144, 78, 228, 255);

    // FONTS
    float fontSize;
    static const Font& getSliderReadoutFont(float fontSize)
    {
        static Font sliderReadout(Font(Typeface::createSystemTypefaceFor(BinaryData::montserrat_ttf, BinaryData::montserrat_ttfSize)));
        sliderReadout.setHeight(fontSize);
        return sliderReadout;
    }
};
