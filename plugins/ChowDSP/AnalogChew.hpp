// AnalogChew.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../mkutils.hpp"
#include "DegradeFilter.h"
#include "Dropout.h"

namespace AnalogChew {

class AnalogChew : public SCUnit {
public:
  AnalogChew();

  // Destructor
  ~AnalogChew();

  inline int getDryTime(float freq, float var)
  {
      auto tScale = std::pow(freq, 0.1f);
      auto varScale = std::pow(((float) rand() / (RAND_MAX))* 2.0f, var);

      auto minVal = (int) ((1.0f - tScale) * samplerate * varScale);
      auto maxVal = (int) ((2.0f - 1.99f * tScale) * samplerate * varScale);
      return (int) ((int) rand()) % (maxVal - minVal) + minVal;
  }

  inline int getWetTime(float freq, float depth, float var)
  {
      auto tScale = std::pow(freq, 0.1f);
      auto start = 0.2f + 0.8f * depth;
      auto end = start - (0.001f + 0.01f * depth);
      auto varScale = std::pow(((float) rand() / (RAND_MAX))* 2.0f, var);

      auto minVal = (int) ((1.0f - tScale) * samplerate * varScale);
      auto maxVal = (int) (((1.0f - tScale) + start - end * tScale) * samplerate * varScale);
      return (int) ((int) rand())  % (maxVal - minVal) + minVal;
  }
private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);
  float uniform();
  void cookParams(float fs, float depthParam, float freqParam, float varParam);

  enum InputParams { Input, Depth, Frequency, Variance,  NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  float samplerate = 44100.0f;
  float mix = 0.0f;
  float power = 0.0f;
  int samplesUntilChange = 1000;
  bool isCrinkled = false;
  int sampleCounter = 0;
  int nextCounter = 19;

  Dropout dropout;
  DegradeFilter filt;

};

} // namespace AnalogChew
