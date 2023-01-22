// AnalogLoss.cpp

#include "AnalogLoss.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace {
constexpr float speedBase = 25.0f / 4.0f;
constexpr float speedMult = 28.0f / 3.0f;
constexpr float speedOff = -25.0f / 3.0f;
inline float getSpeed(float param) {
  return std::pow(speedBase, param) * speedMult + speedOff;
}

constexpr float spaceBase = 10000.0f / 9801.0f;
constexpr float spaceMult = 9801.0f / 10.0f;
constexpr float spaceOff = -980.0f;
inline float getSpacing(float param) {
  return std::pow(spaceBase, param) * spaceMult + spaceOff;
}

constexpr float thickBase = 122500.0f / 22201.0f;
constexpr float thickMult = 22201.0f / 2010.0f;
constexpr float thickOff = -2200.0f / 201.0f;
inline float getThickness(float param) {
  return std::pow(thickBase, param) * thickMult + thickOff;
}

constexpr float gapBase = 1600.0f / 81.0f;
constexpr float gapMult = 81.0f / 31.0f;
constexpr float gapOff = -50.0f / 31.0f;
inline float getGap(float param) {
  return std::pow(gapBase, param) * gapMult + gapOff;
}
}  // namespace

namespace AnalogLoss {

AnalogLoss::AnalogLoss() {
  fs = sampleRate();

  fsFactor = (float)fs / 44100.0f;
  curOrder = int(order * fsFactor);
  currentCoefs.resize(curOrder, 0.0f);
  Hcoefs.resize(curOrder, 0.0f);
  calcCoefs(prevGap, prevThickness, prevSpacing, prevSpeed);

  filter.reset(new FIRFilter(curOrder));
  filter->setCoefs(currentCoefs.data());
  filter->reset();

  mCalcFunc = make_calc_function<AnalogLoss, &AnalogLoss::next>();
  next(1);
}

AnalogLoss::~AnalogLoss() {}

void AnalogLoss::calcCoefs(float speedArg, float thicknessArg, float gapArg,
                           float spacingArg) {
  const auto speed = getSpeed(speedArg);
  const auto thickness = getThickness(thicknessArg);
  const auto gap = getGap(gapArg);
  const auto spacing = getSpacing(spacingArg);

  // Set freq domain multipliers
  binWidth = fs / (float)curOrder;
  auto H = Hcoefs.data();
  for (int k = 0; k < curOrder / 2; k++) {
    const auto freq = (float)k * binWidth;
    const auto waveNumber =
        2.0f * M_PI * std::max(freq, 20.0f) / (speed * 0.0254f);
    const auto thickTimesK = waveNumber * (thickness * (float)1.0e-6);
    const auto kGapOverTwo = waveNumber * (gap * (float)1.0e-6) / 2.0f;

    H[k] = expf(-waveNumber * (spacing * (float)1.0e-6));  // Spacing loss
    H[k] *= (1.0f - expf(-thickTimesK)) / thickTimesK;     // Thickness loss
    H[k] *= sinf(kGapOverTwo) / kGapOverTwo;               // Gap loss
    H[curOrder - k - 1] = H[k];
  }

  // Create time domain filter signal
  auto h = currentCoefs.data();
  for (int n = 0; n < curOrder / 2; n++) {
    const auto idx = (size_t)curOrder / 2 + (size_t)n;
    for (int k = 0; k < curOrder; k++)
      h[idx] +=
          Hcoefs[k] * cosf(2.0f * M_PI * (float)k * (float)n / (float)curOrder);

    h[idx] /= (float)curOrder;
    h[curOrder / 2 - n] = h[idx];
  }

  // compute head bump filters
  calcHeadBumpFilter(speed, gap * 1.0e-6f, fs);
}

void AnalogLoss::calcHeadBumpFilter(float speedIps, float gapMeters, float fs) {
  auto bumpFreq = speedIps * 0.0254f / (gapMeters * 500.0f);
  auto gain =
      std::max(1.5f * (1000.0f - std::abs(bumpFreq - 100.0f)) / 1000.0f, 1.0f);
  headBumpFilter.setParameters(BiquadFilter::PEAK, bumpFreq / fs, 2.0f, gain);
}

void AnalogLoss::next(int nSamples) {
  const float gap = in0(Gap);
  const float thick = in0(Thick);
  const float space = in0(Space);
  const float speed = in0(Speed);
  const float *input = in(Input);
  float *outbuf = out(Out1);

  if (speed == prevSpeed && space == prevSpacing && thick == prevThickness &&
      gap == prevGap) {
  } else {
    calcCoefs(gap, thick, space, speed);
    filter->setCoefs(currentCoefs.data());
    prevGap = gap;
    prevThickness = thick;
    prevSpacing = space;
    prevSpeed = speed;
  }

  // set hysteresis params
  for (int i = 0; i < nSamples; ++i) {
    // get input
    float x = mkutils::constrain(input[i], -1.0f, 1.0f);
    x = filter->process(x);
    x = headBumpFilter.process(x);
    outbuf[i] = x;
  }
}

void AnalogLoss::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace AnalogLoss

PluginLoad(AnalogLossUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogLoss::AnalogLoss>(ft, "AnalogLoss", false);
}
