/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyUtilityAudioProcessor::MyUtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("gain", this);
    treeState.addParameterListener("mute", this);
    treeState.addParameterListener("phase", this);
    treeState.addParameterListener("mono", this);
    treeState.addParameterListener("balance", this);
    treeState.addParameterListener("delay", this);
    treeState.addParameterListener("width", this);
}

MyUtilityAudioProcessor::~MyUtilityAudioProcessor()
{
    treeState.removeParameterListener("gain", this);
    treeState.removeParameterListener("mute", this);
    treeState.removeParameterListener("phase", this);
    treeState.removeParameterListener("mono", this);
    treeState.removeParameterListener("balance", this);
    treeState.removeParameterListener("delay", this);
    treeState.removeParameterListener("width", this);

}

juce::AudioProcessorValueTreeState::ParameterLayout MyUtilityAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //make sure to update number of reservations after adding params
    params.reserve(7);
    
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Gain", -66.0f, 24.0f, 0.0f);
    auto pMute = std::make_unique<juce::AudioParameterBool>("mute", "Mute", 0);
    auto pPhase = std::make_unique<juce::AudioParameterBool>("phase", "Phase", 0);
    auto pMono = std::make_unique<juce::AudioParameterBool>("mono", "Mono", 0);
    auto pBalance = std::make_unique<juce::AudioParameterFloat>("balance", "Balance", -1.0, 1.0, 0);
    auto pDelay = std::make_unique<juce::AudioParameterFloat>("delay", "Sample Delay", juce::NormalisableRange<float> (0.0f, 48000.0f, 1.0f, 0.25f), 0.0f);
    auto pWidth = std::make_unique<juce::AudioParameterFloat>("width", "M/S Width", 0.0f, 5.0f, 1.0f);
    
    params.push_back(std::move(pGain));
    params.push_back(std::move(pMute));
    params.push_back(std::move(pPhase));
    params.push_back(std::move(pMono));
    params.push_back(std::move(pBalance));
    params.push_back(std::move(pDelay));
    params.push_back(std::move(pWidth));

    return { params.begin(), params.end() };
}

void MyUtilityAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    smoothGain.setTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));

    if(parameterID == "mute")
    {
        mute = newValue;
        auto varMute = mute < 0.5 ? 1.0 : 0.0;
        smoothMute.setTargetValue(varMute);
    }
    if(parameterID  == "phase")
    {
        phase = newValue;
        auto varPhase = phase < 0.5 ? 1.0 : -1.0;
        smoothPhase.setTargetValue(varPhase);
    }
    if(parameterID == "mono")
    {
        mono = newValue;
    }
    if(parameterID == "balance")
    {
        balance = newValue;
    }
    if(parameterID == "delay")
    {
        std::fill(delayValue.begin(), delayValue.end(), newValue);
    }
    if(parameterID == "width")
    {
        width = newValue;
    }
}

//==============================================================================
const juce::String MyUtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyUtilityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyUtilityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyUtilityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyUtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyUtilityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyUtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyUtilityAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyUtilityAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyUtilityAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyUtilityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    prepare(sampleRate, samplesPerBlock);
}

void MyUtilityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyUtilityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void MyUtilityAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //mono processing function call
    monoProcessing(buffer, mono, totalNumInputChannels);
    
    //delay processing function call
    delayProcessing(buffer, totalNumInputChannels, totalNumOutputChannels);
    
    //width processing function call // mid/side
    widthProcessing(buffer);
    
    //Target value of smoothGain coming from gain slider
    smoothGain.setTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));
    
    //Target value of smoothMute derived from a tenary operator that uses the mute bool value as the condition
    mute = *treeState.getRawParameterValue("mute");
    auto varMute = mute < 0.5 ? 1.0 : 0.0;
    smoothMute.setTargetValue(varMute);
    
    //Target value of smoothPhase derived from a tenary operator that uses the phase bool value as the condition
    phase = *treeState.getRawParameterValue("phase");
    auto varPhase = phase < 0.5 ? 1.0 : -1.0;
    smoothPhase.setTargetValue(varPhase);
    
    // pan variable connection to dsp panner
    panner.setPan(static_cast<float>(*treeState.getRawParameterValue("balance")));
    
    // My audio block object
    juce::dsp::AudioBlock<float> block (buffer);

    // My dsp block
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            channelData[sample] *= smoothGain.getNextValue() * smoothPhase.getNextValue() * smoothMute.getNextValue();
        }
    }
    
    // dsp panner process context replacing
    panner.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//mono processing function
void MyUtilityAudioProcessor::monoProcessing(juce::AudioBuffer<float> &buffer, bool mono, int totalNumInputChannels)
{
    // Mono
    // Converts Stereo to Mono. If audio is stereo and mono button is 1, then signal will be mono'd
    
    if (totalNumInputChannels == 2 && mono == 1)
    {
        // add the right (1) to the left (0)
        // store the sum in the left
        buffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
        
        // copy the combined left (0) to the right (1)
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
    
        // apply 0.5 gain to both
        buffer.applyGain(0.5f);
    }
}

//delay processing function
void MyUtilityAudioProcessor::delayProcessing(juce::AudioBuffer<float> &buffer, int totalNumInputChannels, int totalNumOutputChannels)
{
    const auto numChannels = juce::jmax (totalNumInputChannels, totalNumOutputChannels);

    auto audioBlock = juce::dsp::AudioBlock<float> (buffer).getSubsetChannelBlock (0, (size_t) numChannels);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();
    
    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* samplesIn = input.getChannelPointer (channel);
        auto* samplesOut = output.getChannelPointer (channel);
           
        for (size_t sample = 0; sample < input.getNumSamples(); ++sample)
        {
            auto input = samplesIn[sample] - lastDelayOutput[channel]; // not sure I need the 'lastDelayOutput' as it is for feedback...
            auto delayAmount = delayValue[channel];

            linear.pushSample (int (channel), input);
            linear.setDelay ((float) delayAmount);
            samplesOut[sample] = linear.popSample ((int) channel);
        }
    }
}

// mid/side processing function
void MyUtilityAudioProcessor::widthProcessing(juce::AudioBuffer<float> &buffer)
{
    const auto localWidth = treeState.getRawParameterValue("width")->load();
    const auto coef_M = 1/std::fmax(1 + localWidth, 2);
    const auto coef_S = localWidth * coef_M;
    
    for(int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* leftChannel = buffer.getWritePointer (0);
        auto* rightChannel = buffer.getWritePointer (1);
        
        for (int i = 0; i  < buffer.getNumSamples(); ++i)
        {
            const auto sampleLeft = leftChannel[i];
            const auto sampleRight = rightChannel[i];

            const auto mid = coef_M * (sampleLeft + sampleRight);
            const auto side = coef_S * (sampleRight - sampleLeft);

            leftChannel[i] = mid - side;
            rightChannel[i] = mid + side;
        }
    }
}

void MyUtilityAudioProcessor::prepare(double sampleRate, int samplesPerBlock)
{
    //smoothing
    smoothGain.reset(sampleRate, 0.1f); // ramp for gain
    smoothMute.reset(sampleRate, 0.1f); // ramp for mute
    smoothPhase.reset(sampleRate, 0.25f); // ramp for phase
    
    //Current and Target value of smoothGain coming from gain slider
    smoothGain.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));

    //Current and Target value of smoothMute derived from a tenary operator that uses the mute bool value as the condition
    mute = *treeState.getRawParameterValue("mute");
    auto varMute = mute < 0.5 ? 1.0 : 0.0;
    smoothMute.setCurrentAndTargetValue(varMute);
    
    //Current and Target value of smoothPhase derived from a tenary operator that uses the phase bool value as the condition
    phase = *treeState.getRawParameterValue("phase");
    auto varPhase = phase < 0.5 ? 1.0 : -1.0;
    smoothPhase.setCurrentAndTargetValue(varPhase);
    
    //mono bool variable connection to parameter
    mono = *treeState.getRawParameterValue("mono");
    
    // balance float variable connection to parameter
    balance = *treeState.getRawParameterValue("balance");
    
    // width float variable connection to parameter
    width = *treeState.getRawParameterValue("width");
    
    //dsp panner and delay preparation
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    panner.reset();
    panner.prepare(spec);
    panner.setRule(juce::dsp::PannerRule::sin3dB);
    
    delay.prepare (spec);
    linear.prepare (spec);
    linear.reset();
    
    std::fill (lastDelayOutput.begin(), lastDelayOutput.end(), 0.0f);
}

//==============================================================================
bool MyUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyUtilityAudioProcessor::createEditor()
{
    return new MyUtilityAudioProcessorEditor (*this); // uncomment this line to return to my own parameter layout
//    return new juce::GenericAudioProcessorEditor(*this); // commment out this line to return to my own parameter layout...using this as a quick way to figure get a if dsp is working and to not have to worry about the gui (from simpleEQ video 3 - creating paramters).
}

//==============================================================================
void MyUtilityAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream mos(destData, true);
    treeState.state.writeToStream(mos);
}

void MyUtilityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if( tree.isValid() )
    {
        treeState.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyUtilityAudioProcessor();
}
