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
    int textBoxWidth = 80;
    int textBoxHeight = 15;

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

    // set label text values
    inputGainLabel.setText("Input Gain", NotificationType::dontSendNotification);
    outputGainLabel.setText("Output Gain", NotificationType::dontSendNotification);

    // attach labels to components
    inputGainLabel.attachToComponent(&inputGainSlider, false);
    outputGainLabel.attachToComponent(&outputGainSlider, false);

    // set label justification types
    inputGainLabel.setJustificationType(Justification::centredTop);
    outputGainLabel.setJustificationType(Justification::centredTop);

    // set slider values 
    inputGainSlider.setValue(0.0f);
    outputGainSlider.setValue(0.0f);

    // add slider listeners
    inputGainSlider.addListener(this);
    outputGainSlider.addListener(this);

    // add all sliders and make visible 
    addAndMakeVisible(&inputGainSlider);
    addAndMakeVisible(&outputGainSlider);

    // add all labels
    addAndMakeVisible(&inputGainLabel);
    addAndMakeVisible(&outputGainLabel);
    
    // handles slider value changes
    inputGainSlider.onValueChange = [this]
    {
        auto currentSliderValue = pow(2, inputGainSlider.getValue() / 6);
        processor.inputGain->setValueNotifyingHost(currentSliderValue);
    };
    outputGainSlider.onValueChange = [this]
    {
        auto currentSliderValue = pow(2, outputGainSlider.getValue() / 6);
        processor.outputGain->setValueNotifyingHost(currentSliderValue);
    };
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
    inputGainSlider.setBounds(getWidth() / 2 - (size / 2) - (size * 0.6f), getHeight() / 2 - (size / 2), size, size);
    outputGainSlider.setBounds(getWidth() / 2 - (size / 2) + (size * 0.6f), getHeight() / 2 - (size / 2), size, size);
}

void RingModulatorAudioProcessorEditor::sliderValueChanged(Slider* slider) 
{
    auto currentSliderValue = slider->getValue();
    // do something ...
}