//
// Created by Syl on 23/03/2023.
//
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace BloodGod {

class LF : public juce::LookAndFeel_V4 {
    public:
        LF(const void* knobImage, int knobImageSize);
        ~LF() override = default;
        void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
    private:
        juce::Image m_knobImage;
    };

} // BloodGod

