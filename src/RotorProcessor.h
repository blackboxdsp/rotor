/*
  ==============================================================================
    This file was auto-generated!
    It contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

//==============================================================================
/**
*/
class RotorAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    RotorAudioProcessor();
    ~RotorAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

   
    float getSkewFactor(float start, float end, float center);
    float getModulationInversion(bool inverted);
    void setPhaseDelta(double frequency, double sampleRate);
    void setWavetable(int waveformIndex);

private:
    //==========================================================================
    AudioProcessorValueTreeState parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==========================================================================

    // declare value tree state parameters - atomic<float>* is THREAD SAFE
    std::atomic<float>* modulationShape;             
    std::atomic<float>* modulationRate;              
    std::atomic<float>* modulationNoise;             
    std::atomic<float>* modulationIsInverted;        
    std::atomic<float>* pulseWidth;                  
    std::atomic<float>* level;                       
    std::atomic<float>* mix;  

    // previous values for specific parameters
    float previousShape;
    float modulationInversionFactor;
    float previousPulseWidth;
    float previousLevel;

    //==========================================================================

    // wavetable variables
    Array<float> wavetable;
    int wavetableSize;
    double currentPhase = 0.0;
    double previousPhase = 0.0;
    double phaseDelta = 0.0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotorAudioProcessor)
};