// AnalogBassDrum.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace AnalogBassDrum {

class AnalogBassDrum : public SCUnit {
public:
  AnalogBassDrum();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);
  float m_freq_past{100.f};
  float m_accent_past{0.5f};
  float m_tone_past{0.5f};
  float m_decay_past{0.5f};
  float m_attackfm_past{0.5f};
  float m_selffm_past{0.5f};

  enum InputParams {
    Trig,
    Sustain,
    Accent,
    Freq,
    Tone,
    Decay,
    AttackFM,
    SelfFM,
    NumInputParams
  };
  enum Outputs { Out1, NumOutputParams };
  daisysp::AnalogBassDrum bd;
};

} // namespace AnalogBassDrum
