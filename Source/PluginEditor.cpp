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
    setSize (480, 960);

    // SLIDERS =================================================================

    // set slider styles
    gainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationFrequencySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationWaveformSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    // set textbox styles
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    modulationFrequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    modulationWaveformSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);

    // set text value suffixes
    gainSlider.setTextValueSuffix(" dB");
    modulationFrequencySlider.setTextValueSuffix(" Hz");
    dryWetSlider.setTextValueSuffix(" %");

    // set double click return values
    gainSlider.setDoubleClickReturnValue(true, 0.0f);
    modulationFrequencySlider.setDoubleClickReturnValue(true, 500.0f);
    modulationWaveformSlider.setDoubleClickReturnValue(true, 1);
    dryWetSlider.setDoubleClickReturnValue(true, 50.0f);

    // attach valueTreeState attachments
    gainAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    modulationFrequencyAttachment.reset(new SliderAttachment(valueTreeState, "modulationFrequency", modulationFrequencySlider));
    modulationWaveformAttachment.reset(new SliderAttachment(valueTreeState, "modulationWaveform", modulationWaveformSlider));
    dryWetAttachment.reset(new SliderAttachment(valueTreeState, "dryWet", dryWetSlider));

    // add all sliders and make visible 
    addAndMakeVisible(&gainSlider);
    addAndMakeVisible(&modulationFrequencySlider);
    addAndMakeVisible(&modulationWaveformSlider);
    addAndMakeVisible(&dryWetSlider);

    // LABELS ==================================================================

    // set label text values
    gainLabel.setText("Output Gain", NotificationType::dontSendNotification);
    modulationFrequencyLabel.setText("Mod Frequency", NotificationType::dontSendNotification);
    modulationWaveformLabel.setText("Mod Waveform", NotificationType::dontSendNotification);
    dryWetLabel.setText("Dry / Wet", NotificationType::dontSendNotification);

    // attach labels to components
    gainLabel.attachToComponent(&gainSlider, false);
    modulationFrequencyLabel.attachToComponent(&modulationFrequencySlider, false);
    modulationWaveformLabel.attachToComponent(&modulationWaveformSlider, false);
    dryWetLabel.attachToComponent(&dryWetSlider, false);

    // set label justification types
    gainLabel.setJustificationType(Justification::centredTop);
    modulationFrequencyLabel.setJustificationType(Justification::centredTop);
    modulationWaveformLabel.setJustificationType(Justification::centredBottom);
    dryWetLabel.setJustificationType(Justification::centredTop);

    // add all labels
    addAndMakeVisible(&gainLabel);
    addAndMakeVisible(&modulationFrequencyLabel);
    addAndMakeVisible(&modulationWaveformLabel);
    addAndMakeVisible(&dryWetLabel);
    
    // ONVALUECHANGE DEFINITIONS ========================================

    // update the phase delta whenever frequency slider is updated
    modulationFrequencySlider.onValueChange = [this]
    {
        auto currentSampleRate = processor.getSampleRate();
        if (currentSampleRate > 0.0)
        {
            auto frequency = modulationFrequencySlider.getValue();
            processor.updatePhaseDelta(frequency, currentSampleRate, processor.getWavetableSize());
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
    modulationWaveformSlider.setBounds(area.removeFromLeft(quarterWidth));
    gainSlider.setBounds(area.removeFromLeft(quarterWidth));
    dryWetSlider.setBounds(area.removeFromLeft(quarterWidth));
}