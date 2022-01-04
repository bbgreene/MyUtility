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
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

MyUtilityAudioProcessor::~MyUtilityAudioProcessor()
{
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
//        for loop that utilies gain and phase values. The phase formula came from here: // got this equation from juce tutorial:  https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto myPhase = apvts.getRawParameterValue("phase")->load() < 0.5f ? 1.0f : -1.0f;
            auto myGain = apvts.getRawParameterValue("gain")->load();
            
            channelData [sample] = channelData[sample] * juce::Decibels::decibelsToGain(myGain) * myPhase;
        }
        
        // for loop that flips mute button bool value: if it is set to '1' (or Mute On) then the audio is muted. If it is set to '0' (or Mute off) then the audio passes. There is probably a better way to do this in the toggleButton itself, but can't figure it out at the mo
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto myMute = apvts.getRawParameterValue("mute")->load();
            if (myMute == 1)
                channelData [sample] = channelData[sample] * 0;
            else if (myMute == 0)
                channelData [sample] = channelData[sample] * 1;
        }
    }
}

//==============================================================================
bool MyUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyUtilityAudioProcessor::createEditor()
{
    return new MyUtilityAudioProcessorEditor (*this);
}

//==============================================================================
void MyUtilityAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void MyUtilityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if( tree.isValid() )
    {
        apvts.replaceState(tree);
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyUtilityAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MyUtilityAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //this pushes gain into the vector list above
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange<float> (-66.0f, 35.0f, 0.5f, 1.63f), 0.0f));
    
    //this pushes the mute button into the vector list above
    params.push_back(std::make_unique<juce::AudioParameterBool>("mute", "Mute", 0));
    
    //this pushes the phase button into the vector list above
    params.push_back(std::make_unique<juce::AudioParameterBool>("phase", "Phase", 0));
    
    
    return { params.begin(), params.end() };
}
