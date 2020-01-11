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

    // set aspect ratio and dimensions accordingly
    double ratio = 0.5;
    setResizeLimits(360, 360 / ratio, 540, 540 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize (480, 480 / ratio);

    // SLIDERS =================================================================

    // set slider styles
    modulationRate.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationWaveformSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationPulseWidthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    mixSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    levelSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    // set textbox styles
    modulationRate.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    modulationWaveformSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    modulationPulseWidthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    
    mixSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    levelSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);

    // set text value suffixes
    modulationRate.setTextValueSuffix(" Hz");

    mixSlider.setTextValueSuffix(" %");
    levelSlider.setTextValueSuffix(" dB");

    // set double click return values
    modulationRate.setDoubleClickReturnValue(true, 500.0f);
    modulationWaveformSlider.setDoubleClickReturnValue(true, 1);
    modulationPulseWidthSlider.setDoubleClickReturnValue(true, 0.5f);

    mixSlider.setDoubleClickReturnValue(true, 50.0f);
    levelSlider.setDoubleClickReturnValue(true, 0.0f);

    // attach valueTreeState attachments
    modulationRateAttachment.reset(new SliderAttachment(valueTreeState, "rate", modulationRate));
    modulationWaveformAttachment.reset(new SliderAttachment(valueTreeState, "waveform", modulationWaveformSlider));
    modulationPulseWidthAttachment.reset(new SliderAttachment(valueTreeState, "pulseWidth", modulationPulseWidthSlider));

    mixAttachment.reset(new SliderAttachment(valueTreeState, "mix", mixSlider));
    levelAttachment.reset(new SliderAttachment(valueTreeState, "level", levelSlider));

    // add all sliders and make visible 
    addAndMakeVisible(&modulationRate);
    addAndMakeVisible(&modulationWaveformSlider);
    addAndMakeVisible(&modulationPulseWidthSlider);

    addAndMakeVisible(&mixSlider);
    addAndMakeVisible(&levelSlider);

    // BUTTONS =================================================================

    // juce boilerplate
    modulationInversionAttachment.reset(new ButtonAttachment(valueTreeState, "inversion", modulationInversionButton));
    addAndMakeVisible(&modulationInversionButton);
    processor.changeModulationInversionFactor(modulationInversionButton.getToggleState());

    // define onclick for inversion button
    modulationInversionButton.onClick = [this]
    {
        processor.changeModulationInversionFactor(modulationInversionButton.getToggleState());
    };

    // LABELS ==================================================================

    // set label text values
    modulationRateLabel.setText("rate", NotificationType::dontSendNotification);
    modulationWaveformLabel.setText("shape", NotificationType::dontSendNotification);
    modulationInversionLabel.setText("invert", NotificationType::dontSendNotification);
    modulationPulseWidthLabel.setText("pulse width", NotificationType::dontSendNotification);

    mixLabel.setText("mix", NotificationType::dontSendNotification);
    levelLabel.setText("level", NotificationType::dontSendNotification);

    // attach labels to components
    modulationRateLabel.attachToComponent(&modulationRate, false);
    modulationWaveformLabel.attachToComponent(&modulationWaveformSlider, false);
    modulationInversionLabel.attachToComponent(&modulationInversionButton, false);
    modulationPulseWidthLabel.attachToComponent(&modulationPulseWidthSlider, false);

    mixLabel.attachToComponent(&mixSlider, false);
    levelLabel.attachToComponent(&levelSlider, false);

    // set label justification types
    modulationRateLabel.setJustificationType(Justification::centredTop);
    modulationWaveformLabel.setJustificationType(Justification::centredBottom);
    modulationInversionLabel.setJustificationType(Justification::centredTop);
    modulationPulseWidthLabel.setJustificationType(Justification::centredBottom);

    mixLabel.setJustificationType(Justification::centredTop);
    levelLabel.setJustificationType(Justification::centredTop);

    // add all labels
    addAndMakeVisible(&modulationRateLabel);
    addAndMakeVisible(&modulationWaveformLabel);
    addAndMakeVisible(&modulationInversionLabel);
    addAndMakeVisible(&modulationPulseWidthLabel);

    addAndMakeVisible(&mixLabel);
    addAndMakeVisible(&levelLabel);
    
    // ONVALUECHANGE DEFINITIONS ========================================

    // update the phase delta whenever frequency slider is updated
    modulationRate.onValueChange = [this]
    {
        auto currentSampleRate = processor.getSampleRate();
        if (currentSampleRate > 0.0)
        {
            auto frequency = modulationRate.getValue();
            processor.updatePhaseDelta(frequency, currentSampleRate);
        }
    };

    // write a new wavetable whenever new item is selected
    modulationWaveformSlider.onValueChange = [this]
    {
        processor.writeWavetable((int) modulationWaveformSlider.getValue());
    };

    // write new wavetable when square is selected and pulse width slider is changed
    modulationPulseWidthSlider.onValueChange = [this]
    {
        if ((int) modulationWaveformSlider.getValue() == 3)
        {
            processor.writeWavetable(3);
        }
    };
}

RingModulatorAudioProcessorEditor::~RingModulatorAudioProcessorEditor()
{
}

//==============================================================================
void RingModulatorAudioProcessorEditor::paint (Graphics& g)
{
    // get background image and apply as background
    g.drawImage(backgroundImage, getLocalBounds().toFloat());
}

void RingModulatorAudioProcessorEditor::resized()
{
    // get canvas
    auto area = getLocalBounds();

    // handle header / footer area
    auto margin = area.getHeight() * 0.25;
    area.removeFromTop(margin / 3.0);
    area.removeFromBottom(margin / 3.0);

    // dimension and layout variables
    int dialWidth = area.getWidth() / 5.6;
    int quarterWidth = area.getWidth() / 4;

    // position sliders
    auto tempArea = area.removeFromLeft(quarterWidth);
    modulationRate.setBounds(tempArea.removeFromTop(tempArea.getHeight() / 2));
    tempArea.removeFromTop(margin / 3.0);
    modulationPulseWidthSlider.setBounds(tempArea);

    tempArea = area.removeFromLeft(quarterWidth);
    modulationWaveformSlider.setBounds(tempArea.removeFromBottom(area.getHeight() / 2));
    tempArea.removeFromTop(margin / 3.0);
    modulationInversionButton.setBounds(tempArea.removeFromBottom(area.getHeight() / 2));
    
    levelSlider.setBounds(area.removeFromLeft(quarterWidth));
    mixSlider.setBounds(area.removeFromLeft(quarterWidth));
}