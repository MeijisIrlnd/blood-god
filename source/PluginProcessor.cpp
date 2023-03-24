/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
:
#endif
    m_tree(*this, nullptr, { "Params" }, createLayout())
{
    bindListeners();
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginProcessor::getProgramName (int index)
{
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_fuzz.prepareToPlay(samplesPerBlock, sampleRate);
}

void PluginProcessor::releaseResources()
{
    m_fuzz.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    m_fuzz.getNextAudioBlock(buffer);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this, m_tree);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = m_tree.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (m_tree.state.getType()))
            m_tree.replaceState (juce::ValueTree::fromXml (*xmlState));
}

void PluginProcessor::parameterChanged(const juce::String &id, float value) {
    if(id == "Distortion") {

    }
    else if(id == "Tone") {

    }
    else if(id == "Volume") {

    }
    else if(id == "Bypass") {

    }

    if(id == "Bias") {
        m_fuzz.setBias(value);
    }
    else if(id == "PreHPCutoff1") {
        m_fuzz.setPreHPCutoff1(value);
    }
    else if(id == "PreHPCutoff2") {
        m_fuzz.setPreHPCutoff2(value);
    }
    else if(id == "K") {
        m_fuzz.setK(value);
    }
    else if(id == "L") {
        m_fuzz.setL(value);
    }
    else if(id == "A") {
        m_fuzz.setA(value);
    }
    else if(id == "Smoothing1") {
        m_fuzz.setSmoothingCutoff1(value);
    }
    else if(id == "Smoothing2") {
        m_fuzz.setSmoothingCutoff2(value);
    }
}

APVTS::ParameterLayout PluginProcessor::createLayout()
{
    using FloatParam = juce::AudioParameterFloat;
    using StateParam = juce::AudioParameterBool;
    APVTS::ParameterLayout layout;
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"Distortion", 1}, "Distortion", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"Tone", 1}, "Tone", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"Volume", 1}, "Volume", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<StateParam>(juce::ParameterID{"Bypass", 1}, "Bypass", false));

    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"Bias", 1}, "Bias", juce::NormalisableRange<float>(0.0f, 30.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"PreHPCutoff1", 1}, "PreHPCutoff1", juce::NormalisableRange<float>(100.0f, 500.0f, 0.01f), 250.0f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"PreHPCutoff2", 1}, "PreHPCutoff2", juce::NormalisableRange<float>(100.0f, 500.0f, 0.01f), 250.0f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"K", 1}, "K", juce::NormalisableRange<float>(0.01f, 0.5f, 0.01f), 0.5f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"L", 1}, "L", juce::NormalisableRange<float>(0.5f, 1.05, 0.01f), 1.0f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"A", 1}, "A", juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"Smoothing1", 1}, "Smoothing1", juce::NormalisableRange<float>(20.0f, 20000.0f, 0.01f), 10000.0f));
    layout.add(std::make_unique<FloatParam>(juce::ParameterID{"Smoothing2", 1}, "Smoothing2", juce::NormalisableRange<float>(20.0f, 20000.0f, 0.01f), 10000.0f));
    return layout;
}

void PluginProcessor::bindListeners()
{
    m_tree.addParameterListener("Distortion", this);
    m_tree.addParameterListener("Tone", this);
    m_tree.addParameterListener("Volume", this);
    m_tree.addParameterListener("Bypass", this);

    m_tree.addParameterListener("Bias", this);
    m_tree.addParameterListener("PreHPCutoff1", this);
    m_tree.addParameterListener("PreHPCutoff2", this);
    m_tree.addParameterListener("K", this);
    m_tree.addParameterListener("L", this);
    m_tree.addParameterListener("A", this);
    m_tree.addParameterListener("Smoothing1", this);
    m_tree.addParameterListener("Smoothing2", this);
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
