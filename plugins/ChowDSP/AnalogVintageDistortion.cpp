// AnalogVintageDistortion.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogVintageDistortion.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogVintageDistortion {

AnalogVintageDistortion::AnalogVintageDistortion() {
  // const float samplerate = sampleRate();

  dcblocker.init();

  oversample.reset(sampleRate());
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
  clipper.reset(sampleRate() * oversample.getOversamplingRatio());

  mCalcFunc = make_calc_function<AnalogVintageDistortion,
                                 &AnalogVintageDistortion::next>();
  next(1);
}

AnalogVintageDistortion::~AnalogVintageDistortion() {}

void AnalogVintageDistortion::next(int nSamples) {
  // These were calculated from the min/max settings defined in the ChowDer
  // module.
  //
  // The eq gain settings should never reach 0 since that will result in a
  // division by zero
  const float minEQGain = 0.035481338923358;
  const float maxEQGain = 0.28183829312645;

  const float *input = in(Input);

  SlopeSignal<float> slopedDriveGain =
      makeSlope(in0(DriveGain), m_drivegain_past);
  SlopeSignal<float> slopedBias = makeSlope(in0(Bias), m_bias_past);
  SlopeSignal<float> slopedLowGain =
      makeSlope(sc_max(in0(LowGain), minEQGain), m_lowgain_past);
  SlopeSignal<float> slopedHighGain =
      makeSlope(sc_max(in0(HighGain), minEQGain), m_highgain_past);
  SlopeSignal<float> slopedShelvingFreq =
      makeSlope(in0(ShelvingFreq), m_shelvingfreq_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float shelvingfreq = slopedShelvingFreq.consume();
    const float highgain = slopedHighGain.consume();
    const float lowgain = slopedLowGain.consume();
    const float drivegain = slopedDriveGain.consume();
    const float bias = slopedBias.consume();

    shelfFilter.calcCoefs(lowgain, highgain, shelvingfreq, sampleRate());

    float x = input[i];
    x = ((1.0 + drivegain) * 30.f) * shelfFilter.process(x) + bias;

    oversample.upsample(x);

    float *osBuffer = oversample.getOSBuffer();
    for (int k = 0; k < oversample.getOversamplingRatio(); k++)
      osBuffer[k] = clipper.processSample(osBuffer[k]);
    float y = oversample.downsample();

    outbuf[i] = dcblocker.process(y);
  }

  m_shelvingfreq_past = slopedShelvingFreq.value;
  m_bias_past = slopedBias.value;
  m_lowgain_past = slopedLowGain.value;
  m_highgain_past = slopedHighGain.value;
  m_drivegain_past = slopedDriveGain.value;
}

void AnalogVintageDistortion::clear(int nSamples) {
  ClearUnitOutputs(this, nSamples);
}

} // namespace AnalogVintageDistortion

PluginLoad(AnalogVintageDistortionUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogVintageDistortion::AnalogVintageDistortion>(
      ft, "AnalogVintageDistortion", false);
}
