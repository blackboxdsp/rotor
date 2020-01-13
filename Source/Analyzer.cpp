/*
  ==============================================================================

    Analyzer.cpp
    Created: 13 Jan 2020 10:50:34am
    Author:  Matt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Analyzer.h"

//==============================================================================
Analyzer::Analyzer()
    : forwardFFT(FFTOrder),
      window(FFTSize, dsp::WindowingFunction<float>::hann)
{
    zeromem(outputData, sizeof(outputData));
    setSize(240, 360);
    startTimerHz(30);
}

Analyzer::~Analyzer()
{
    stopTimer();
}

//==============================================================================
void Analyzer::paint (Graphics& g)
{
    // get dimensions
    const float width = (float) getWidth();
    const float height = (float) getHeight();

    // clear all drawing
    g.setColour(Colours::transparentBlack);
    g.fillAll();

    // get range and then set scale accordingly
    Range<float> maxRange = FloatVectorOperations::findMinAndMax(outputData, outputSize);
    const float scale = 1.0f / jmax((float)FFTSize, maxRange.getEnd());

    g.setColour(fillStart);

    for (int i = 0; i < outputSize; i++)
    {
        float x = std::log10(1 + 39 * ((i + 1.0f) / outputSize)) / std::log10(40.0f) * width;
        
        const float magnitude = outputData[i] * scale;
        const float decibels = Decibels::gainToDecibels(magnitude);
        const float y = jmap(decibels, -90.0f, -12.0f, height, 0.0f);

        g.drawVerticalLine((int)x, y, height);
    }
}

void Analyzer::resized()
{
}

void Analyzer::timerCallback()
{
    if (nextFFTBlockReady)
    {
        window.multiplyWithWindowingTable(FFTData, FFTSize);
        forwardFFT.performFrequencyOnlyForwardTransform(FFTData);

        // copy frequency bins into output data buffer using with
        // max(outputData[i], FFTData[i]). After the FFT calculation
        // on array A of size N, A[N / 2, N) is filled with zeros, and
        // A[0, N/4) is a mirror of A[N/4, N/2). Hence we only copy 
        // FFTSize / 2 samples into the output data.
        FloatVectorOperations::max(outputData, outputData, FFTData, outputSize);

        nextFFTBlockReady = false;
    }

    // gradually decay magnitudes
    for (int i = 0; i < outputSize; i++)
        outputData[i] *= 0.7f;

    repaint();
}

//==============================================================================
void Analyzer::pushBuffer(AudioSampleBuffer& buffer) 
{
    if (buffer.getNumChannels() > 0)
    {
        const auto* channelData = buffer.getReadPointer(0);

        for (int i = 0; i < buffer.getNumSamples(); i++)
            pushNextSample(channelData[i]);
    }
}

inline void Analyzer::pushNextSample(float sample)
{
    // if current index is at the end of array
    if (FFTQueueIndex == FFTSize)
    {
        // check that the block is done, fill with data from queue if not done
        if (!nextFFTBlockReady)
        {
            zeromem(FFTData, sizeof(FFTData));
            memcpy(FFTData, FFTQueue, sizeof(FFTQueue));
            nextFFTBlockReady = true;
        }
        // wrap index back to zero
        FFTQueueIndex = 0;
    }
    FFTQueue[FFTQueueIndex++] = sample;
}

void Analyzer::setColours(Colour stroke, Colour start, Colour stop)
{
    strokeColour = stroke;
    fillStart = start;
    fillStop = stop;
}