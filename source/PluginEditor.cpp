/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p, APVTS& tree)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
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

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::resized()
{
    if(m_layout.uiElements.empty()) return;
    auto h = getHeight() / static_cast<int>(m_layout.uiElements.size());
    for(size_t i = 0; i < m_layout.uiElements.size(); ++i) {
        m_layout.uiElements[i]->label.setBounds(0, h * static_cast<int>(i), getWidth() / 6, h);
        m_layout.uiElements[i]->slider.setBounds(getWidth() / 6, h * static_cast<int>(i), getWidth() - getWidth() / 6, h);
    }
}
