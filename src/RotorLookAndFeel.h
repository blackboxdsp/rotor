/*
  ==============================================================================

    RotorLookAndFeel.h
    Created: 11 Jan 2020 6:40:05pm
    Author:  Matthew Maxwell

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "RotorData.h"

//==============================================================================
class RotorLookAndFeel : public LookAndFeel_V4
{
public:

    RotorLookAndFeel();
    ~RotorLookAndFeel();

    //==========================================================================

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        Slider& s) override;

    void drawToggleButton(Graphics& g, ToggleButton& b, 
        bool shouldDrawButtonAsHighlighted, 
        bool shouldDrawButtonAsDown) override;

    void setFontSize(int newSize);
    void setStrokeWidth(int currentWindowWidth);

private:
    //==========================================================================

    // COLOURS
    const Colour inactiveFill = Colour::fromRGBA(255, 255, 255, 63);
    const Colour activeFill = Colour::fromRGBA(167, 98, 255, 255);
    const Colour activeFillStart = Colour::fromRGBA(116, 42, 208, 255);
    const Colour activeFillStop = Colour::fromRGBA(144, 78, 228, 255);

    // SLIDERS
    float strokeWidth;

    // FONTS
    float fontSize;
    static const Font& getSliderReadoutFont(float fontSize)
    {
        static Font sliderReadout(Font(Typeface::createSystemTypefaceFor(RotorData::montserrat_ttf, RotorData::montserrat_ttfSize)));
        sliderReadout.setHeight(fontSize);
        return sliderReadout;
    }
};
