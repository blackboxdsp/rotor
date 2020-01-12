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
}

RingModulatorLookAndFeel::~RingModulatorLookAndFeel()
{
}

// draw arc path representing slider
void RingModulatorLookAndFeel::drawRotarySlider(
	Graphics& g,
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

	// even if slider is fully zero, still display something
	if (sliderPos == 0.0f && slider.getName() != "inversion") sliderPos = 0.01f;
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	// get readout value
	const double value = slider.getValue();
	String readoutValue = (value >= 1000.0 ? String(value / 1000.0, 1) + "k" : String(value, 1));
	String readout = readoutValue + slider.getTextValueSuffix();

	// draw shape if waveform slider, text otherwise
	if (slider.getName() == "shape")
	{

	}
	else
	{
		// get slider data and format
		const double value = slider.getValue();
		String readoutValue = (value >= 1000.0 ? String(value / 1000.0, 1) + "k" : String(value, 1));
		String readout = readoutValue + slider.getTextValueSuffix();

		// handle text colouring 
		if (slider.getName() == "inversion")
		{
			readout = "ON";
			g.setColour(slider.getValue() > 0.5f ? activeFill : inactiveFill);
		}
		else
		{
			g.setColour(activeFill);
		}
		
		// draw text accordingly
		g.setFont(getSliderReadoutFont(fontSize));
		g.drawText(readout,
			centerX - radius,                     // int x (radius for suffix offset)
			centerY - (fontSize * 5.0f / 12.0f),  // int y (ratio based off font size)
			rw,		                             // int width
			fontSize,		                     // int height
			Justification::centred);
	}

	// set color variables for gradient
	ColourGradient sliderFill = ColourGradient(activeFillStart, (float)x, 0.0f, activeFillStop, (float)width, 0.0f, false);
	g.setGradientFill(sliderFill);

	// draw the filled path
	Path arcPathFilled;
	arcPathFilled.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
	PathStrokeType(3.0f).createStrokedPath(arcPathFilled, arcPathFilled);
	g.fillPath(arcPathFilled);
}

// draw nothing for toggle button
void RingModulatorLookAndFeel::drawToggleButton(
	Graphics& g,
	ToggleButton& b,
	bool shouldDrawButtonAsHighlighted, 
	bool shouldDrawButtonAsDown = 0)
{
}

// sets font to given new size
void RingModulatorLookAndFeel::setFontSize(int newSize)
{
	fontSize = (float) newSize;
}