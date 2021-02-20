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
  void next(int nSamples);
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
	OutNum,
    NumInputParams
  };

  enum Outputs { Out1, NumOutputParams };

  lpg::LowPassGate lowpassgate;
};

} // namespace LPG
