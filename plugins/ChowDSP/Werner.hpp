// Werner.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

/* #include "GenSVF.hpp" */
#include "GenSVF.cpp"
#include "SC_PlugIn.hpp"
#include "VariableOversampling.hpp"
#include "../mkutils.hpp"

namespace Werner {

class Werner : public SCUnit {
public:
  Werner();

  VariableOversampling<> oversample;

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);
  enum class OverSamplingAmounts { None, Double, Four, Eight, Sixteen };

  enum InputParams { Input, Freq, Damp, Feedback, Drive, OverSample, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  GeneralSVF svf;

  float m_freq_past{0.5f};
  float m_damp_past {0.5f};
  float m_feedback_past {0.5f};
  float m_drive_past {0.0f};

};

} // namespace Werner
