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
    // set default font
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Avenir Next");
    
    gainAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", gain);
    gain.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(gain);
    
    delayAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "delay", delay);
    delay.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialDotModernStyle);
    addAndMakeVisible(delay);
    
    balanceAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "balance", balance);
    balance.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialDotModernStyle);
    balance.panDialSuffixChange(balance);
    addAndMakeVisible(balance);
    
    widthAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "width", width);
    width.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialDotModernStyle);
    addAndMakeVisible(width);
    
    phaseAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "phase", phase);
    phase.setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, CustomColours::blackGrey);
    phase.setColour(juce::ToggleButton::ColourIds::tickColourId, CustomColours::blue);
    addAndMakeVisible(phase);

    muteAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "mute", mute);
    addAndMakeVisible(mute);
    
    monoAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "mono", mono);
    addAndMakeVisible(mono);
    
//    bypassAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "bypass", bypass);
    addAndMakeVisible(bypass);

    setSize (300, 600);
}

MyUtilityAudioProcessorEditor::~MyUtilityAudioProcessorEditor()
{
}

//==============================================================================
void MyUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.fillAll(juce::Colours::grey);

}

void MyUtilityAudioProcessorEditor::resized()
{
    auto bigDialSize = 200;
    auto smallDialSize = 150;
    auto buttonWidth = smallDialSize / 2;
    auto buttonHeight = smallDialSize / 3;
    auto middle = getWidth() / 6;
    
    gain.setBounds(middle, getHeight() - 500, bigDialSize, bigDialSize);
    delay.setBounds(0, gain.getBottom(), smallDialSize, smallDialSize);
    phase.setBounds(delay.getRight(), gain.getBottom() + buttonHeight, buttonWidth, buttonHeight);
    mute.setBounds(delay.getRight() + buttonWidth, gain.getBottom(), buttonWidth, buttonHeight);
    mono.setBounds(delay.getRight() + buttonWidth, gain.getBottom() + buttonHeight, buttonWidth, buttonHeight);
    bypass.setBounds(delay.getRight() + buttonWidth, gain.getBottom() + buttonHeight * 2, buttonWidth, buttonHeight);
    balance.setBounds(0, delay.getBottom(), smallDialSize, smallDialSize);
    width.setBounds(smallDialSize, delay.getBottom(), smallDialSize, smallDialSize);
    
}
