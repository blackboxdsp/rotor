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
    // GUI variables
    int textBoxWidth = 100;
    int textBoxHeight = 20;

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (480, 360);

    // set slider styles
    inputGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    
    // set slider ranges
    inputGainSlider.setRange(-30.0f, 0.0f, 0.5f);
    outputGainSlider.setRange(-30.0f, 0.0f, 0.5f);

    // set textbox styles
    inputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    outputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    
    // set popup display configs
    inputGainSlider.setPopupDisplayEnabled(true, false, this);
    outputGainSlider.setPopupDisplayEnabled(true, false, this);

    // set text value suffixes
    inputGainSlider.setTextValueSuffix(" dB");
    outputGainSlider.setTextValueSuffix(" dB");

    // set slider values 
    inputGainSlider.setValue(0.0f);
    outputGainSlider.setValue(0.0f);

    // add slider listeners
    inputGainSlider.addListener(this);
    outputGainSlider.addListener(this);

    // add all sliders and make visible 
    addAndMakeVisible(&inputGainSlider);
    addAndMakeVisible(&outputGainSlider);
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
    int size = getWidth() * 0.4f;
    inputGainSlider.setBounds(getWidth() / 2 - (size / 2) - (size * 0.8f), getHeight() / 2 - (size / 2), size, size);
    outputGainSlider.setBounds(getWidth() / 2 - (size / 2) + (size * 0.8f), getHeight() / 2 - (size / 2), size, size);
}

void RingModulatorAudioProcessorEditor::sliderValueChanged(Slider* slider) 
{
    auto currentSliderValue = slider->getValue();
    if (slider == &inputGainSlider)
    {
        currentSliderValue = pow(2, currentSliderValue / 6);
        processor.inputGain->setValueNotifyingHost(currentSliderValue);
    }
    else if (slider == &outputGainSlider)
    {
        currentSliderValue = pow(2, currentSliderValue / 6);
        processor.outputGain->setValueNotifyingHost(currentSliderValue);
    }
    else
    {
        // do something else...
    }
}