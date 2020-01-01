/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModulatorAudioProcessor::RingModulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    // Parameter definitions
    addParameter(inputGain = new AudioParameterFloat("inputGain", "Input Gain", 0.0f, 1.0f, 1.0f));
    addParameter(outputGain = new AudioParameterFloat("outputGain", "Output Gain", 0.0f, 1.0f, 1.0f));
    addParameter(dryWet = new AudioParameterFloat("dryWet", "Dry / Wet", 0.0f, 1.0f, 1.0f));
    const StringArray waveformChoices =
    {
        "Sine",
        "Sawtooth",
        "Square",
        "Triangle"
    };
    addParameter(modulationWaveform = new AudioParameterChoice("modulationWaveform", "Modulation Frequency", waveformChoices, 0));
    addParameter(modulationFrequency = new AudioParameterFloat("modulationFrequency", "Modulation Frequency", NormalisableRange<float> (20.0f, 15000.0f), 250.0f));
}

RingModulatorAudioProcessor::~RingModulatorAudioProcessor()
{
}

//==============================================================================
const String RingModulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RingModulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RingModulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RingModulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RingModulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RingModulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RingModulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RingModulatorAudioProcessor::setCurrentProgram (int index)
{
}

const String RingModulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void RingModulatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void RingModulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // initialize the gain floats
    previousInputGain = *inputGain;
    previousOutputGain = *outputGain;
}

void RingModulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RingModulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RingModulatorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    // get current input gain and update if needed then apply it
    auto currentInputGain = inputGain->get();
    previousInputGain = setBufferGain(previousInputGain, currentInputGain, buffer);

    // loop through channels...
    for (int channel = 0; channel < totalNumInputChannels; channel += 1)
    {
        // Get pointer for channel from buffer method
        auto* channelData = buffer.getWritePointer (channel);
        
        // Loop through samples and do the processing...
        for (int sample = 0; sample < buffer.getNumSamples(); sample += 1)
        {
            // create two variables (original and processed)
            auto sampleData = channelData[sample], p_sampleData = channelData[sample];

            // other processing here...

            // write sampleData to specific sample in channelData adding wet (left) and dry (right)
            channelData[sample] = (p_sampleData * dryWet->get()) + (sampleData * (1.0f - dryWet->get()));
        }
    }

    // get current output gain and update if needed then apply it
    auto currentOutputGain = outputGain->get();
    previousOutputGain = setBufferGain(previousOutputGain, currentOutputGain, buffer);
}

//==============================================================================
bool RingModulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RingModulatorAudioProcessor::createEditor()
{
    return new RingModulatorAudioProcessorEditor (*this);
}

//==============================================================================
void RingModulatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RingModulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingModulatorAudioProcessor();
}

float RingModulatorAudioProcessor::setBufferGain(float p_gain, float c_gain, AudioBuffer<float>& buffer)
{
    if (p_gain == c_gain)
    {
        buffer.applyGain(c_gain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), p_gain, c_gain);
    }
    return(c_gain);
}
