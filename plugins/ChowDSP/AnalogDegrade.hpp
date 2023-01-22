// AnalogDegrade.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../mkutils.hpp"
#include "DegradeFilter.h"
#include "DegradeNoise.h"
#include "LevelDetector.hpp"

namespace AnalogDegrade {

class AnalogDegrade : public SCUnit {
public:
  AnalogDegrade();

  // Destructor
  ~AnalogDegrade();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);
  float uniform();
  void cookParams(float fs, float depthParam, float amtParam, float varParam, float envParam);

  enum InputParams { Input, Depth, Amount, Variance, Envelope, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  float samplerate = 44100;
  DegradeFilter filterProc;
  DegradeNoise noiseProc;
  LevelDetector<float> levelDetector;
  SmoothedValue<float, ValueSmoothingTypes::Multiplicative> gainProc;

};

} // namespace AnalogDegrade
