/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModulatorAudioProcessorEditor::RingModulatorAudioProcessorEditor (RingModulatorAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // GUI variables
    int textBoxWidth = 80;
    int textBoxHeight = 15;

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (480, 360);

    // SLIDERS =================================================================

    // set slider styles
    levelSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationFrequencySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationWaveformSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    // set textbox styles
    levelSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    modulationFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    modulationWaveformSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    mixSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);

    // set text value suffixes
    levelSlider.setTextValueSuffix(" dB");
    modulationFrequencySlider.setTextValueSuffix(" Hz");
    mixSlider.setTextValueSuffix(" %");

    // set double click return values
    levelSlider.setDoubleClickReturnValue(true, 0.0f);
    modulationFrequencySlider.setDoubleClickReturnValue(true, 500.0f);
    modulationWaveformSlider.setDoubleClickReturnValue(true, 1);
    mixSlider.setDoubleClickReturnValue(true, 50.0f);

    // attach valueTreeState attachments
    levelAttachment.reset(new SliderAttachment(valueTreeState, "level", levelSlider));
    modulationFrequencyAttachment.reset(new SliderAttachment(valueTreeState, "modulationFrequency", modulationFrequencySlider));
    modulationWaveformAttachment.reset(new SliderAttachment(valueTreeState, "modulationWaveform", modulationWaveformSlider));
    mixAttachment.reset(new SliderAttachment(valueTreeState, "dryWet", mixSlider));

    // add all sliders and make visible 
    addAndMakeVisible(&levelSlider);
    addAndMakeVisible(&modulationFrequencySlider);
    addAndMakeVisible(&modulationWaveformSlider);
    addAndMakeVisible(&mixSlider);

    // BUTTONS =================================================================

    // juce boilerplate
    modulationInversionAttachment.reset(new ButtonAttachment(valueTreeState, "modulationInversion", modulationInversionButton));
    addAndMakeVisible(&modulationInversionButton);
    processor.changeModulationInversion(modulationInversionButton.getToggleState());

    // define onclick for inversion button
    modulationInversionButton.onClick = [this]
    {
        processor.changeModulationInversion(modulationInversionButton.getToggleState());
    };

    // LABELS ==================================================================

    // set label text values
    levelLabel.setText("Output Gain", NotificationType::dontSendNotification);
    modulationFrequencyLabel.setText("Mod Frequency", NotificationType::dontSendNotification);
    modulationWaveformLabel.setText("Mod Waveform", NotificationType::dontSendNotification);
    modulationInversionLabel.setText("Mod Inversion", NotificationType::dontSendNotification);
    mixLabel.setText("Dry / Wet", NotificationType::dontSendNotification);

    // attach labels to components
    levelLabel.attachToComponent(&levelSlider, false);
    modulationFrequencyLabel.attachToComponent(&modulationFrequencySlider, false);
    modulationWaveformLabel.attachToComponent(&modulationWaveformSlider, false);
    modulationInversionLabel.attachToComponent(&modulationInversionButton, false);
    mixLabel.attachToComponent(&mixSlider, false);

    // set label justification types
    levelLabel.setJustificationType(Justification::centredTop);
    modulationFrequencyLabel.setJustificationType(Justification::centredTop);
    modulationWaveformLabel.setJustificationType(Justification::centredBottom);
    modulationInversionLabel.setJustificationType(Justification::centredTop);
    mixLabel.setJustificationType(Justification::centredTop);

    // add all labels
    addAndMakeVisible(&levelLabel);
    addAndMakeVisible(&modulationFrequencyLabel);
    addAndMakeVisible(&modulationWaveformLabel);
    addAndMakeVisible(&modulationInversionLabel);
    addAndMakeVisible(&mixLabel);
    
    // ONVALUECHANGE DEFINITIONS ========================================

    // update the phase delta whenever frequency slider is updated
    modulationFrequencySlider.onValueChange = [this]
    {
        auto currentSampleRate = processor.getSampleRate();
        if (currentSampleRate > 0.0)
        {
            auto frequency = modulationFrequencySlider.getValue();
            processor.updatePhaseDelta(frequency, currentSampleRate);
        }
    };

    // write a new wavetable whenever new item is selected
    modulationWaveformSlider.onValueChange = [this]
    {
        processor.writeWavetable((int) modulationWaveformSlider.getValue());
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
    // get canvas
    auto area = getLocalBounds();

    // handle header / footer area
    auto headerHeight = area.getHeight() * 0.1;
    area.removeFromTop(headerHeight * 2.5);
    area.removeFromBottom(headerHeight * 2.5);

    // dimension and layout variables
    int dialWidth = area.getWidth() / 5;
    int quarterWidth = area.getWidth() / 4;

    // position sliders
    modulationFrequencySlider.setBounds(area.removeFromLeft(quarterWidth));

    auto tempArea = area.removeFromLeft(quarterWidth);
    modulationWaveformSlider.setBounds(tempArea.removeFromBottom(area.getHeight() / 2));
    modulationInversionButton.setBounds(tempArea.removeFromBottom(area.getHeight() / 2));
    levelSlider.setBounds(area.removeFromLeft(quarterWidth));
    mixSlider.setBounds(area.removeFromLeft(quarterWidth));
}