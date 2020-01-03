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
    Slider inputGainSlider;
    Slider outputGainSlider;
    Slider modulationFrequencySlider;
    Slider dryWetSlider;

    // declare labels
    Label inputGainLabel;
    Label outputGainLabel;
    Label modulationFrequencyLabel;
    Label dryWetLabel;

    // attachment declarations
    std::unique_ptr<SliderAttachment> inputGainAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;
    std::unique_ptr<SliderAttachment> dryWetAttachment;
    std::unique_ptr<SliderAttachment> modulationFrequencyAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
