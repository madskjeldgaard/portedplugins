// AnalogPulseShaper.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogPulseShaper.hpp"

#include "PulseShaper.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogPulseShaper {

AnalogPulseShaper::AnalogPulseShaper() {
  const float samplerate = sampleRate();
  pulseShaper.init(samplerate);

  mCalcFunc = make_calc_function<AnalogPulseShaper, &AnalogPulseShaper::next>();
  next(1);
}

AnalogPulseShaper::~AnalogPulseShaper() {}

void AnalogPulseShaper::next(int nSamples) {
  const float *pulse = in(PulseInput);

  SlopeSignal<float> slopedWidth = makeSlope(in0(Width), m_width_past);
  SlopeSignal<float> slopedDecay = makeSlope(in0(Decay), m_decay_past);
  SlopeSignal<float> slopedDouble = makeSlope(in0(Double), m_double_past);

  // Pulsewidth ms
  // pulseWidthSamples = int ((pulseWidthMs / 1000.0f) * args.sampleRate);
  /* float triggered = trigger.process(inputs[TRIG_IN].getVoltage()); */
  /* sampleCount = triggered ? pulseWidthSamples : sampleCount; */
  /* float pulse = sampleCount ? 1.0f : 0.0f; */
  /*         sampleCount = sampleCount > 0 ? sampleCount - 1 : 0; */

  float *outbuf = out(Out1);

  constexpr float widthMult = 1.0f;
  constexpr float capVal = (float)0.015e-6;
  constexpr float widthBase = 300.0f;
  constexpr float decayBase = 30.0f;
  constexpr float decayMult = 10.0f;

  for (int i = 0; i < nSamples; ++i) {
    const float decayParam = slopedDecay.consume();
    const float width = slopedWidth.consume();
    const float doubleParam = slopedDouble.consume();

    doubleTapGain = -2.0f * doubleParam;

    float decayTimeMs = std::pow(decayBase, decayParam) * decayMult;
    float r162 = (decayTimeMs / 1000.0f) / capVal;
    float r163 = r162 * 200.0f;
    pulseShaper.setResistors(r162, r163);

    float env = pulseShaper.process(pulse[i]);
    /* env = env > 0.0f ? env : env * doubleTapGain; */

    outbuf[i] = env;
  }

  m_width_past = slopedWidth.value;
  m_decay_past = slopedDecay.value;
  m_double_past = slopedDouble.value;
}

void AnalogPulseShaper::clear(int nSamples) {
  ClearUnitOutputs(this, nSamples);
}

}  // namespace AnalogPulseShaper

PluginLoad(AnalogPulseShaperUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogPulseShaper::AnalogPulseShaper>(ft, "AnalogPulseShaper",
                                                     false);
}
