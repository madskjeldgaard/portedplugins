// AnalogTape.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

//#include "HysteresisProcessing.hpp"
#include "HysteresisProcessing.cpp"
#include "SC_PlugIn.hpp"
#include "VariableOversampling.hpp"
#include "../mkutils.hpp"
#include "../dcblocker.h"

namespace AnalogTape {

class AnalogTape : public SCUnit {
public:
  AnalogTape();

  // Destructor
  ~AnalogTape();
  VariableOversampling<> oversample;

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum class OverSamplingAmounts { None, Double, Four, Eight, Sixteen };
  enum InputParams { Input, Bias, Saturation, Drive, OverSample, Mode, NumInputParams };
  enum Outputs { Out1, NumOutputParams };
  HysteresisProcessing hysteresis;
  dcblocker::Dcblocker dcfilter;

  float m_drive_past{0.5f};
  float m_saturation_past{0.5f};
  float m_bias_past{0.5f};

};

} // namespace AnalogTape
