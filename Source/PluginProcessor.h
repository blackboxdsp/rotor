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
    void updatePhaseDelta(double frequency, double sampleRate, double tableSize);
    float getSkewFactor(float start, float end, float center);
    int getWavetableSize();
    void writeWavetable(int waveformIndex);

    // Gain parameter declarations
    float* inputGain;
    float* outputGain;

    // Dry / wet parameter declaration
    float* dryWet;

    // Modulation parameter declarations
    float* modulationFrequency;
    int* modulationWaveform;

    // variable for waveform choices
    const StringArray waveformChoices =
    {
        "Sine",
        "Triangle",
        "Sawtooth",
        "Square",
        "Noise"
    };

private:
    // for storing / retrieving parameters
    AudioProcessorValueTreeState parameters;
    
    // variables for gain ramps --- 0.0f to 1.0f (same for current gain in .cpp ^)
    float previousInputGain;
    float previousOutputGain;

    // wavetable variables
    Array<float> wavetable;
    int wavetableSize;
    double currentPhase = 0.0;
    double previousPhase = 0.0;
    double phaseDelta = 0.0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessor)
};
