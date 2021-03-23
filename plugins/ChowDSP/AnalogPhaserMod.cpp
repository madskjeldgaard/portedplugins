// AnalogPhaserMod.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogPhaserMod.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogPhaserMod {

AnalogPhaserMod::AnalogPhaserMod() {
  const float samplerate = sampleRate();
  phasermod.init(samplerate);

  if (inRate(Input) == calc_FullRate) {
    mCalcFunc = make_calc_function<AnalogPhaserMod, &AnalogPhaserMod::next_a>();
    next_a(1);
  } else {
    mCalcFunc = make_calc_function<AnalogPhaserMod, &AnalogPhaserMod::next_k>();
    next_k(1);
  }
}

// Control rate input
void AnalogPhaserMod::next_k(int nSamples) {
  float *outbuf = out(Out1);

  SlopeSignal<float> slopedInput = makeSlope(in0(Input), m_input_past);

  SlopeSignal<float> slopedSkew = makeSlope(in0(Skew), m_skew_past);

  SlopeSignal<float> slopedModulation =
      makeSlope(in0(Modulation), m_modulation_past);

  SlopeSignal<float> slopedStages = makeSlope(in0(Stages), m_stages_past);

  for (int i = 0; i < nSamples; ++i) {
    const float input = slopedInput.consume();
    const float skew = slopedSkew.consume();
    const float stages = slopedStages.consume();
    const float modulation = slopedModulation.consume();
    outbuf[i] = phasermod.process(input);
  }

  m_skew_past = slopedSkew.value;
  m_modulation_past = slopedModulation.value;
  m_stages_past = slopedStages.value;
  m_input_past = slopedInput.value;
}

// Audio rate input
void AnalogPhaserMod::next_a(int nSamples) {
  const float *input = in(Input);
  float *outbuf = out(Out1);

  SlopeSignal<float> slopedSkew = makeSlope(in0(Skew), m_skew_past);

  SlopeSignal<float> slopedModulation =
      makeSlope(in0(Modulation), m_modulation_past);

  SlopeSignal<float> slopedStages = makeSlope(in0(Stages), m_stages_past);

  for (int i = 0; i < nSamples; ++i) {
    const float skew = slopedSkew.consume();
    const float stages = slopedStages.consume();
    const float modulation = slopedModulation.consume();
    outbuf[i] = phasermod.process(input[i]);
  }

  m_skew_past = slopedSkew.value;
  m_modulation_past = slopedModulation.value;
  m_stages_past = slopedStages.value;
}

void AnalogPhaserMod::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogPhaserMod

PluginLoad(AnalogPhaserModUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogPhaserMod::AnalogPhaserMod>(ft, "AnalogPhaserMod", false);
}
