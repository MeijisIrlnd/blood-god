//
// Created by Syl on 23/03/2023.
//

#include "LF.h"

namespace BloodGod {
    LF::LF(const void* knobImage, int knobImageSize) {
        m_knobImage = juce::ImageCache::getFromMemory(knobImage, knobImageSize);
    }

    void LF::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
    {
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        juce::AffineTransform rotation = juce::AffineTransform::rotation(toAngle, static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f);
        auto sliderImageScaled = m_knobImage.rescaled(width, height, juce::Graphics::ResamplingQuality::highResamplingQuality);
        g.drawImageTransformed(sliderImageScaled, rotation);
    }
} // BloodGod