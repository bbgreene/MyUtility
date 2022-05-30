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
    
    gain.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(gain);

    setSize (300, 600);
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
    auto bigDialSize = 150;
    auto middle = getWidth() / 4;
    
    gain.setBounds(middle, getHeight() / 2, bigDialSize, bigDialSize);
}
