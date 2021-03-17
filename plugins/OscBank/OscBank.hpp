// OscBank.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace OscBank {

class OscBank : public SCUnit {
public:
  OscBank();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  float m_freq_past{0.f}, m_gain_past{0.f}, m_saw8_past{0.f}, m_square8_past{0.f}, m_saw4_past{0.f},
      m_square4_past{0.f}, m_saw2_past{0.f}, m_square2_past{0.f}, m_saw1_past{0.f};

  enum InputParams {
    Freq,
    Gain,
    Saw8,
    Square8,
    Saw4,
    Square4,
    Saw2,
    Square2,
    Saw1,
    NumInputParams
  };

  enum Outputs { Out1, NumOutputParams };

  daisysp::OscillatorBank oscbank;
};

} // namespace OscBank
