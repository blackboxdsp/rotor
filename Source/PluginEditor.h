/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RingModulatorAudioProcessorEditor  : public AudioProcessorEditor,
                                           private Slider::Listener
{
public:
    RingModulatorAudioProcessorEditor (RingModulatorAudioProcessor&);
    ~RingModulatorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RingModulatorAudioProcessor& processor;

    void sliderValueChanged(Slider* slider) override;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
