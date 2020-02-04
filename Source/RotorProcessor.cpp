/*
  ==============================================================================
    This file was auto-generated!
    It contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "./RotorEditor.h"
#include "./RotorProcessor.h"

//==============================================================================
RotorAudioProcessor::RotorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : parameters(*this, nullptr, Identifier("Rotor"), createParameterLayout()),
      AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    // init parameters from valueTreeState
    previousShape               = new float(0.0f);
    modulationShape             = parameters.getRawParameterValue("waveform");
    modulationRate              = parameters.getRawParameterValue("rate");
    modulationNoise             = parameters.getRawParameterValue("noise");
    modulationIsInverted        = parameters.getRawParameterValue("inversion");
    modulationInversionFactor   = new float(1.0f);
    pulseWidth                  = parameters.getRawParameterValue("pulseWidth");
    previousPulseWidth          = new float(0.5f);
    level                       = parameters.getRawParameterValue("level"); 
    mix                         = parameters.getRawParameterValue("mix");
}

RotorAudioProcessor::~RotorAudioProcessor()
{
}

//==============================================================================
AudioProcessorValueTreeState::ParameterLayout RotorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // MODULATOR ===============================================================

    // WAVEFORM
    auto p_modulationShape = std::make_unique<AudioParameterInt>(
        "waveform", "Waveform", 0, 4, 0);
    params.push_back(std::move(p_modulationShape));

    // RATE
    auto p_modulationRate = std::make_unique<AudioParameterFloat>(
        "rate", "Rate",
        NormalisableRange<float>(10.0f, 12000.0f, 1.0f, getSkewFactor(10.0f, 12000.0f, 500.0f), false), 
        500.0f);
    params.push_back(std::move(p_modulationRate));

    // NOISE
    auto p_modulationNoise = std::make_unique<AudioParameterFloat>(
        "noise", "Noise", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(p_modulationNoise));

    // PULSE WIDTH
    auto p_pulseWidth = std::make_unique<AudioParameterFloat>(
        "pulseWidth", "Pulse Width",
        NormalisableRange<float>(0.01f, 0.99f, 0.1f), // any lower than 0.1f gets mehh performance...
        0.5f);
    params.push_back(std::move(p_pulseWidth));

    // INVERSION
    auto p_modulationIsInverted = std::make_unique<AudioParameterBool>(
        "inversion",
        "Inversion",
        false);
    params.push_back(std::move(p_modulationIsInverted));

    // OUTPUT ==================================================================

    // MIX
    auto p_mix = std::make_unique<AudioParameterFloat>(
        "mix", "Mix", 0.0f, 100.0f, 50.0f);
    params.push_back(std::move(p_mix));

    // LEVEL
    auto p_level = std::make_unique<AudioParameterFloat>(
        "level", "Level",
        NormalisableRange<float>(-60.0f, 18.0f, 0.0f, getSkewFactor(-60.0f, 18.0f, 0.0f), false),
        0.0f);
    params.push_back(std::move(p_level));

    return { params.begin(), params.end() };
}

const String RotorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RotorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool RotorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool RotorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double RotorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RotorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RotorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RotorAudioProcessor::setCurrentProgram(int index)
{
}

const String RotorAudioProcessor::getProgramName(int index)
{
    return {};
}

void RotorAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void RotorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // set the inversion factor
    *modulationInversionFactor = getModulationInversion((bool) *modulationIsInverted);

    // initialize previous gain value
    previousLevel = powf(2, *level / 6);

    // set frequency variables
    wavetableSize = 1024;
    setPhaseDelta((double) *modulationRate, sampleRate);

    // write the wavetable according to current waveform   
    *previousShape = *modulationShape;
    setWavetable((int) *modulationShape);

    // set pulse width stuff
    *previousPulseWidth = *pulseWidth;
}

void RotorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RotorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
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

void RotorAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // write new wavetable if pulse width changes
    if (*previousPulseWidth != *pulseWidth)
    {
        *previousPulseWidth = *pulseWidth;
        setWavetable((int) *modulationShape);
    }

    // write new wavetable if waveform selection change
    if (*previousShape != *modulationShape)
    {
        *previousShape = *modulationShape;
        setWavetable((int) *modulationShape);
    }
    // update phase delta for wavetable
    setPhaseDelta((double) *modulationRate, this->getSampleRate());

    // update inversion factor
    *modulationInversionFactor = getModulationInversion((bool) *modulationIsInverted);

    // grab reference to the editor (for the analyzer)
    RotorAudioProcessorEditor* editor = static_cast<RotorAudioProcessorEditor*>(getActiveEditor());

    // push unprocessed buffer to the pre analyzer
    if (editor)
        editor->preAnalyzer->pushBuffer(buffer);

    for (auto sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        // set current phase to previous block's last sample value (for continuity)
        currentPhase = previousPhase;

        // get write pointer to buffer samples
        auto* samples = buffer.getWritePointer(0);

        // read first sample value into two variables (original [o_sampleData] and processed [p_sampleData])
        auto o_sampleData = samples[sample], p_sampleData = samples[sample];

        // multiply wavetable value by original signal and update phase value
        p_sampleData *= (wavetable[(int) currentPhase] * *modulationInversionFactor);
        currentPhase = fmod(currentPhase + phaseDelta, wavetableSize);

        // read current mix ratio value and scale between 0.0f and 1.0f (** DOES NOT CHECK IF MIX IS > 100.0f **)
        auto currentDryWet = *mix / 100.0f;

        // calculate values of dry and wet signals according to currentDryWet
        p_sampleData *= currentDryWet;
        o_sampleData *= (1.0f - currentDryWet);

        // calculate random noise and udpate p_sampleData accordingly to noise parameter
        auto n_sampleData = p_sampleData * Random::getSystemRandom().nextFloat();
        n_sampleData *= *modulationNoise;
        p_sampleData *= (1.0f - *modulationNoise);
        p_sampleData += n_sampleData;

        // calculate result sample value (r_sampleData)
        auto r_sampleData = p_sampleData + o_sampleData;

        // loop through channels and set samples accordingly
        for (auto channel = 0; channel < buffer.getNumChannels(); channel++)
            buffer.setSample(channel, sample, r_sampleData);

        // update previous phase value to avoid discontinuities
        previousPhase = currentPhase;
    }

    // get current output gain and update if needed then apply it
    auto currentGain = powf(2, *level / 6);
    if (previousLevel == currentGain)
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousLevel, currentGain);
        previousLevel = currentGain;
    }

    // push processed buffer to the post analyzer
    if (editor)
        editor->postAnalyzer->pushBuffer(buffer);
}

//==============================================================================
bool RotorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RotorAudioProcessor::createEditor()
{
    return new RotorAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void RotorAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RotorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
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
    return new RotorAudioProcessor();
}

//==============================================================================

// returns skew factor that is skewed from the midpoint
float RotorAudioProcessor::getSkewFactor(float start, float end, float center)
{
    return std::log((0.5f)) / std::log((center - start) / (end - start));
}

// switches modulation inversion factor between values -1.0f and 1.0f
float RotorAudioProcessor::getModulationInversion(bool inverted)
{
    if (inverted)
        return -1.0f;
    return 1.0f;
}

// updates angle delta provided a frequency value from slider
void RotorAudioProcessor::setPhaseDelta(double frequency, double sampleRate)
{
    auto cyclesPerSample = frequency / sampleRate;
    phaseDelta = cyclesPerSample * (double) wavetableSize;
}

// writes wavetable according to currently selected waveform
void RotorAudioProcessor::setWavetable(int waveformIndex)
{
    // clear the wavetable
    wavetable.clear();

    // set pulse width 
    int pulseWidthSize = *pulseWidth * wavetableSize;

    // check values and compute accordingly
    switch (waveformIndex)
    {
        // SINE
        default:
        case 0:
            for (int i = 0; i < wavetableSize; i++)
            {
                auto waveformValue = 1.0f + 0.5f * sinf(MathConstants<float>::twoPi * (float) i / wavetableSize);
                wavetable.insert(i, waveformValue);
            }
            break;

        // TRIANGLE
        case 1:
            for (int i = 0; i < wavetableSize / 2; i++)
            {
                float waveformValue = (float) i / (wavetableSize / 2);
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
                float waveformValue = (float) i / wavetableSize;
                wavetable.insert(i, waveformValue);
            }
            break;

        // SQUARE
        case 3:
            for (int i = 0; i < pulseWidthSize; i++)
            {
                wavetable.insert(i, 1.0f);
            }
            for (int i = pulseWidthSize; i < wavetableSize; i++)
            {
                wavetable.insert(i, 0.0f);
            }
            break;

        // SEMI-SINE
        case 4:
            for (int i = 0; i < wavetableSize; i++)
            {
                auto waveformValue = sinf(MathConstants<float>::pi * (float) i / wavetableSize);
                wavetable.insert(i, waveformValue);
            }
            break;
    }
}
