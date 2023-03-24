//
// Created by Syl on 24/03/2023.
//
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
using APVTS = juce::AudioProcessorValueTreeState;

namespace BloodGod::UI {
    class Pedal : public juce::Component {
    public:
        explicit Pedal(APVTS& tree);
        ~Pedal() override = default;
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        struct ClickRegion : public juce::Button {
            explicit ClickRegion(APVTS& tree) : juce::Button("Bypass") {
                setClickingTogglesState(true);
                m_attachment = std::make_unique<juce::ButtonParameterAttachment>(*tree.getParameter("Bypass"), *dynamic_cast<juce::Button*>(this), nullptr);
            }

            ~ClickRegion() override = default;

            void mouseUp(const juce::MouseEvent& /*ev*/) override {
                triggerClick();
            }

            void paintButton(juce::Graphics&, bool, bool) override { }
            void paint(juce::Graphics& g) override { }
            void resized() override { }
        private:
            std::unique_ptr<juce::ButtonParameterAttachment> m_attachment{ nullptr };
        } m_clickRegion;

        void stateChangeCallback(bool newState);
        bool m_internalState{ false };
        juce::Image m_ledImageOff, m_ledImageOn;
        std::unique_ptr<juce::ParameterAttachment> m_stateParamAttachment{ nullptr };
    };
} // UI


