/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p, APVTS& tree)
    : AudioProcessorEditor (&p), audioProcessor (p), m_lf(BinaryData::Knob_png, BinaryData::Knob_pngSize), m_pots(tree),
      m_pedal(tree)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setLookAndFeel(&m_lf);
    setSize (300, 600);
    addAndMakeVisible(&m_pots);
    addAndMakeVisible(&m_pedal);
    m_backgroundImage = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);
    m_layout.instantiateSlider(this, m_biasSlider, tree, "Bias");
    m_layout.instantiateSlider(this, m_preCutoffSlider1, tree, "PreHPCutoff1");
    m_layout.instantiateSlider(this, m_preCutoffSlider2, tree, "PreHPCutoff2");
    m_layout.instantiateSlider(this, m_lSlider, tree, "L");
    m_layout.instantiateSlider(this, m_kSlider, tree, "K");
    m_layout.instantiateSlider(this, m_aSlider, tree, "A");
    m_layout.instantiateSlider(this, m_smoothingSlider1, tree, "Smoothing1");
    m_layout.instantiateSlider(this, m_smoothingSlider2, tree, "Smoothing2");
    resized();
}

PluginEditor::~PluginEditor() {
    setLookAndFeel(nullptr);
}
//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    auto currentImage = m_backgroundImage.rescaled(getWidth(), getHeight(), juce::Graphics::highResamplingQuality);
    g.drawImage(currentImage, getLocalBounds().toFloat());
}

void PluginEditor::resized()
{
    m_pedal.setBounds(getLocalBounds());
    m_pots.setBounds(0, getHeight() / 5, getWidth(), getHeight() - getHeight() / 5);
    if(m_layout.uiElements.empty()) return;
    auto h = getHeight() / static_cast<int>(m_layout.uiElements.size());
    for(size_t i = 0; i < m_layout.uiElements.size(); ++i) {
        //m_layout.uiElements[i]->label.setBounds(0, h * static_cast<int>(i), getWidth() / 6, h);
        //m_layout.uiElements[i]->slider.setBounds(getWidth() / 6, h * static_cast<int>(i), getWidth() - getWidth() / 6, h);
    }
}
