/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GuitarTocarAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GuitarTocarAudioProcessorEditor (GuitarTocarAudioProcessor&);
    ~GuitarTocarAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarTocarAudioProcessor& audioProcessor;
    
    juce::TextButton gainButton;
    juce::Slider gainKnob;
    //  BASS
    juce::TextButton bassButton;
    juce::Slider bassKnob;

    //  MIDDLE
    juce::TextButton middleButton;
    juce::Slider middleKnob;

    //  TREBLE
    juce::TextButton trebleButton;
    juce::Slider trebleKnob;

    //  PRESENCE
    juce::TextButton presenceButton;
    juce::Slider presenceKnob;

    //  MASTER
    juce::TextButton masterButton;
    juce::Slider masterKnob;

    //  REVERB
    juce::TextButton reverbButton;
    juce::Slider reverbKnob;

    //  VOLUME
    juce::TextButton volumeButton;
    juce::Slider volumeKnob;


    //BACKGROUND
    juce::Image backgroundImage;
    juce::TooltipWindow tooltipWindow{ this, 700 };

    // ATTACHMENTS (СВЯЗЬ КРУТИЛОК С APVTS)
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> middleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarTocarAudioProcessorEditor)
};
