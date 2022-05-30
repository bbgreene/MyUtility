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
    
    gainLabel.attachToComponent(&gain, false);
    delayLabel.attachToComponent(&delay, false);
    balanceLabel.attachToComponent(&balance, false);
    widthLabel.attachToComponent(&width, false);
    phaseLabel.attachToComponent(&phase, false);

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
    auto topMargin = getHeight() / 6;
    auto middle = getWidth() / 6;
    auto bigDialSize = 200;
    auto smallDialSize = 125;
    auto leftMargin = getWidth() / 12;
    auto smallGap = getHeight() / 40;
    auto bigGap = getHeight() / 17.14;
    auto phaseGap = getWidth() / 43;
    
    auto buttonHeightGap = smallDialSize / 17.86;
    auto buttonHeight = smallDialSize / 3 - 7;
    auto buttonWidth = smallDialSize / 2 - buttonHeightGap;
    auto buttonLeftGap = (smallDialSize - buttonWidth) / 2;
    
    auto phaseSize = smallDialSize * 0.4;
    auto phaseXPos = (getWidth() / 2) - (phaseSize / 2);
    auto phaseYPos = getHeight() / 20;

    
    
    gain.setBounds(middle, topMargin, bigDialSize, bigDialSize);
    phase.setBounds(phaseXPos, phaseYPos, phaseSize, phaseSize);
    delay.setBounds(leftMargin, gain.getBottom() + smallGap, smallDialSize, smallDialSize);
    
    mute.setBounds(delay.getRight() + buttonLeftGap, gain.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    mono.setBounds(delay.getRight() + buttonLeftGap, mute.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    bypass.setBounds(delay.getRight() + buttonLeftGap, mono.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    
    balance.setBounds(leftMargin, delay.getBottom() + bigGap, smallDialSize, smallDialSize);
    width.setBounds(balance.getRight(), delay.getBottom() + bigGap, smallDialSize, smallDialSize);
    
}
