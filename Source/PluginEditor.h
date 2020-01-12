/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RingModulatorLookAndFeel.h"
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

    //==========================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RingModulatorAudioProcessor& processor;

    // Reference to value tree state for quick access
    AudioProcessorValueTreeState& valueTreeState;

    // for better rendering
    OpenGLContext glContext;
    
    // GUI COMPONENTS ================================================

    RingModulatorLookAndFeel lookAndFeel;

    // MODULATOR
    Slider modulationRateSlider;
    std::unique_ptr<SliderAttachment> modulationRateAttachment;
    Slider modulationPulseWidthSlider;
    std::unique_ptr<SliderAttachment> modulationPulseWidthAttachment;
    Slider modulationShapeSlider;
    std::unique_ptr<SliderAttachment> modulationShapeAttachment;
    ToggleButton modulationInversionButton;
    std::unique_ptr<ButtonAttachment> modulationInversionButtonAttachment;
    Slider modulationInversionSlider;
    std::unique_ptr<SliderAttachment> modulationInversionSliderAttachment;

    // OUTPUT
    Slider levelSlider;
    std::unique_ptr<SliderAttachment> levelAttachment;
    Slider mixSlider;
    std::unique_ptr<SliderAttachment> mixAttachment;

    // =========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
