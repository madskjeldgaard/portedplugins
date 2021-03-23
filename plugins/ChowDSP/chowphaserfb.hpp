#pragma once
#ifndef CHOWPHASEFB
#define CHOWPHASEFB

#include "chowphasermod.hpp"
#include "iir.hpp"
#include "ldr.hpp"

namespace chow {
/*
 * Ported from Jatin Chowdhury's ChowDSP-VCV package:
 * https://github.com/jatinchowdhury18/ChowDSP-VCV
 *
 */

class ChowPhaserFeedback {
public:
  void init(float samplerate) {
    m_samplerate = samplerate;
    lfo.init(samplerate);
  }

  void setSkew(float skew) { 
	  lfo.setSkew(skew);
	  m_skew = skew; 
  }
  void setFeedback(float fb) { m_fb = fb; }

  void setStages(float stages) { lfo.setStages(stages); }

  void setModulation(float mod) { lfo.setModulation(mod); }

  float process(float sample, float lfoInput) {

    // handle LFO
    const auto lfoval = lfo.process(lfoInput);
    const auto rVal = LDR::getLDRResistance(lfoval, m_skew);

    // feedback process
    calcCoefs(rVal, -1.0f * m_fb, m_samplerate);
    auto y = fbFilter.process(sample);
    return std::tanh(y);
  }

private:
  inline void calcCoefs(float R, float fbAmt, float fs) noexcept {
    constexpr float C = (float)15e-9;
    const float RC = R * C;

    // analog coefs
    const float b0s = RC * RC;
    const float b1s = -2.0f * RC;
    const float b2s = 1.0f;
    const float a0s = b0s * (1.0f + fbAmt);
    const float a1s = -b1s * (1.0f - fbAmt);
    const float a2s = 1.0f + fbAmt;

    // frequency warping
    const float wc = calcPoleFreq(a0s, a1s, a2s);
    const auto K = wc == 0.0f ? 2.0f * fs : wc / std::tan(wc / (2.0f * fs));
    const auto KSq = K * K;

    // bilinear transform
    const float a0 = a0s * KSq + a1s * K + a2s;
    a[1] = 2.0f * (a2s - a0s * KSq) / a0;
    a[2] = (a0s * KSq - a1s * K + a2s) / a0;
    b[0] = (b0s * KSq + b1s * K + b2s) / a0;
    b[1] = 2.0f * (b2s - b0s * KSq) / a0;
    b[2] = (b0s * KSq - b1s * K + b2s) / a0;

    fbFilter.setCoefficients(b, a);
  }

  inline float calcPoleFreq(float a, float b, float c) const noexcept {
    auto radicand = b * b - 4.0f * a * c;

    return radicand >= 0.0f ? 0.0f : std::sqrt(-radicand) / (2.0f * a);
  }

  BiquadFilter fbFilter;
  float a[3];
  float b[3];
  float m_samplerate, m_skew, m_fb;

  ChowPhaserMod lfo;
};
} // namespace chow
#endif
