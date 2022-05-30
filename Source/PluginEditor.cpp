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
    
    //dial, buttons with their respective attachments
    gainAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", gain);
    gain.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    gain.dialDecimalPoint(gain); // attempt at fixing decimal places
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
    phase.setToggleStyle(bbg_gui::bbg_Toggle::ToggleStyle::kPhaseToggle);
    addAndMakeVisible(phase);

    muteAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "mute", mute);
    addAndMakeVisible(mute);
    
    monoAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "mono", mono);
    addAndMakeVisible(mono);
    
//    bypassAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "bypass", bypass);
    addAndMakeVisible(bypass);
    
    //dial label attachments
    gainLabel.attachToComponent(&gain, false);
    delayLabel.attachToComponent(&delay, false);
    balanceLabel.attachToComponent(&balance, false);
    widthLabel.attachToComponent(&width, false);
    phaseLabel.attachToComponent(&phase, false);
    
//    setResizable(true, true);
//    setResizeLimits(300, 600, 600, 1200);
//    getConstrainer()->setFixedAspectRatio(1.0);

    setSize (300, 600);
}

MyUtilityAudioProcessorEditor::~MyUtilityAudioProcessorEditor()
{
}

//==============================================================================
void MyUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    
//    g.fillAll(juce::Colours::grey);
    juce::Rectangle<int> background = getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::lightblue, getHeight() - getHeight(), juce::Colours::lightpink, getHeight() / 2));
    g.fillRect(background);
    
    //Image title test
    
    auto imageXPos = getWidth() / 12;
    auto imageYPos = getHeight() / 28;
    auto imageWidth = getWidth() / 4;
    auto imageHeight = getWidth() / 7;
    
    titleImage = juce::ImageCache::getFromMemory(BinaryData::TestTil_png, BinaryData::TestTil_pngSize);
    g.drawImageWithin(titleImage, imageXPos, imageYPos, imageWidth, imageHeight, juce::RectanglePlacement::stretchToFit);
}

void MyUtilityAudioProcessorEditor::resized()
{
    auto bigDialTopMargin = getHeight() / 6;
    auto middle = getWidth() / 6;
    auto bigDialSize = getHeight() / 3;
    auto smallDialSize = getHeight() / 4.8;
    auto leftMargin = getWidth() / 12;
    auto smallGap = getHeight() / 40;
    auto bigGap = getHeight() / 17.14;
    
    auto buttonHeightGap =  getHeight() / 55;
    auto buttonHeight = getHeight() / 20.69;
    auto buttonWidth = getWidth() / 4;
    auto buttonLeftGap = (smallDialSize - buttonWidth) / 2;
    
    auto phaseSize = smallDialSize * 0.4;
    auto phaseXPos = (getWidth() / 2) - (phaseSize / 2);
    auto phaseYPos = getHeight() / 20;
    
    gain.setBounds(middle, bigDialTopMargin, bigDialSize, bigDialSize);
    phase.setBounds(phaseXPos, phaseYPos, phaseSize, phaseSize);
    delay.setBounds(leftMargin, gain.getBottom() + smallGap, smallDialSize, smallDialSize);
    
    mute.setBounds(delay.getRight() + buttonLeftGap, gain.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    mono.setBounds(delay.getRight() + buttonLeftGap, mute.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    bypass.setBounds(delay.getRight() + buttonLeftGap, mono.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    
    balance.setBounds(leftMargin, delay.getBottom() + bigGap, smallDialSize, smallDialSize);
    width.setBounds(balance.getRight(), delay.getBottom() + bigGap, smallDialSize, smallDialSize);
    
}
