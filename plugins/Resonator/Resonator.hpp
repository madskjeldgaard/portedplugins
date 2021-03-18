// Resonator.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace Resonator {

class Resonator : public SCUnit {
public:
  Resonator();


private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  daisysp::Resonator resonator;

  enum InputParams {
    Input,
    Freq,
	Position,
	Resolution,
    Structure,
    Brightness,
    Damping,
    NumInputParams
  };

  enum Outputs { Out1, NumOutputParams };

  float m_freq_past{100.f};
  float m_structure_past{0.f};
  float m_brightness_past{0.f};
  float m_damping_past{0.f};
};

} // namespace Resonator
