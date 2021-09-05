// LockhartWavefolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "LockhartCell.hpp"
#include "SC_PlugIn.hpp"

namespace LockhartWavefolder {

class LockhartWavefolder : public SCUnit {
public:
  LockhartWavefolder();

  // Destructor
  ~LockhartWavefolder();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { Input, Gain, NumInputParams };
  enum Outputs { Out1, NumOutputParams };
  float m_gain_past;
  LockhartCell cell1, cell2, cell3, cell4;
};

} // namespace LockhartWavefolder
