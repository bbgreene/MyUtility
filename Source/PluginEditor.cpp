/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyUtilityAudioProcessorEditor::MyUtilityAudioProcessorEditor (MyUtilityAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
//    
//    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
//    gainSlider.setTextValueSuffix(" dB");
//    addAndMakeVisible(gainSlider);
//
//    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", gainSlider);
//
//    muteButton.setButtonText("Mute"); //why do I need this text? Isn't the avpts name not used? check out chucks simple eq for this
//    addAndMakeVisible(muteButton);
//
//    muteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "mute", muteButton);
//
//    phaseButton.setButtonText("Phase"); //why do I need this text? Isn't the avpts name not used? check out chucks simple eq for this
//    addAndMakeVisible(phaseButton);
//
//    phaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "phase", phaseButton);
//
//    setSize (200, 300);
}

MyUtilityAudioProcessorEditor::~MyUtilityAudioProcessorEditor()
{
}

//==============================================================================
void MyUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.fillAll(juce::Colours::black);

}

void MyUtilityAudioProcessorEditor::resized()
{
//    gainSlider.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 50, 200, 100);
//    muteButton.setBounds(getWidth() / 2, 0, 100, 100);
//    phaseButton.setBounds(0, 0, 100, 100);
//
}
