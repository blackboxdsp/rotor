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
    int textBoxHeight = 20;

    // set aspect ratio and dimensions accordingly
    double ratio = 0.5;
    setResizeLimits(360, 360 / ratio, 540, 540 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize (480, 480 / ratio);

    // handle open gl initializing
    glContext.setComponentPaintingEnabled(true);
    glContext.attachTo(*this);

    // SLIDERS =================================================================

    // set slider styles
    modulationRateSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationShapeSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationPulseWidthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    mixSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    levelSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    // set textbox styles
    modulationRateSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    modulationShapeSlider.setTextBoxStyle(Slider::NoTextBox, true, textBoxWidth, textBoxHeight);
    modulationPulseWidthSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    levelSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);

    // set text value suffixes
    modulationRateSlider.setTextValueSuffix(" Hz");

    mixSlider.setTextValueSuffix(" %");
    levelSlider.setTextValueSuffix(" dB");

    // set double click return values
    modulationRateSlider.setDoubleClickReturnValue(true, 500.0f);
    modulationShapeSlider.setDoubleClickReturnValue(true, 1);
    modulationPulseWidthSlider.setDoubleClickReturnValue(true, 0.5f);

    mixSlider.setDoubleClickReturnValue(true, 50.0f);
    levelSlider.setDoubleClickReturnValue(true, 0.0f);

    // attach valueTreeState attachments
    modulationRateAttachment.reset(new SliderAttachment(valueTreeState, "rate", modulationRateSlider));
    modulationShapeAttachment.reset(new SliderAttachment(valueTreeState, "waveform", modulationShapeSlider));
    modulationPulseWidthAttachment.reset(new SliderAttachment(valueTreeState, "pulseWidth", modulationPulseWidthSlider));

    mixAttachment.reset(new SliderAttachment(valueTreeState, "mix", mixSlider));
    levelAttachment.reset(new SliderAttachment(valueTreeState, "level", levelSlider));

    // add all sliders and make visible 
    addAndMakeVisible(&modulationRateSlider);
    addAndMakeVisible(&modulationShapeSlider);
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
    
    // ONVALUECHANGE DEFINITIONS ========================================

    // update the phase delta whenever frequency slider is updated
    modulationRateSlider.onValueChange = [this]
    {
        auto currentSampleRate = processor.getSampleRate();
        if (currentSampleRate > 0.0)
        {
            auto frequency = modulationRateSlider.getValue();
            processor.updatePhaseDelta(frequency, currentSampleRate);
        }
    };

    // write a new wavetable whenever new item is selected
    modulationShapeSlider.onValueChange = [this]
    {
        processor.writeWavetable((int) modulationShapeSlider.getValue());
    };

    // write new wavetable when square is selected and pulse width slider is changed
    modulationPulseWidthSlider.onValueChange = [this]
    {
        if ((int) modulationShapeSlider.getValue() == 3)
        {
            processor.writeWavetable(3);
        }
    };
}

RingModulatorAudioProcessorEditor::~RingModulatorAudioProcessorEditor()
{
    // handle gl context
    glContext.detach();

    // nullify attachments
    modulationRateAttachment = nullptr;
    modulationShapeAttachment = nullptr;
    modulationPulseWidthAttachment = nullptr;
    modulationInversionAttachment = nullptr;
    mixAttachment = nullptr;
    levelAttachment = nullptr;
}

//==============================================================================
void RingModulatorAudioProcessorEditor::paint (Graphics& g)
{
    // draw background image (this method allows for dynamic resizing)
    Image backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImage(backgroundImage, getLocalBounds().toFloat());
}

void RingModulatorAudioProcessorEditor::resized()
{
    // get canvas
    auto area = getLocalBounds();

    // set layout variables 
    auto margin = getWidth() * 1 / 30;
    auto sectionSize = getHeight() / 5;
    auto largeDialSize = getWidth() * 13 / 45;
    auto smallDialSize = getWidth()  * 13 / 60;

    // remove margins from sides
    area.removeFromLeft(margin);
    area.removeFromRight(margin);

    // OUTPUT ==================================================================
   
    // designate area for drawing output's parameters' sliders
    auto outputArea = area.removeFromBottom(sectionSize);
    outputArea.removeFromBottom(margin * 2);
    outputArea = outputArea.removeFromBottom(smallDialSize);

    // MIX
    const int outputBlankSpace = getWidth() * 5 / 36;
    outputArea.removeFromLeft(outputBlankSpace); // space between outline and slider
    mixSlider.setBounds(outputArea.removeFromLeft(smallDialSize));

    // LEVEL
    outputArea.removeFromRight(outputBlankSpace);
    levelSlider.setBounds(outputArea.removeFromRight(smallDialSize));

    // MODULATOR ===============================================================

    // designate area for drawing modulator's parameters' sliders
    auto modulatorArea = area.removeFromBottom(sectionSize * 2);
    modulatorArea.removeFromBottom(margin * 2);

    // divide modulator into sections for [rate] and [pulse width, shape, inversion]
    auto secondaryArea = modulatorArea.removeFromBottom(smallDialSize);

    // PULSE WIDTH
    secondaryArea.removeFromLeft(margin);
    modulationPulseWidthSlider.setBounds(secondaryArea.removeFromLeft(smallDialSize));

    // INVERT
    secondaryArea.removeFromRight(margin);
    modulationInversionButton.setBounds(secondaryArea.removeFromRight(smallDialSize / 2));
    secondaryArea.removeFromRight(smallDialSize / 2);

    // SHAPE
    secondaryArea.removeFromLeft(smallDialSize / 2);
    modulationShapeSlider.setBounds(secondaryArea.removeFromLeft(smallDialSize));

    // RATE
    modulatorArea.removeFromBottom(sectionSize * 7 / 24);
    modulatorArea = modulatorArea.removeFromBottom(largeDialSize);
    auto modulatorBlankSpace = modulatorArea.getWidth() * 29 / 84;
    modulatorArea.removeFromLeft(modulatorBlankSpace);
    modulatorArea.removeFromRight(modulatorBlankSpace);
    modulationRateSlider.setBounds(modulatorArea);
}