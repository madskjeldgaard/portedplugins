#pragma once
#ifndef CHOW_PHASERMOD
#define CHOW_PHASERMOD

#include "ldr.hpp"
#include <algorithm>
namespace chow{

/*
 * Ported from Jatin Chowdhury's ChowDSP-VCV package:
 * https://github.com/jatinchowdhury18/ChowDSP-VCV
 *
 */

class ChowPhaserMod {
public:
  ChowPhaserMod() {}

  void init(float samplerate){
	  m_samplerate = samplerate;
	  setSkew(0.25f);
	  setModulation(0.25);
	  setStages(8.0f);
	  std::fill(z, &z[maxNumStages], 0.0f);
  }

  void setSkew(float skew) { m_skew = skew; }

  void setModulation(float modulation) { m_modulation = modulation; }

  void setStages(float stages) { m_stages = stages; }

  float process(float input) {
    // handle LFO
    const auto lfo = input;
    const auto rVal = LDR::getLDRResistance(lfo, m_skew);

    // process mod
    const auto mod = m_modulation;
    const auto numStages = m_stages;

    calcCoefs(rVal, m_samplerate);
    const auto x = input;
    float y = x;
    for (int stage = 0; stage < (int)numStages; ++stage)
      y = processStage(y, stage);

    // fractional stage
    float stageFrac = numStages - (int)numStages;
    y = stageFrac * processStage(y, (int)numStages) + (1.0f - stageFrac) * y;

    // mix
    y = mod * y + (1.0f - mod) * x;
    return y;
  }

private:
  inline float processStage(float x, int stage) {
    float y = z[stage] + x * b[0];
    z[stage] = x * b[1] - y * a[1];
    return y;
  }

  inline void calcCoefs(float R, float fs) {
    // component values
    constexpr float C = (float)25e-9;
    const float RC = R * C;

    // analog coefs
    const float b0s = RC;
    const float b1s = -1.0f;
    const float a0s = b0s;
    const float a1s = 1.0f;

    // bilinear transform
    const auto K = 2.0f * fs;
    const auto a0 = a0s * K + a1s;
    b[0] = (b0s * K + b1s) / a0;
    b[1] = (-b0s * K + b1s) / a0;
    a[0] = 1.0f;
    a[1] = (-a0s * K + a1s) / a0;
  }

  enum {
    maxNumStages = 52,
  };

  float m_lfo, m_skew, m_modulation, m_stages, m_samplerate;

  float a[2] = {1.0f, 0.0f};
  float b[2] = {1.0f, 0.0f};
  float z[maxNumStages];
};

}
#endif
