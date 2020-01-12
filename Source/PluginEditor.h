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
class MainLookAndFeel : public LookAndFeel_V4
{
public:
    MainLookAndFeel()
    {
        setColour(Slider::thumbColourId, Colours::transparentWhite);
    }

    void drawRotarySlider(Graphics& g, 
                          int x, 
                          int y, 
                          int width,
                          int height, 
                          float sliderPos, 
                          const float rotaryStartAngle, 
                          const float rotaryEndAngle, 
                          Slider& slider) override
    {
        // init and get all important variables for drawing
        auto radius = jmin(width / 2, height / 2) - 4.0f;
        auto centerX = x + width * 0.5f;
        auto centerY = y + height * 0.5f;
        auto radX = centerX - radius;
        auto radY = centerY - radius;
        auto radW = radius * 2.0f;

        // pointer drawing
        Path p;
        auto strokeWidth = 2.0f;
        p.addArc(x, y, width, height, rotaryStartAngle, rotaryEndAngle);

        g.setColour(Colours::purple);
        g.fillPath(p);
    }
};

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
    std::unique_ptr<ButtonAttachment> modulationInversionAttachment;

    // OUTPUT
    Slider levelSlider;
    std::unique_ptr<SliderAttachment> levelAttachment;
    Slider mixSlider;
    std::unique_ptr<SliderAttachment> mixAttachment;

    // =========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
