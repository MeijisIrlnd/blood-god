//
// Created by Syl on 24/03/2023.
//

#include "Pots.h"

namespace BloodGod::UI {

    Pots::Pots(APVTS& tree)
    {
        setInterceptsMouseClicks(false, true);
        m_distortionPot.setup(this, tree, "Distortion");
        m_tonePot.setup(this, tree, "Tone");
        m_volumePot.setup(this, tree, "Volume");
    }

    void Pots::paint(juce::Graphics& /*g*/) {

    }


    void Pots::resized() {
        auto area = getLocalBounds().withX(10).withWidth(getWidth() - 20);
        auto sliderRect = area.withWidth(static_cast<int>(area.getWidth() / 3.5)).withHeight(static_cast<int>(area.getWidth() / 3.5));
        auto reducedSliderRect = sliderRect.reduced(10, 10);
        m_distortionPot.slider.setBounds(sliderRect.withY(static_cast<int>(sliderRect.getWidth() / 1.5)));
        m_tonePot.slider.setBounds(sliderRect.withX(area.getWidth() - sliderRect.getWidth() + 10).withY(static_cast<int>(sliderRect.getWidth() / 1.5)));
        m_volumePot.slider.setBounds(reducedSliderRect.withX((area.getX() + area.getWidth() / 2) - (reducedSliderRect.getWidth() / 2)));
    }
} // UI