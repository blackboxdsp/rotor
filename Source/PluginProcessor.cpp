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
            std::make_unique<AudioParameterFloat>("gain",
                                                   "Gain",
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
                                                   500.0f),
            std::make_unique<AudioParameterInt>("modulationWaveform",
                                                "Modulation Waveform", 
                                                0,
                                                4,
                                                0)
        })
{
    // add parameters to state value tree
    gain = parameters.getRawParameterValue("gain");
    dryWet = parameters.getRawParameterValue("dryWet");
    modulationFrequency = parameters.getRawParameterValue("modulationFrequency");
    modulationWaveform = (int*) parameters.getRawParameterValue("modulationWaveform");
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
    // initialize previous gain value
    previousGain = *gain;

    // set frequency variables
    wavetableSize = samplesPerBlock;
    updatePhaseDelta(*modulationFrequency, sampleRate, wavetableSize);

    // set wavetable variables
    *modulationWaveform = (int) *parameters.getRawParameterValue("modulationWaveform");
    writeWavetable(*modulationWaveform);
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

    // loop through channels...
    for (int channel = 0; channel < totalNumInputChannels; channel += 1)
    {
        // Get pointer for channel from buffer method
        auto* channelData = buffer.getWritePointer (channel);

        // set current phase to previous block's last sample value (for continuity)
        currentPhase = previousPhase;
        
        // Loop through samples and do the processing...
        for (int sample = 0; sample < buffer.getNumSamples(); sample += 1)
        {
            // create two variables (original and processed)
            auto sampleData = channelData[sample], processedSampleData = channelData[sample];

            // multiply wavetable value by original signal and update phase value
            processedSampleData *= wavetable[(int) currentPhase];
            currentPhase = fmod(currentPhase + phaseDelta, wavetableSize);

            // write sampleData to specific sample in channelData adding wet (left) and dry (right)
            auto currentDryWet = *dryWet / 100.0f;
            
            // calculate according to current dry / wet value
            processedSampleData *= currentDryWet;
            sampleData *= (1.0f - currentDryWet);

            // add two sample values and check if > 1.0f
            auto sampleSum = processedSampleData + sampleData;

            // add the two signals and write to buffer
            channelData[sample] = sampleSum;
        }

        // update previous phase value to avoid discontinuities
        previousPhase = currentPhase;
    }

    // get current output gain and update if needed then apply it
    auto currentGain = pow(2, *gain / 6);
    if (previousGain == currentGain)
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
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
void RingModulatorAudioProcessor::updatePhaseDelta(double frequency, double sampleRate, double tableSize)
{
    auto cyclesPerSample = frequency / sampleRate;
    phaseDelta = cyclesPerSample * tableSize;
}

// returns skew factor that is skewed from the midpoint
float RingModulatorAudioProcessor::getSkewFactor(float start, float end, float center)
{
    return std::log((0.5f)) / std::log((center - start) / (end - start));
}

// return integer indicating number of samples in wavetable
int RingModulatorAudioProcessor::getWavetableSize()
{
    return wavetableSize;
}

void RingModulatorAudioProcessor::writeWavetable(int waveformIndex)
{
    // clear the wavetable
    wavetable.clear();

    // check values and compute accordingly
    switch (waveformIndex)
    {
        // SINE
        default:
        case 0:
            for (int i = 0; i < wavetableSize; i++)
            {
                wavetable.insert(i, sin((MathConstants<double>::twoPi * i) / wavetableSize));
            }
            break;
        // TRIANGLE
        case 1:
            for (int i = 0; i < wavetableSize / 2; i++)
            {
                float waveformValue = (MathConstants<float>::twoPi * i) / (wavetableSize / 2);
                wavetable.insert(i, waveformValue);
            }
            for (int i = wavetableSize / 2; i < wavetableSize; i++)
            {
                wavetable.insert(i, wavetable[-1 * (i + 1)]);
            }
            break;
        // SAWTOOTH
        case 2:
            for (int i = 0; i < wavetableSize; i++)
            {
                float waveformValue = (MathConstants<float>::twoPi * i) / wavetableSize;
                wavetable.insert(i, waveformValue);
            }
            break;
        // SQUARE
        case 3:
            for (int i = 0; i < wavetableSize / 2; i++)
            {
                wavetable.insert(i, 1.0f);
            }
            for (int i = wavetableSize / 2; i < wavetableSize; i++)
            {
                wavetable.insert(i, 0.0f);
            }
            break;
        // NOISE
        case 4:
            for (int i = 0; i < wavetableSize; i++)
            {
                wavetable.insert(i, Random::getSystemRandom().nextFloat() - Random::getSystemRandom().nextFloat());
            }
            break;
    }
}