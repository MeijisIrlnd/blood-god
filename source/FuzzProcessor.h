//
// Created by Syl on 18/03/2023.
//

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <SDSP/Macros.h>
#include <SDSP/Filters/DSPBiquad.h>
#include <SDSP/Filters/SmoothedFilterCoefficients.h>
namespace Fuzz {

    class FuzzProcessor {
    public:
        FuzzProcessor();
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
        void getNextAudioBlock(juce::AudioBuffer<float>& buffer);
        void releaseResources();

        SDSP_INLINE void setBias(float newBiasDB) {
            m_bias = newBiasDB;
            m_forwardBias.setGainDecibels(newBiasDB);
            m_backwardBias.setGainDecibels(-newBiasDB);
        }
        SDSP_INLINE void setK(float newK) noexcept {
            m_k = newK;
            m_smoothedK.setTargetValue(m_k);
        }

        SDSP_INLINE void setL(float newL) noexcept {
            m_l = newL;
        }

        SDSP_INLINE void setA(float newA) noexcept {
            m_a = newA;
        }

        SDSP_INLINE void setSmoothingCutoff1(float newSmoothing) noexcept {
            m_smoothingCutoff1 = newSmoothing;
        }

        SDSP_INLINE void setSmoothingCutoff2(float newSmoothing) noexcept {
            m_smoothingCutoff2 = newSmoothing;
        }

        SDSP_INLINE void setPreHPCutoff1(float newCutoff) noexcept {
            m_preHighpassCutoff1 = newCutoff;
        }

        SDSP_INLINE void setPreHPCutoff2(float newCutoff) noexcept {
            m_preHighpassCutoff2 = newCutoff;
        }
    private:
        float processSoftClip(float x) const noexcept;
        float processFuzz(float x) const noexcept;


        float m_l{ 1.0f }, m_a{ 0.0f };
        void interpolateCoeffs();
        double m_sampleRate{ 44100.0 };
        float m_bias{ 1.0f };
        float m_k{ 1.0f }, m_preHighpassCutoff1{ 250.0f }, m_preHighpassCutoff2{ 250.0f }, m_smoothingCutoff1{ 10000.0f }, m_smoothingCutoff2{ 10000.0f };
        juce::dsp::Gain<float> m_forwardBias, m_backwardBias;
        juce::SmoothedValue<float> m_smoothedK;
        std::array<SDSP::BiquadCascade<1>, 2> m_smoothingLowpasses1, m_smoothingLowpasses2;

        SDSP::SmoothedFilterCoefficients<1> m_smoothingLowpassCoeffs1, m_smoothingLowpassCoeffs2;

        std::array<SDSP::BiquadCascade<1>, 2> m_preHighpasses1, m_preHighpasses2;
        SDSP::SmoothedFilterCoefficients<1> m_preHighpassCoeffs1, m_preHighpassCoeffs2;

        const int m_updateRate = 100;
        int m_samplesUntilUpdate{ 0 };
    };

} // Fuzz

