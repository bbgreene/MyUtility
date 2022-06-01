/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MyUtilityAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    MyUtilityAudioProcessor();
    ~MyUtilityAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState treeState;

private:
    
    //Variables
    juce::LinearSmoothedValue<float> smoothGain { 0.0f };
    juce::LinearSmoothedValue<float> smoothPhase { 0.0f };
    juce::LinearSmoothedValue<float> smoothMute { 0.0f };
    bool mono { false };
    bool phase { false };
    bool mute { false };
    bool bypass { false };
    float width;
    
    
    //dsp panner and raw panning variable instantiation
    juce::dsp::Panner<float> panner;
    float balance { 0.0 };
    
    //delay variables and instantiations
    static constexpr auto effectDelaySamples = 192000;
    juce::dsp::DelayLine<float> delay { effectDelaySamples };
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> linear { effectDelaySamples };
    std::array<float, 2> delayValue { {} };
    std::array<float, 2> lastDelayOutput;
    
    //Functions for param layout and changes
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    //Function for delay processing
    void delayProcessing(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels);
    
    //Function for mono processing
    void monoProcessing(juce::AudioBuffer<float>& buffer, bool mono, int totalNumInputChannels);
    
    //Function for m/s width processing
    void widthProcessing(juce::AudioBuffer<float>& buffer);
    
    void prepare(double sampleRate, int samplesPerBlock);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyUtilityAudioProcessor)
};
