/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class RingModulatorAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    RingModulatorAudioProcessor();
    ~RingModulatorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // custom methods
    void updateSmoothedValues();

    // Gain parameter declarations
    AudioParameterFloat* inputGain;
    AudioParameterFloat* outputGain;

    // Dry / wet parameter declaration
    AudioParameterFloat* dryWet;

    // Modulation parameter declarations
    AudioParameterChoice* modulationWaveform;
    AudioParameterFloat* modulationFrequency;

private:
    // Smoothed values for parameters (gain)
    SmoothedValue<float, ValueSmoothingTypes::Linear> s_inputGain;
    SmoothedValue<float, ValueSmoothingTypes::Linear> s_outputGain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessor)
};
