// ZOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace ZOsc {

class ZOsc : public SCUnit {
public:
  ZOsc();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { Freq, FormantFreq, Shape, Mode, NumInputParams };
  enum Outputs { Out1, NumOutputParams };
  float m_freq_past{0.f};
  float m_formantfreq_past{0.f};
  float m_shape_past{0.f};
  float m_mode_past{0.f};

  daisysp::ZOscillator zosc;
};

} // namespace ZOsc
