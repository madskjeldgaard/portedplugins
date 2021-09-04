// AnalogBBDDelay.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "bbd_line.h"

namespace AnalogBBDDelay {

class AnalogBBDDelay : public SCUnit {
public:
  AnalogBBDDelay();

  // Destructor
  ~AnalogBBDDelay();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { Input, DelayTime, NumStages, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  BBD_Line bbd_delay;
  int m_number_of_stages;
  float *clockbuf;
  float m_delaytime_past;
};

} // namespace AnalogBBDDelay
