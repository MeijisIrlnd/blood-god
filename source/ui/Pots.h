//
// Created by Syl on 24/03/2023.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
using APVTS = juce::AudioProcessorValueTreeState;
namespace BloodGod::UI {
    class Pots : public juce::Component {
    public:
        explicit Pots(APVTS& tree);
        ~Pots() override = default;
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        struct BGSlider {
            BGSlider() {
                slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
                slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            }

            void setup(juce::Component* parent, APVTS& tree, const juce::String& paramId) {
                attachment = std::make_unique<juce::SliderParameterAttachment>(*tree.getParameter(paramId), slider, nullptr);
                parent->addAndMakeVisible(&slider);
            }

            juce::Slider slider;
            std::unique_ptr<juce::SliderParameterAttachment> attachment{ nullptr };

        };
        BGSlider m_distortionPot, m_tonePot, m_volumePot;
    };

    } // BloodGod


