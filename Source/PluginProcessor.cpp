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
    
}

MyUtilityAudioProcessor::~MyUtilityAudioProcessor()
{
    treeState.removeParameterListener("gain", this);
    treeState.removeParameterListener("mute", this);
    treeState.removeParameterListener("phase", this);
    treeState.removeParameterListener("mono", this);
    treeState.removeParameterListener("balance", this);

}

juce::AudioProcessorValueTreeState::ParameterLayout MyUtilityAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //make sure to update number of reservations after adding params
    params.reserve(4);
    
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Gain", -66.0f, 24.0f, 0.0f);
    auto pMute = std::make_unique<juce::AudioParameterBool>("mute", "Mute", 0);
    auto pPhase = std::make_unique<juce::AudioParameterBool>("phase", "Phase", 0);
    auto pMono = std::make_unique<juce::AudioParameterBool>("mono", "Mono", 0);
    auto pBalance = std::make_unique<juce::AudioParameterFloat>("balance", "Balance", -1.0, 1.0, 0);
    
    params.push_back(std::move(pGain));
    params.push_back(std::move(pMute));
    params.push_back(std::move(pPhase));
    params.push_back(std::move(pMono));
    params.push_back(std::move(pBalance));

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
    
    //dsp panner preparation
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    panner.reset();
    panner.prepare(spec);
    panner.setRule(juce::dsp::PannerRule::sin3dB);
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
    
    //mono function call
    monoUpdate(buffer, mono, totalNumInputChannels);
    
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

    DBG(smoothMute.getNextValue());
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

//mono function
void MyUtilityAudioProcessor::monoUpdate(juce::AudioBuffer<float> &buffer, bool mono, int totalNumInputChannels)
{
    // Mono
    // Converts Stereo to Mono. If audio is stereo and mono button is 1, then signal will be mono'd
    // https://forum.juce.com/t/how-do-i-sum-stereo-to-mono/37579/4
    
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

//==============================================================================
bool MyUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyUtilityAudioProcessor::createEditor()
{
//    return new MyUtilityAudioProcessorEditor (*this); // uncomment this line to return to my own parameter layout
    return new juce::GenericAudioProcessorEditor(*this); // commment out this line to return to my own parameter layout...using this as a quick way to figure get a if dsp is working and to not have to worry about the gui (from simpleEQ video 3 - creating paramters).
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
