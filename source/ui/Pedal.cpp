//
// Created by Syl on 24/03/2023.
//

#include "Pedal.h"
#include <BinaryData.h>
namespace BloodGod::UI {
    Pedal::Pedal(APVTS &tree) : m_clickRegion(tree) {
        setInterceptsMouseClicks(false, true);
        m_ledImageOff = juce::ImageCache::getFromMemory(BinaryData::LED_OFF_png, BinaryData::LED_OFF_pngSize);
        m_ledImageOn = juce::ImageCache::getFromMemory(BinaryData::LED_ON_png, BinaryData::LED_ON_pngSize);
        m_stateParamAttachment = std::make_unique<juce::ParameterAttachment>(*tree.getParameter("Bypass"), [this](float x) { stateChangeCallback(static_cast<bool>(x)); }, nullptr);
        m_stateParamAttachment->sendInitialUpdate();
        addAndMakeVisible(&m_clickRegion);
    }

    void Pedal::paint(juce::Graphics &g) {
        auto& image = m_internalState ? m_ledImageOff : m_ledImageOn;
        image = image.rescaled(getWidth(), getHeight(), juce::Graphics::highResamplingQuality);
        g.drawImage(image, getLocalBounds().toFloat());
    }

    void Pedal::resized() {
        m_clickRegion.setBounds(0, getHeight() - getHeight() / 4, getWidth(), getHeight() / 4);
    }

    void Pedal::stateChangeCallback(bool newState) {
        m_internalState = newState;
        repaint();
    }
} // UI