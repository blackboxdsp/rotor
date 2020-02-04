/*
  ==============================================================================

    RotorLookAndFeel.cpp
    Created: 11 Jan 2020 6:39:54pm
    Author:  Matthew Maxwell

  ==============================================================================
*/

#include "./RotorLookAndFeel.h"

//==============================================================================
RotorLookAndFeel::RotorLookAndFeel()
{
}

RotorLookAndFeel::~RotorLookAndFeel()
{
}

//==============================================================================
void RotorLookAndFeel::drawRotarySlider(
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
	//if (sliderPos == 0.0f && slider.getName() != "inversion") sliderPos = 0.01f;
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	// get readout value
	const double value = slider.getValue();
	String readoutValue = (value >= 1000.0 ? String(value / 1000.0, 1) + "k" : String(value, 1));
	String readout = readoutValue + slider.getTextValueSuffix();

	// draw shape if waveform slider, text otherwise --- SHOULD BE SWITCH STATEMENT ?
	if (slider.getName() == "shape")
	{
		// init path variables to draw wave shape
		Path waveformPath;
		float waveformPathHeight = radius / 2.0f;
		float xPos = centerX - radius / 2;
		float xEndPos = centerX + radius / 2;
		float yPos = centerY + waveformPathHeight / 2.0f;

		// draw particular wave shape according to slider
		switch ((int) slider.getValue())
		{
			// SINE
			default:
			case 0:
				yPos = centerY;
				waveformPathHeight /= 2.0f;
				waveformPath.startNewSubPath(xPos, yPos);
				for (auto x = xPos; x <= xEndPos; x++)
				{
					yPos = centerY + waveformPathHeight * -sinf(MathConstants<float>::twoPi * (x - xPos) / (xEndPos - xPos));
					waveformPath.lineTo(x, yPos);
				}
				break;

			// TRIANGLE
			case 1:
				waveformPath.startNewSubPath(xPos, yPos);
				waveformPath.lineTo(centerX, centerY - waveformPathHeight / 2.0f);
				waveformPath.lineTo(xEndPos, centerY + waveformPathHeight / 2.0f);
				break;

			// SAWTOOTH
			case 2:
				waveformPath.startNewSubPath(xPos, yPos);
				waveformPath.lineTo(xEndPos, centerY - waveformPathHeight / 2.0f);
				waveformPath.lineTo(xEndPos, yPos);
				break;

			// SQUARE
			case 3:
				yPos = centerY - waveformPathHeight / 2.0f;
				waveformPath.startNewSubPath(xPos, yPos);
				waveformPath.lineTo(centerX, yPos);
				yPos = centerY + waveformPathHeight / 2.0f;
				waveformPath.lineTo(centerX, yPos);
				waveformPath.lineTo(xEndPos, yPos);
				break;

			// SEMI-SINE
			case 4:
				waveformPathHeight /= 2.0f;
				waveformPath.startNewSubPath(xPos, yPos);
				for (auto x = xPos; x <= xEndPos; x++)
				{
					auto newY = yPos + waveformPathHeight * -2.0f * sinf(MathConstants<float>::pi * (x - xPos) / (xEndPos - xPos));
					waveformPath.lineTo(x, newY);
				}
				break;
		}

		// set color and stroke path
		g.setColour(activeFill);
		g.strokePath(waveformPath, PathStrokeType(3.0f));
	}
	else
	{
		// get needed variables for displaying slider value
		String readout;
		const double value = slider.getValue();

		// set string properties according to slider
		auto name = slider.getName();
		if(name == "rate")
		{
			String readoutValue = (value >= 1000.0 ? String(value / 1000.0, 1) + "k" : String(value, 1));
			readout = readoutValue + slider.getTextValueSuffix();
		}
		else if (name == "mix" || name == "level")
		{
			readout = String(value, 1) + slider.getTextValueSuffix();
		}
		else
		{
			readout = String(value, 2) + slider.getTextValueSuffix();
		}

		// handle slider coloring (esp invert button / slider)
		if (name == "inversion")
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
		g.drawText(
			readout,
			centerX - radius,						// int x (radius for suffix offset)
			centerY - (fontSize * 5.0f / 12.0f),	// int y (ratio based off font size)
			rw,										// int width
			fontSize,								// int height
			Justification::centred);
	}

	// set color variables for gradient
	ColourGradient sliderFill = ColourGradient(activeFillStart, (float) x, 0.0f, activeFillStop, (float) width, 0.0f, false);
	g.setGradientFill(sliderFill);

	// draw the filled path
	Path arcPathFilled;
	arcPathFilled.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
	PathStrokeType(3.0f).createStrokedPath(arcPathFilled, arcPathFilled);
	g.fillPath(arcPathFilled);
}

void RotorLookAndFeel::drawToggleButton(
	Graphics& g,
	ToggleButton& b,
	bool shouldDrawButtonAsHighlighted, 
	bool shouldDrawButtonAsDown = 0)
{
}

//==============================================================================
void RotorLookAndFeel::setFontSize(int newSize)
{
	fontSize = (float) newSize;
}