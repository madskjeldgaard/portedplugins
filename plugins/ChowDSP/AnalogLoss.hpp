// AnalogLoss.hpp


#include "HysteresisProcessing.cpp"
#include "SC_PlugIn.hpp"
#include "VariableOversampling.hpp"
#include "../mkutils.hpp"
#include "../dcblocker.h"
#include "FIRFilter.h"
#include "iir.hpp"
#include "wdf.h"

namespace AnalogLoss {

class AnalogLoss : public SCUnit {
public:
  AnalogLoss();

  // Destructor
  ~AnalogLoss();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);
  void calcHeadBumpFilter(float speedIps, float gapMeters, float fs);
  void calcCoefs(float speed, float thickness, float gap, float spacing);

  enum InputParams { Input, Gap, Thick, Space, Speed, NumInputParams };
  enum Outputs { Out1, NumOutputParams };
  HysteresisProcessing hysteresis;

  float prevSpeed = 0.5f;
  float prevSpacing = 0.5f;
  float prevThickness = 0.5f;
  float prevGap = 0.5f;

  float fs = 44100.0f;
  float fsFactor = 1.0f;
  float binWidth = fs / 100.0f;

  const int order = 64;
  int curOrder = order;
  std::vector<float> currentCoefs;
  std::vector<float> Hcoefs;

  std::unique_ptr<FIRFilter> filter;
  BiquadFilter headBumpFilter;

};

} // namespace AnalogLoss
