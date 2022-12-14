// AnalogTape.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogTape.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogTape {

AnalogTape::AnalogTape() {
  const float samplerate = sampleRate();
  dcfilter.init();

  hysteresis.reset();

  const auto modeInput = static_cast<SolverType>(in0(Mode));
  hysteresis.setSolver(modeInput);

  oversample.reset(samplerate);
  const auto overSampleInput =
      static_cast<OverSamplingAmounts>(in0(OverSample));

  int overSampleIndex = 0;

  switch (overSampleInput) {
    case OverSamplingAmounts::None:
      overSampleIndex = 0;
      break;
    case OverSamplingAmounts::Double:
      overSampleIndex = 1;
      break;
    case OverSamplingAmounts::Four:
      overSampleIndex = 2;
      break;
    case OverSamplingAmounts::Eight:
      overSampleIndex = 3;
      break;
    case OverSamplingAmounts::Sixteen:
      overSampleIndex = 4;
      break;
    default:
      overSampleIndex = 0;
      break;
  };

  oversample.setOversamplingIndex(overSampleIndex);
  hysteresis.setSampleRate(samplerate * oversample.getOversamplingRatio());

  mCalcFunc = make_calc_function<AnalogTape, &AnalogTape::next>();
  next(1);
}

AnalogTape::~AnalogTape() {}

void AnalogTape::next(int nSamples) {
  SlopeSignal<float> slopedDrive = makeSlope(in0(Drive), m_drive_past);
  SlopeSignal<float> slopedSaturation =
      makeSlope(in0(Saturation), m_saturation_past);
  SlopeSignal<float> slopedBias = makeSlope(in0(Bias), m_bias_past);
  const float *input = in(Input);
  float *outbuf = out(Out1);

  // set hysteresis params
  for (int i = 0; i < nSamples; ++i) {
    const float width = 1.0f - slopedBias.consume();
    const float drive = slopedDrive.consume();
    const float saturation = slopedSaturation.consume();

    const bool v1 = false;
    hysteresis.cook(drive, width, saturation, v1);

    // get input
    float x = mkutils::constrain(input[i], -1.0f, 1.0f);

    // process hysteresis
    oversample.upsample(x);
    float *osBuffer = oversample.getOSBuffer();
    for (int k = 0; k < oversample.getOversamplingRatio(); k++)
      osBuffer[k] = (float)hysteresis.process((double)osBuffer[k]);
    float y = oversample.downsample();

    outbuf[i] = dcfilter.process(y);
  }

  m_drive_past = slopedDrive.value;
  m_saturation_past = slopedSaturation.value;
  m_bias_past = slopedBias.value;
}

void AnalogTape::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace AnalogTape

PluginLoad(AnalogTapeUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogTape::AnalogTape>(ft, "AnalogTape", false);
}
