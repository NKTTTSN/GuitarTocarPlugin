/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
GuitarTocarAudioProcessorEditor::GuitarTocarAudioProcessorEditor(GuitarTocarAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // ЗАГРУЗКА КАРТИНКИ (добавь эту строку ПЕРВОЙ)
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::comb_png, BinaryData::comb_pngSize);

    // НАСТРОЙКА кнопки
    gainButton.setButtonText("");
    gainButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    gainButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    gainButton.setTooltip("Gain");
    gainButton.onClick = [this] { gainKnob.setVisible(!gainKnob.isVisible()); };
    addAndMakeVisible(gainButton);
    //крутилка
    gainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    gainKnob.setNumDecimalPlacesToDisplay(0);
    gainKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    gainKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    gainKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    gainKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    gainKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    gainKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    gainKnob.setTooltip("Gain");
    gainKnob.setVisible(false);
    addAndMakeVisible(gainKnob);

    // ПРИВЯЗКА КРУТИЛКИ К ПАРАМЕТРУ (через APVTS)
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "gain", gainKnob);

    //НАСТРОЙКА КНОПКИ BASS
    bassButton.setButtonText("");
    bassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    bassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    bassButton.setTooltip("Bass");
    bassButton.onClick = [this] { bassKnob.setVisible(!bassKnob.isVisible()); };
    addAndMakeVisible(bassButton);

    //НАСТРОЙКА КРУТИЛКИ bass
    bassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    bassKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    bassKnob.setNumDecimalPlacesToDisplay(1);
    bassKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    bassKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    bassKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    bassKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    bassKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    bassKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    bassKnob.setTooltip("Bass");
    bassKnob.setVisible(false);
    addAndMakeVisible(bassKnob);

    // ПРИВЯЗКА КРУТИЛКИ К ПАРАМЕТРУ (через APVTS)
    bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "bass", bassKnob);

    middleButton.setButtonText("");
    middleButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    middleButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    middleButton.setTooltip("Middle");
    middleButton.onClick = [this] { middleKnob.setVisible(!middleKnob.isVisible()); };
    addAndMakeVisible(middleButton);

    middleKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    middleKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    middleKnob.setNumDecimalPlacesToDisplay(1);
    middleKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    middleKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    middleKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    middleKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    middleKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    middleKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    middleKnob.setTooltip("Middle");
    middleKnob.setVisible(false);
    addAndMakeVisible(middleKnob);

    middleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "middle", middleKnob);


    trebleButton.setButtonText("");
    trebleButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    trebleButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    trebleButton.setTooltip("Treble");
    trebleButton.onClick = [this] { trebleKnob.setVisible(!trebleKnob.isVisible()); };
    addAndMakeVisible(trebleButton);

    trebleKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    trebleKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    trebleKnob.setNumDecimalPlacesToDisplay(1);
    trebleKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    trebleKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    trebleKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    trebleKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    trebleKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    trebleKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    trebleKnob.setTooltip("Treble");
    trebleKnob.setVisible(false);
    addAndMakeVisible(trebleKnob);

    trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "treble", trebleKnob);

    presenceButton.setButtonText("");
    presenceButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presenceButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    presenceButton.setTooltip("presence");
    presenceButton.onClick = [this] { presenceKnob.setVisible(!presenceKnob.isVisible()); };
    addAndMakeVisible(presenceButton);

    presenceKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    presenceKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    presenceKnob.setNumDecimalPlacesToDisplay(1);
    presenceKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    presenceKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    presenceKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    presenceKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    presenceKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    presenceKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    presenceKnob.setTooltip("Presence");
    presenceKnob.setVisible(false);
    addAndMakeVisible(presenceKnob);

    presenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "presence", presenceKnob);

    masterButton.setButtonText("");
    masterButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    masterButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    masterButton.setTooltip("Master");
    masterButton.onClick = [this] { masterKnob.setVisible(!masterKnob.isVisible()); };
    addAndMakeVisible(masterButton);

    masterKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    masterKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    masterKnob.setNumDecimalPlacesToDisplay(0);
    masterKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    masterKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    masterKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    masterKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    masterKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    masterKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    masterKnob.setTooltip("Master");
    masterKnob.setVisible(false);
    addAndMakeVisible(masterKnob);

    masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "master", masterKnob);

    reverbButton.setButtonText("");
    reverbButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    reverbButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    reverbButton.setTooltip("Reverb");
    reverbButton.onClick = [this] { reverbKnob.setVisible(!reverbKnob.isVisible()); };
    addAndMakeVisible(reverbButton);

    reverbKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    reverbKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    reverbKnob.setNumDecimalPlacesToDisplay(0);
    reverbKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    reverbKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    reverbKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    reverbKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    reverbKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    reverbKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    reverbKnob.setTooltip("Reverb");
    reverbKnob.setVisible(false);
    addAndMakeVisible(reverbKnob);

    reverbAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "reverb", reverbKnob);


    volumeButton.setButtonText("");
    volumeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    volumeButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    volumeButton.setTooltip("Volume");
    volumeButton.onClick = [this] { volumeKnob.setVisible(!volumeKnob.isVisible()); };
    addAndMakeVisible(volumeButton);

    volumeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeKnob.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    volumeKnob.setNumDecimalPlacesToDisplay(0);
    volumeKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    volumeKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    volumeKnob.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    volumeKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    volumeKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    volumeKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    volumeKnob.setTooltip("Volume");
    volumeKnob.setVisible(false);
    addAndMakeVisible(volumeKnob);

    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "volume", volumeKnob);


    // СЛУШАЕМ КЛИКИ В ЛЮБОМ МЕСТЕ
    addMouseListener(this, true);

    setSize(900, 686);
}



GuitarTocarAudioProcessorEditor::~GuitarTocarAudioProcessorEditor()
{
}

//==============================================================================
void GuitarTocarAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour (0xffFFEBCD));
    if(backgroundImage.isValid()) 
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat(), juce::RectanglePlacement::fillDestination);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void GuitarTocarAudioProcessorEditor::resized()
{
    gainButton.setBounds(294, 130, 45, 45);
    gainKnob.setBounds(15, 70, 130, 110);

    bassButton.setBounds(339, 130, 45, 45);
    bassKnob.setBounds(15, 70 + 96, 130, 110);

    middleButton.setBounds(339 + 45, 130, 45, 45);
    middleKnob.setBounds(15, 70 + 96 + 96, 130, 110);

    trebleButton.setBounds(339 + 45 + 45, 130, 45, 45);
    trebleKnob.setBounds(15, 160 + 96 + 90, 130, 110);

    presenceButton.setBounds(339 + 45 + 45 + 45, 130, 45, 45);
    presenceKnob.setBounds(15, 160 + 96 + 90 + 90, 130, 110);

    masterButton.setBounds(339 + 45 + 45 + 45 + 45, 130, 45, 45);
    masterKnob.setBounds(15, 160 + 96 + 90 + 90 + 90, 130, 110);

    reverbButton.setBounds(339 + 45 + 45 + 45 + 45 + 45 + 20 - 7, 137 + 6, 22, 22);
    reverbKnob.setBounds(15 + 60 + 440, 3, 130, 110);

    volumeButton.setBounds(339 + 45 + 45 + 45 + 45 + 45 + 90 - 8 + 4, 137 + 6, 22, 22);
    volumeKnob.setBounds(15 + 160 + 440, 3, 130, 110);
}
