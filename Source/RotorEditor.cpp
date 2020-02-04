/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "./RotorEditor.h"
#include "./RotorProcessor.h"

//==============================================================================
RotorAudioProcessorEditor::RotorAudioProcessorEditor(RotorAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts)
{
    // GUI variables
    int textBoxWidth = 80;
    int textBoxHeight = 20;

    // MODULATOR ===============================================================

    // SHAPE
    modulationShapeSlider.setName("shape");
    modulationShapeSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationShapeSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    modulationShapeSlider.setDoubleClickReturnValue(true, 1);
    modulationShapeAttachment.reset(new SliderAttachment(valueTreeState, "waveform", modulationShapeSlider));
    modulationShapeSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&modulationShapeSlider);
    
    // RATE
    modulationRateSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationRateSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    modulationRateSlider.setDoubleClickReturnValue(true, 500.0f);
    modulationRateSlider.setTextValueSuffix(" Hz");
    modulationRateAttachment.reset(new SliderAttachment(valueTreeState, "rate", modulationRateSlider));
    modulationRateSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&modulationRateSlider);

    // NOISE
    modulationNoiseSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationNoiseSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    modulationNoiseSlider.setDoubleClickReturnValue(true, 0.0f);
    modulationNoiseAttachment.reset(new SliderAttachment(valueTreeState, "noise", modulationNoiseSlider));
    modulationNoiseSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&modulationNoiseSlider);

    // PULSE WIDTH
    modulationPulseWidthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationPulseWidthSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    modulationPulseWidthSlider.setDoubleClickReturnValue(true, 0.5f);
    modulationPulseWidthAttachment.reset(new SliderAttachment(valueTreeState, "pulseWidth", modulationPulseWidthSlider));
    modulationPulseWidthSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&modulationPulseWidthSlider);

    // INVERSION

    // slider 
    modulationInversionSlider.setName("inversion");
    modulationInversionSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    modulationInversionSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    modulationInversionSliderAttachment.reset(new SliderAttachment(valueTreeState, "inversion", modulationInversionSlider));
    modulationInversionSlider.setRotaryParameters(0.0f, MathConstants<float>::twoPi, true);
    modulationInversionSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&modulationInversionSlider);

    // button
    modulationInversionButtonAttachment.reset(new ButtonAttachment(valueTreeState, "inversion", modulationInversionButton));
    modulationInversionButton.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&modulationInversionButton);

    // OUTPUT ==================================================================

    // MIX
    mixSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    mixSlider.setTextValueSuffix(" %");
    mixSlider.setDoubleClickReturnValue(true, 50.0f);
    mixAttachment.reset(new SliderAttachment(valueTreeState, "mix", mixSlider));
    mixSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&mixSlider);

    // LEVEL
    levelSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    levelSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);
    levelSlider.setTextValueSuffix(" dB");
    levelSlider.setDoubleClickReturnValue(true, 0.0f);
    levelAttachment.reset(new SliderAttachment(valueTreeState, "level", levelSlider));
    levelSlider.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(&levelSlider);

    // ANALYZERS ===============================================================

    preAnalyzer = std::make_unique<RotorAnalyzer>();
    addAndMakeVisible(preAnalyzer.get());
    preAnalyzer->setColours(Colour::fromRGBA(126, 105, 251, 255),
                            Colour::fromRGBA(67, 42, 208, 255).withAlpha(0.7f),
                            Colour::fromRGBA(99, 78, 228, 255).withAlpha(0.7f));

    postAnalyzer = std::make_unique<RotorAnalyzer>();
    addAndMakeVisible(postAnalyzer.get());
    postAnalyzer->setColours(Colour::fromRGBA(167, 100, 251, 255),
                             Colour::fromRGBA(144, 78, 228, 255).withAlpha(0.7f),
                             Colour::fromRGBA(116, 42, 208, 255).withAlpha(0.7f));
    preAnalyzer->toBehind(postAnalyzer.get());

    //==========================================================================

    // handle open gl initializing
    glContext.setComponentPaintingEnabled(true);
    glContext.attachTo(*this);

    // set aspect ratio and dimensions accordingly
    double ratio = 0.5;
    setResizeLimits(360, 360 / ratio, 540, 540 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(480, 480 / ratio);

    // set lookAndFeel configurations
    setLookAndFeel(&lookAndFeel);
}

RotorAudioProcessorEditor::~RotorAudioProcessorEditor()
{
    // handle gl context
    glContext.detach();

    // look and feel
    setLookAndFeel(nullptr);

    // nullify attachments
    modulationRateAttachment = nullptr;
    modulationShapeAttachment = nullptr;
    modulationPulseWidthAttachment = nullptr;
    modulationInversionButtonAttachment = nullptr;
    mixAttachment = nullptr;
    levelAttachment = nullptr;
}

//==============================================================================
void RotorAudioProcessorEditor::paint (Graphics& g)
{
    // draw background image (this method allows for dynamic resizing)
    Image backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImage(backgroundImage, getLocalBounds().toFloat());
}

void RotorAudioProcessorEditor::resized()
{
    // get canvas
    auto area = getLocalBounds();

    // set layout variables 
    auto margin = getWidth() / 30;
    auto sectionSize = getHeight() / 5;
    auto largeDialSize = getWidth() * 13 / 45;
    auto smallDialSize = getWidth()  * 13 / 60;

    // set font size according to GUI editor's width
    int fontSize = getWidth() / 18; 
    lookAndFeel.setFontSize(fontSize);

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
    modulatorArea.removeFromTop(getWidth() * 13 / 90);

    // divide modulator into sections for [shape, rate, and noise] and [pulse width, and invert]
    auto modulatorAreaTop = modulatorArea.removeFromTop(largeDialSize);
    auto modulatorAreaBottom = modulatorArea.removeFromBottom(smallDialSize);

    // PULSE WIDTH
    modulatorAreaBottom.removeFromLeft(outputBlankSpace);
    modulationPulseWidthSlider.setBounds(modulatorAreaBottom.removeFromLeft(smallDialSize));

    // INVERT
    modulatorAreaBottom.removeFromRight(outputBlankSpace);
    auto inversionArea = modulatorAreaBottom.removeFromRight(smallDialSize);
    modulationInversionButton.setBounds(inversionArea);
    modulationInversionSlider.setBounds(inversionArea);

    // RATE
    auto copyArea = modulatorAreaTop;
    copyArea.removeFromLeft(area.getWidth() * 29 / 90);
    copyArea.removeFromRight(area.getWidth() * 29 / 90);
    modulationRateSlider.setBounds(copyArea);

    // SHAPE
    modulatorAreaTop.removeFromLeft(margin);
    auto shapeArea = modulatorAreaTop.removeFromLeft(smallDialSize);
    modulationShapeSlider.setBounds(shapeArea.removeFromTop(smallDialSize));

    // NOISE
    modulatorAreaTop.removeFromRight(margin);
    auto noiseArea = modulatorAreaTop.removeFromRight(smallDialSize);
    modulationNoiseSlider.setBounds(noiseArea.removeFromTop(smallDialSize));

    // ANALYZER ================================================================
    auto analyzerArea = area.removeFromBottom(sectionSize);
    analyzerArea.removeFromBottom(margin);
    preAnalyzer->setBounds(analyzerArea);
    postAnalyzer->setBounds(analyzerArea);
} 