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
    ),
#endif
    parameters(*this, nullptr, Identifier("RingModulator"),
        {
            std::make_unique<AudioParameterFloat>("inputGain",
                                                   "Input Gain",
                                                   -36.0f,
                                                   0.0f,
                                                   0.0f),
            std::make_unique<AudioParameterFloat>("outputGain",
                                                   "Output Gain",
                                                   -36.0f,
                                                   0.0f,
                                                   0.0f),
            std::make_unique<AudioParameterFloat>("dryWet",
                                                   "Dry / Wet",
                                                   0.0f,
                                                   100.0f,
                                                   50.0f),
            std::make_unique<AudioParameterFloat>("modulationFrequency",
                                                   "Modulation Frequency",
                                                   NormalisableRange<float>(10.0f, 
                                                                            12000.0f, 
                                                                            1.0f, 
                                                                            getSkewFactor(10.0f, 12000.0f, 500.0f),
                                                                            false),
                                                   500.0f)
        })
{
    // Parameter definitions
    const StringArray waveformChoices =
    {
        "Sine",
        "Sawtooth",
        "Square",
        "Triangle"
    };
    addParameter(modulationWaveform = new AudioParameterChoice("modulationWaveform", "Modulation Frequency", waveformChoices, 0));

    // add parameters to state value tree
    inputGain = parameters.getRawParameterValue("inputGain");
    outputGain = parameters.getRawParameterValue("outputGain");
    dryWet = parameters.getRawParameterValue("dryWet");
    modulationFrequency = parameters.getRawParameterValue("modulationFrequency");
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
    // initialize previous gain values
    previousInputGain = *inputGain;
    previousOutputGain = *outputGain;

    // handle frequency parameter stuff
    updateAngleDelta(*modulationFrequency, sampleRate);
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

    // get current input gain (in 0.0 - 1.0) and update if needed then apply it
    auto currentInputGain = pow(2, *inputGain / 6);
    if (previousInputGain == currentInputGain)
    {
        buffer.applyGain(currentInputGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousInputGain, currentInputGain);
        previousInputGain = currentInputGain;
    }

    // loop through channels...
    for (int channel = 0; channel < totalNumInputChannels; channel += 1)
    {
        // Get pointer for channel from buffer method
        auto* channelData = buffer.getWritePointer (channel);
        
        // Loop through samples and do the processing...
        for (int sample = 0; sample < buffer.getNumSamples(); sample += 1)
        {
            // create two variables (original and processed)
            auto sampleData = channelData[sample], previousSampleData = channelData[sample];

            // processing here...
            previousSampleData *= (float) std::sin(currentAngle);
            currentAngle += angleDelta;

            // write sampleData to specific sample in channelData adding wet (left) and dry (right)
            auto currentDryWet = *dryWet / 100.0f;
            channelData[sample] = (previousSampleData * currentDryWet) + (sampleData * (1.0f - currentDryWet));
        }
    }

    // get current output gain and update if needed then apply it
    auto currentOutputGain = pow(2, *outputGain / 6);
    if (previousOutputGain == currentOutputGain)
    {
        buffer.applyGain(currentOutputGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousOutputGain, currentOutputGain);
        previousOutputGain = currentOutputGain;
    }
}

//==============================================================================
bool RingModulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RingModulatorAudioProcessor::createEditor()
{
    return new RingModulatorAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void RingModulatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RingModulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingModulatorAudioProcessor();
}

// updates angle delta provided a frequency value from slider
void RingModulatorAudioProcessor::updateAngleDelta(double frequency, double sampleRate)
{
    auto cyclesPerSample = frequency / sampleRate;
    angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
}

// returns skew factor that is skewed from the midpoint
float RingModulatorAudioProcessor::getSkewFactor(float start, float end, float center)
{
    return std::log((0.5f)) / std::log((center - start) / (end - start));
}
