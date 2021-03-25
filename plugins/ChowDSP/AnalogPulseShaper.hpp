// AnalogPulseShaper.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "PulseShaper.hpp"
#include "SC_PlugIn.hpp"

namespace AnalogPulseShaper {

class AnalogPulseShaper : public SCUnit {
public:
  AnalogPulseShaper();

  // Destructor
  ~AnalogPulseShaper();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { PulseInput, Width, Decay, Double, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  chow::PulseShaper pulseShaper;
  float doubleTapGain = 0.0f;

  float m_width_past {0.5f};
  float m_decay_past {0.5f};
  float m_double_past {0.5f};

};

} // namespace AnalogPulseShaper
