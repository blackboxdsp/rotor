/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/**
*/
class RingModulatorAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    RingModulatorAudioProcessorEditor (RingModulatorAudioProcessor&, AudioProcessorValueTreeState&);
    ~RingModulatorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RingModulatorAudioProcessor& processor;

    // Reference to value tree state for quick access
    AudioProcessorValueTreeState& valueTreeState;

    // slider declarations for parameters
    Slider levelSlider;
    Slider mixSlider;
    Slider modulationFrequencySlider;
    Slider modulationWaveformSlider;

    // buttons for modulator inversion
    ToggleButton modulationInversionButton;

    // declare labels
    Label levelLabel;
    Label mixLabel;
    Label modulationFrequencyLabel;
    Label modulationWaveformLabel;
    Label modulationInversionLabel;

    // attachment declarations
    std::unique_ptr<SliderAttachment> levelAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> modulationFrequencyAttachment;
    std::unique_ptr<SliderAttachment> modulationWaveformAttachment;
    std::unique_ptr<ButtonAttachment> modulationInversionAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
