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
    
    bypassAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "bypass", bypass);
    addAndMakeVisible(bypass);
    
    //dial label attachments
    gainLabel.attachToComponent(&gain, false);
    delayLabel.attachToComponent(&delay, false);
    balanceLabel.attachToComponent(&balance, false);
    widthLabel.attachToComponent(&width, false);
    
    //Titles
    tilTitle.setFont(juce::Font (26.0f, juce::Font::plain));
    tilTitle.setJustificationType(juce::Justification::centredLeft);
    tilTitle.setColour(juce::Label::textColourId, juce::Colours::darkslategrey);
    addAndMakeVisible(tilTitle);

    tilVersion.setFont(juce::Font (15.0f, juce::Font::plain));
    tilVersion.setJustificationType(juce::Justification::centredLeft);
    tilVersion.setColour(juce::Label::textColourId, juce::Colours::mintcream);
    addAndMakeVisible(tilVersion);
    
    olumay.setFont(juce::Font (15.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    addAndMakeVisible(olumay);
    
    //Resizing
    setResizable(true, true);
    setResizeLimits(270, 495, 324, 594); // max limits are min * 1.2
    getConstrainer()->setFixedAspectRatio(0.55);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (270, 495);
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
}

void MyUtilityAudioProcessorEditor::resized()
{

    auto bigDialTopMargin = getHeight() / 6;
    auto bigDialSize = getHeight() / 3.5;
    auto bigDialMiddle = (getWidth() / 2) - (bigDialSize / 2);
    auto smallDialSize = getHeight() / 4.0;
    auto smallGap = getHeight() / 40;
    auto leftMargin = (getWidth() - (smallDialSize * 2)) / 2;
    
    auto buttonHeightGap =  getHeight() * 0.024;
    auto buttonHeight = getHeight() / 20.69;
    auto buttonWidth = getWidth() / 4;
    auto buttonLeftGap = (smallDialSize - buttonWidth) / 2;
    
    auto phaseSize = smallDialSize * 0.35;
    auto phaseXPos = (getWidth() / 2) - (phaseSize / 2);
    auto phaseYPos = phaseSize * 0.55;
    
    gain.setBounds(bigDialMiddle, bigDialTopMargin, bigDialSize, bigDialSize);
    phase.setBounds(phaseXPos, phaseYPos, phaseSize, phaseSize);
    delay.setBounds(leftMargin, gain.getBottom() + smallGap, smallDialSize, smallDialSize);
    
    mute.setBounds(delay.getRight() + buttonLeftGap, gain.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    mono.setBounds(delay.getRight() + buttonLeftGap, mute.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    bypass.setBounds(delay.getRight() + buttonLeftGap, mono.getBottom() + buttonHeightGap, buttonWidth, buttonHeight);
    
    balance.setBounds(leftMargin, delay.getBottom() + smallGap, smallDialSize, smallDialSize);
    width.setBounds(balance.getRight(), delay.getBottom() + smallGap, smallDialSize, smallDialSize);
    
    auto olumayY = getHeight() * 0.9596;
    auto olumayWidth = getWidth() * 0.3;
    auto tilVersionWidth = getWidth() * 0.233;
    auto allTitlesHeight = getHeight() * 0.0404;
    auto titlesTopMargin = getHeight() * 0.0101;
    
    olumay.setBounds(leftMargin, olumayY, olumayWidth, allTitlesHeight);
    tilTitle.setBounds(leftMargin, titlesTopMargin, allTitlesHeight * 2.3, allTitlesHeight);
    tilVersion.setBounds(tilTitle.getRight(), titlesTopMargin, tilVersionWidth, allTitlesHeight);
}
