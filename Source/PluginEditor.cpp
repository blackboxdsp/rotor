/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModulatorAudioProcessorEditor::RingModulatorAudioProcessorEditor (RingModulatorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    // define parameters for input gain slider
    inputGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    inputGainSlider.setRange(0.0f, 1.0f, 0.001f);
    inputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 90, 10);
    inputGainSlider.setPopupDisplayEnabled(true, false, this);
    inputGainSlider.setTextValueSuffix("Input Gain");
    inputGainSlider.setValue(1.0f);

    // add slider listeners
    inputGainSlider.addListener(this);

    // add all sliders and make visible 
    addAndMakeVisible(&inputGainSlider);
}

RingModulatorAudioProcessorEditor::~RingModulatorAudioProcessorEditor()
{
}

//==============================================================================
void RingModulatorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void RingModulatorAudioProcessorEditor::resized()
{
    int size = 60;
    inputGainSlider.setBounds(getWidth() / 2 - (size / 2), getHeight() / 2 - (size / 2), size, size);
}

void RingModulatorAudioProcessorEditor::sliderValueChanged(Slider* slider) 
{
    processor.inputGain->setValueNotifyingHost(inputGainSlider.getValue());
}