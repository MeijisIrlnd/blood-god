/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <SUX/Utils/LayoutQuickstart.h>

//==============================================================================
/**
*/
class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor&, APVTS&);
    ~PluginEditor() override = default;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& audioProcessor;
    SUX::Quickstart::LayoutQuickstart m_layout;
    SUX::Quickstart::QuickSlider m_biasSlider, m_preCutoffSlider1, m_preCutoffSlider2, m_lSlider, m_aSlider, m_kSlider, m_smoothingSlider1, m_smoothingSlider2;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
