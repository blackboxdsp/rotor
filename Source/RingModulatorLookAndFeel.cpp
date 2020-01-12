/*
  ==============================================================================

    RingModulatorLookAndFeel.cpp
    Created: 11 Jan 2020 6:39:54pm
    Author:  Matt

  ==============================================================================
*/

#include "RingModulatorLookAndFeel.h"

RingModulatorLookAndFeel::RingModulatorLookAndFeel()
{
	setColour(Slider::rotarySliderFillColourId, Colour::fromRGBA(226, 115, 0, 255));
}

void RingModulatorLookAndFeel::drawRotarySlider(Graphics& g,
	int x, int y,
	int width, int height,
	float sliderPos,
	const float rotaryStartAngle,
	const float rotaryEndAngle,
	Slider& slider)
{
	// get all necessary variables
	const float radius = jmin(width / 2, height / 2) - 2.0f;
	const float centerX = x + width * 0.5f;
	const float centerY = y + height * 0.5f;
	const float rx = centerX - radius;
	const float ry = centerY - radius;
	const float rw = radius * 2.0f;
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	// draw value in middle of slider

	// draw slider track
	Path arcPath;
	arcPath.addArc(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, true);
	g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
	g.strokePath(arcPath, PathStrokeType(3.0f));

	// set color variables for gradient
	Colour sliderFillStart = Colour::fromRGBA(116, 42, 208, 255);
	Colour sliderFillStop = Colour::fromRGBA(144, 78, 228, 255);
	ColourGradient sliderFill = ColourGradient(sliderFillStart, (float)x, 0.0f, sliderFillStop, (float)width, 0.0f, false);
	g.setGradientFill(sliderFill);

	// draw the filled path
	Path arcPathFilled;
	arcPathFilled.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
	PathStrokeType(3.0f).createStrokedPath(arcPathFilled, arcPathFilled);
	g.fillPath(arcPathFilled);
}