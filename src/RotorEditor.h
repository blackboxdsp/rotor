/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "./RotorAnalyzer.h"
#include "./RotorLookAndFeel.h"
#include "./RotorProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/**
*/
class RotorAudioProcessorEditor : public AudioProcessorEditor
{
public:
    RotorAudioProcessorEditor(RotorAudioProcessor&, AudioProcessorValueTreeState&);
    ~RotorAudioProcessorEditor();

    //==========================================================================

    void paint (Graphics&) override;
    void resized() override;

    //==========================================================================
    std::unique_ptr<RotorAnalyzer> preAnalyzer;
    std::unique_ptr<RotorAnalyzer> postAnalyzer;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RotorAudioProcessor& processor;

    // Reference to value tree state for quick access
    AudioProcessorValueTreeState& valueTreeState;

    // for better rendering
    OpenGLContext glContext;
    
    // GUI COMPONENTS ================================================

    RotorLookAndFeel lookAndFeel;

    // MODULATOR
    Slider                              modulationShapeSlider;
    std::unique_ptr<SliderAttachment>   modulationShapeAttachment;

    Slider                              modulationRateSlider;
    std::unique_ptr<SliderAttachment>   modulationRateAttachment;

    Slider                              modulationNoiseSlider;
    std::unique_ptr<SliderAttachment>   modulationNoiseAttachment;

    Slider                              modulationPulseWidthSlider;
    std::unique_ptr<SliderAttachment>   modulationPulseWidthAttachment;

    ToggleButton                        modulationInversionButton;
    std::unique_ptr<ButtonAttachment>   modulationInversionButtonAttachment;

    Slider                              modulationInversionSlider;
    std::unique_ptr<SliderAttachment>   modulationInversionSliderAttachment;

    // OUTPUT
    Slider                              levelSlider;
    std::unique_ptr<SliderAttachment>   levelAttachment;

    Slider                              mixSlider;
    std::unique_ptr<SliderAttachment>   mixAttachment;

    // =========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotorAudioProcessorEditor)
};
