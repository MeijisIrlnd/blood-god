//
// Created by Syl on 18/03/2023.
//

#include "FuzzProcessor.h"

namespace Fuzz {

    FuzzProcessor::FuzzProcessor() {

    }

    void FuzzProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32>(samplesPerBlockExpected), 2};
        m_forwardBias.prepare(spec);
        m_backwardBias.prepare(spec);
        m_forwardBias.setGainDecibels(m_bias);
        m_backwardBias.setGainDecibels(-m_bias);

        SDSP::RBJ::lowpass(m_smoothingLowpassCoeffs1.target(0), sampleRate, static_cast<double>(m_smoothingCutoff1), 0.3);
        SDSP::RBJ::lowpass(m_smoothingLowpassCoeffs2.target(0), sampleRate, static_cast<double>(m_smoothingCutoff2), 0.3);
        std::memcpy(m_smoothingLowpassCoeffs1.current(0), m_smoothingLowpassCoeffs1.target(0), sizeof(double) * 6);
        std::memcpy(m_smoothingLowpassCoeffs2.current(0), m_smoothingLowpassCoeffs2.target(0), sizeof(double) * 6);

        for(size_t i = 0; i < m_smoothingLowpasses1.size(); ++i) {
            m_smoothingLowpasses1[i].setCoefficients(m_smoothingLowpassCoeffs1.target(0));
            m_smoothingLowpasses2[i].setCoefficients(m_smoothingLowpassCoeffs2.target(0));
        }
        SDSP::RBJ::highpass(m_preHighpassCoeffs1.target(0), sampleRate, m_preHighpassCutoff1, 0.5);
        SDSP::RBJ::highpass(m_preHighpassCoeffs2.target(0), sampleRate, m_preHighpassCutoff2, 0.5);
        std::memcpy(m_preHighpassCoeffs1.current(0), m_preHighpassCoeffs1.target(0), sizeof(double) * 6);
        std::memcpy(m_preHighpassCoeffs2.current(0), m_preHighpassCoeffs2.target(0), sizeof(double) * 6);

        for(size_t i = 0; i < m_preHighpasses1.size(); ++i) {
            m_preHighpasses1[i].setCoefficients(m_preHighpassCoeffs1.target(0));
            m_preHighpasses2[i].setCoefficients(m_preHighpassCoeffs2.target(0));
        }

        m_smoothedK.reset(sampleRate, 0.01);
        m_smoothedK.setCurrentAndTargetValue(m_k);
    }

    void FuzzProcessor::getNextAudioBlock(juce::AudioBuffer<float>& buffer) {
        // In general, fuzz applies
        // x = {x, x >= 0
        //     {kx, x < 0
        auto* read = buffer.getArrayOfReadPointers();
        auto* write = buffer.getArrayOfWritePointers();

        for(auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
            if(m_samplesUntilUpdate == 0) {
                SDSP::RBJ::highpass(m_preHighpassCoeffs1.target(0), m_sampleRate, static_cast<double>(m_preHighpassCutoff1), 0.5);
                SDSP::RBJ::highpass(m_preHighpassCoeffs2.target(0), m_sampleRate, static_cast<double>(m_preHighpassCutoff2), 0.5);
                SDSP::RBJ::lowpass(m_smoothingLowpassCoeffs1.target(0), m_sampleRate, static_cast<double>(m_smoothingCutoff1), 0.3);
                SDSP::RBJ::lowpass(m_smoothingLowpassCoeffs2.target(0), m_sampleRate, static_cast<double>(m_smoothingCutoff2), 0.3);
                m_samplesUntilUpdate = m_updateRate;
            }
            --m_samplesUntilUpdate;
            interpolateCoeffs();
            auto currentK = m_smoothedK.getNextValue();
            for(auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
                auto biased = m_forwardBias.processSample(read[channel][sample]);
                float gainPath1 = biased, gainPath2 = biased;
                gainPath1 = m_preHighpasses1[static_cast<size_t>(channel)].processSample(gainPath1);
                gainPath1 = processSoftClip(gainPath1);
                gainPath1 = m_smoothingLowpasses1[static_cast<size_t>(channel)].processSample(gainPath1);
                gainPath2 = m_preHighpasses2[static_cast<size_t>(channel)].processSample(gainPath2);
                gainPath2 = processFuzz(gainPath2);
                gainPath2 = m_smoothingLowpasses2[static_cast<size_t>(channel)].processSample(gainPath2);
                write[channel][sample] = m_backwardBias.processSample(biased * gainPath1 * gainPath2);
            }
        }
                //gainPath1 = std::abs(gainPath1) >= 1e-6f ? std::abs(std::tanh(gainPath1 * (1 / currentK))) / gainPath1 : 1;
    }

    void FuzzProcessor::releaseResources() {

    }


    void FuzzProcessor::interpolateCoeffs() {
        m_preHighpassCoeffs1.interpolate();
        m_preHighpassCoeffs2.interpolate();
        m_smoothingLowpassCoeffs1.interpolate();
        m_smoothingLowpassCoeffs2.interpolate();
        for(size_t i = 0; i < m_preHighpasses1.size(); ++i){
            m_preHighpasses1[i].setCoefficients(m_preHighpassCoeffs1.current(0));
            m_preHighpasses2[i].setCoefficients(m_preHighpassCoeffs2.current(0));
            m_smoothingLowpasses1[i].setCoefficients(m_smoothingLowpassCoeffs1.current(0));
            m_smoothingLowpasses2[i].setCoefficients(m_smoothingLowpassCoeffs2.current(0));
        }
    }

    float FuzzProcessor::processSoftClip(float x) const noexcept {
        // soft clip gain function..
        auto denom = std::sqrt(1 + std::powf((x / m_l) - m_a, 2));
        return 1 / denom;
    }

    float FuzzProcessor::processFuzz(float x) const noexcept {
        return x < 0 ? 1 - m_k - std::powf(m_k, 2) : 1 + m_k - std::powf(m_k, 2);
    }
} // Fuzz