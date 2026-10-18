/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout
GuitarTocarAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Атрибуты для дБ-параметров (Bass, Middle, Treble, Presence)
    auto dbAttributes = juce::AudioParameterFloatAttributes()
        .withLabel("dB")
        .withStringFromValueFunction([](float value, int)
            {
                return juce::String(value, 1) + " dB";
            })
        .withValueFromStringFunction([](const juce::String& text)
            {
                return text.getFloatValue();
            });

    // Атрибуты для процентов (Master, Reverb, Volume, Gain)
    auto percentAttributes = juce::AudioParameterFloatAttributes()
        .withLabel("%")
        .withStringFromValueFunction([](float value, int)
            {
                return juce::String(juce::roundToInt(value * 100.0f)) + " %";
            })
        .withValueFromStringFunction([](const juce::String& text)
            {
                return text.getFloatValue() / 100.0f;
            });

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "gain", "Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f,
        percentAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "bass", "Bass",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f,
        dbAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "middle", "Middle",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f,
        dbAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "treble", "Treble",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f,
        dbAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "presence", "Presence",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f,
        dbAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "master", "Master",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f,
        percentAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "reverb", "Reverb",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f,
        percentAttributes));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "volume", "Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f,
        percentAttributes));

    return layout;
}

GuitarTocarAudioProcessor::GuitarTocarAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
       ,apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    

}

GuitarTocarAudioProcessor::~GuitarTocarAudioProcessor()
{
}

//==============================================================================
const juce::String GuitarTocarAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GuitarTocarAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GuitarTocarAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GuitarTocarAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GuitarTocarAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GuitarTocarAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GuitarTocarAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GuitarTocarAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GuitarTocarAudioProcessor::getProgramName (int index)
{
    return {};
}

void GuitarTocarAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GuitarTocarAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //  ПОДГОТОВКА DSP
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    bassFilter.prepare(spec);
    middleFilter.prepare(spec);
    trebleFilter.prepare(spec);
    presenceFilter.prepare(spec);

    bassFilter.reset();
    middleFilter.reset();
    trebleFilter.reset();
    presenceFilter.reset();

    //  НАСТРАИВАЕМ ФИЛЬТРЫ С РЕАЛЬНОЙ ЧАСТОТОЙ
    *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 100.0f, 0.7f, 1.0f);

    *middleFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 1000.0f, 0.7f, 1.0f);

    *trebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 8000.0f, 0.7f, 1.0f);

    *presenceFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 10000.0f, 0.7f, 1.0f);
}

void GuitarTocarAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GuitarTocarAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GuitarTocarAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // ПОЛУЧАЕМ ЗНАЧЕНИЯ 6 ПАРАМЕТРОВ
    float gain = *apvts.getRawParameterValue("gain");
    float bass = *apvts.getRawParameterValue("bass");
    float middle = *apvts.getRawParameterValue("middle");
    float treble = *apvts.getRawParameterValue("treble");
    float presence = *apvts.getRawParameterValue("presence");
    float master = *apvts.getRawParameterValue("master");
    float reverb = *apvts.getRawParameterValue("reverb");
    float volume = *apvts.getRawParameterValue("volume");

    //ОБНОВЛЯЕМ КОЭФФИЦИЕНТЫ ФИЛЬТРОВ ПО КРУТИЛКАМ
    *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        getSampleRate(), 100.0f, 0.7f, juce::Decibels::decibelsToGain(bass));

    *middleFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        getSampleRate(), 1000.0f, 0.7f, juce::Decibels::decibelsToGain(middle));

    *trebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        getSampleRate(), 8000.0f, 0.7f, juce::Decibels::decibelsToGain(treble));

    *presenceFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        getSampleRate(), 10000.0f, 0.7f, juce::Decibels::decibelsToGain(presence));

    // ПРИМЕНЯЕМ ФИЛЬТРЫ К БУФЕРУ
    juce::dsp::AudioBlock<float> block(buffer);
    bassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    middleFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    trebleFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    presenceFilter.process(juce::dsp::ProcessContextReplacing<float>(block));

    //  ОБРАБОТКА ЗВУКА
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float s = channelData[sample];

            // Gain — усиление
            float driven = s * (1.0f + gain * 20.0f);  // усиливаем
            driven = std::tanh(driven);                 // мягкий клиппинг (как лампа)
            s = driven;


            // Master — общая громкость
            s *= master;
            // Reverb — пока простое усиление (позже заменим на настоящий реверб)
            s *= (0.7f + reverb * 0.6f);

            // Volume — доп. громкость
            s *= (0.5f + volume * 1.0f);

            // Ограничение от перегруза
            s = juce::jlimit(-1.0f, 1.0f, s);

            channelData[sample] = s;
        }
    }

}

//==============================================================================
bool GuitarTocarAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GuitarTocarAudioProcessor::createEditor()
{
    return new GuitarTocarAudioProcessorEditor (*this);
}

//==============================================================================
void GuitarTocarAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GuitarTocarAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarTocarAudioProcessor();
}
