// VASEM12.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "../surgefilters/SEMFilter12.h"
#include "SC_PlugIn.hpp"

namespace VASEM12 {

class VASEM12 : public SCUnit {
public:
  VASEM12();

  // Destructor
  ~VASEM12();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { Input, Freq, Res, Transition, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  SEMFilter12 filter;

  float m_freq_past, m_res_past, m_transition_past;
};

} // namespace VASEM12
