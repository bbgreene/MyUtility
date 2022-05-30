/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Source/bbg_gui/Dial.h"
#include "../Source/bbg_gui/Toggle.h"
#include "../Source/bbg_gui/PushButton.h"
#include "../Source/bbg_gui/Menu.h"
#include "../Source/bbg_gui/Label.h"

//==============================================================================
/**
*/
class MyUtilityAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MyUtilityAudioProcessorEditor (MyUtilityAudioProcessor&);
    ~MyUtilityAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //UI dials and Buttons
    bbg_gui::bbg_Dial gain { " dB", -66.0, 24.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial delay { " Smps", 0.0, 48000.0, 1.0, 0.0, 0.0 };
    
    //Attachments
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachement;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyUtilityAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyUtilityAudioProcessorEditor)
};
