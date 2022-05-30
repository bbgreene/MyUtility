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
    bbg_gui::bbg_Dial gain { " dB", -66.0, 24.0, 0.01, 0.0, 0.0 };
    bbg_gui::bbg_Dial delay { " Smps", 0.0, 48000.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_Dial balance { " C", -1.0, 1.0, 0.1, 0.0, 0.0 };
    bbg_gui::bbg_Dial width { " W", 0.0, 5.0, 0.1, 1.0, 1.0 };
    bbg_gui::bbg_PushButton mute { "Mute" };
    bbg_gui::bbg_PushButton mono { "Mono" };
    bbg_gui::bbg_PushButton bypass { "Bypass" };
    bbg_gui::bbg_Toggle phase { "Phase" };
    
    //Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> balanceAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachement;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoAttachement;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachement;
    
    //Labels
    bbg_gui::bbg_dialLabel gainLabel { "Gain" };
    bbg_gui::bbg_dialLabel delayLabel { "Delay" };
    bbg_gui::bbg_dialLabel balanceLabel { "Balance" };
    bbg_gui::bbg_dialLabel widthLabel { "Width" };
    bbg_gui::bbg_toggleLabel phaseLabel { "Phase" };
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyUtilityAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyUtilityAudioProcessorEditor)
};
