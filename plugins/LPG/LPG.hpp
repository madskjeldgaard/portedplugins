// LPG.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "../lpg.cpp"
#include "SC_PlugIn.hpp"

namespace LPG {

class LPG : public SCUnit {
public:
  LPG();

private:
  // Calc function
  
  // Audio rate control signal for the vactrol
  void next_a(int nSamples);

  // Control rate control signal for the vactrol
  void next_k(int nSamples);

  void clear(int nSamples);

  enum InputParams {
    Input,
    ControlInput,
    ControlOffset,
    ControlScale,
    VCAAmount,
    Resonance,
    LowpassMode,
    Linearity,
    NumInputParams
  };

  enum Outputs { Out1, NumOutputParams };

  lpg::LowPassGate lowpassgate;

  float m_control_offset_past{0.f}, m_sloped_control_scale{0.f},
      m_sloped_vca_amount{0.f}, m_sloped_resonance{0.f}, m_control_input_past{0.f};
};

} // namespace LPG
