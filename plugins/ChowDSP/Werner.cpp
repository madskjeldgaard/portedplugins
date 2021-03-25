// Werner.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "Werner.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace Werner {

Werner::Werner() {
  const float samplerate = sampleRate();
  svf.reset();

  // Oversampling
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

  mCalcFunc = make_calc_function<Werner, &Werner::next>();
  next(1);
}

void Werner::next(int nSamples) {
  const float *input = in(Input);
  /* Input, Freq, Damp, Feedback, Drive, */

  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedDamp = makeSlope(in0(Damp), m_damp_past);

  SlopeSignal<float> slopedFeedback = makeSlope(in0(Feedback), m_feedback_past);

  SlopeSignal<float> slopedDrive = makeSlope(in0(Drive), m_drive_past);

  // @TODO Un-normalize freq
  const float highFreq = 20000.0f;
  const float lowFreq = 20.0f;

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    float drive = slopedDrive.consume();
    const float feedback = slopedFeedback.consume();
    const float damp = slopedDamp.consume();
    const float freq = slopedFreq.consume();

    float r = damp;
    r = mkutils::constrain(r, 0.25f, 1.25f);
    float k = feedback;
    k = mkutils::constrain(k, 0.0f, 1.0f);
    float freqParam = freq;
    auto freqCalculated = pow(highFreq / lowFreq, freqParam) * lowFreq;

    float wc =
        (freqCalculated / (sampleRate() * oversample.getOversamplingRatio())) *
        M_PI;

    Print("%f %f \n", freq, wc);

    svf.calcCoefs(r, k, wc);

    // @TODO Can this be made more effective
    // @TODO Why this limit?
    const float highDrive = 10.0f;
    const float lowDrive = 0.1f;
    drive = pow(highDrive / lowDrive, pow(drive, 0.33f)) * lowDrive;
    svf.setDrive(drive);

    oversample.upsample(input[i]);
    float *osBuffer = oversample.getOSBuffer();
    for (int k = 0; k < oversample.getOversamplingRatio(); k++)
      osBuffer[k] = svf.process(osBuffer[k]);
    float y = oversample.downsample();

    outbuf[i] = y;
  }

  m_freq_past = slopedFreq.value;
  m_damp_past = slopedDamp.value;
  m_feedback_past = slopedFeedback.value;
  m_drive_past = slopedDrive.value;
}

void Werner::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace Werner

PluginLoad(WernerUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<Werner::Werner>(ft, "Werner", false);
}
