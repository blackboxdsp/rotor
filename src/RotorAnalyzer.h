/*
  ==============================================================================

    RotorAnalyzer.h
    Created: 13 Jan 2020 10:50:34am
    Author:  Matthew Maxwell

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class RotorAnalyzer : public Component,
                      private Timer
{
public:
    RotorAnalyzer();
    ~RotorAnalyzer();

    //==========================================================================
    void paint(Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void pushBuffer(AudioSampleBuffer& buffer);
    inline void pushNextSample(float sample);

    void setColours(Colour stroke, Colour fillStart, Colour fillStop);

    //==========================================================================
    enum
    {
        FFTOrder = 11,
        FFTSize = 1 << FFTOrder,            // 2048
        outputSize = FFTSize / 2            // 1024
    };

private:

    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;

    float FFTQueue[FFTSize];            // contains incoming audio data in samples
    float FFTData[2 * FFTSize];         // contains results of FFT calculations
    float outputData[outputSize];       

    unsigned int FFTQueueIndex = 0;     // keeps count of amount of samples in queue
    bool nextFFTBlockReady = false;

    Colour fillStart = Colours::white.withAlpha(0.2f);
    Colour fillStop = Colours::white.withAlpha(0.8f);
    Colour strokeColour = Colours::white;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotorAnalyzer)
};
