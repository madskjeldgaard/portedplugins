// AnalogPhaser.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogPhaser.hpp"

#include "SC_PlugIn.hpp"
#include "chowphaserfb.hpp"

static InterfaceTable *ft;

namespace AnalogPhaser {

AnalogPhaser::AnalogPhaser() {
  const float samplerate = sampleRate();
  chowphasefb.init(samplerate);

  if (inRate(LfoInput) == calc_FullRate) {
    mCalcFunc = make_calc_function<AnalogPhaser, &AnalogPhaser::next_a>();
    next_a(1);
  } else {
    mCalcFunc = make_calc_function<AnalogPhaser, &AnalogPhaser::next_k>();
    next_k(1);
  }
}

// Audio rate lfo input
void AnalogPhaser::next_a(int nSamples) {
  const float *input = in(Input);
  const float *lfoinput = in(LfoInput);

  SlopeSignal<float> slopedSkew = makeSlope(in0(Skew), m_skew_past);
  SlopeSignal<float> slopedFeedback = makeSlope(in0(Feedback), m_feedback_past);
  SlopeSignal<float> slopedModulation =
      makeSlope(in0(Modulation), m_modulation_past);
  SlopeSignal<float> slopedStages = makeSlope(in0(Stages), m_stages_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float stages = slopedStages.consume();
    chowphasefb.setStages(stages);
    const float modulation = slopedModulation.consume();
    chowphasefb.setModulation(modulation);
    const float feedback = slopedFeedback.consume();
    chowphasefb.setFeedback(feedback);
    const float skew = slopedSkew.consume();
    chowphasefb.setSkew(skew);

    outbuf[i] = chowphasefb.process(input[i], lfoinput[i]);
  }

  m_skew_past = slopedSkew.value;
  m_feedback_past = slopedFeedback.value;
  m_modulation_past = slopedModulation.value;
  m_stages_past = slopedStages.value;
}

// Control rate lfo input
void AnalogPhaser::next_k(int nSamples) {
  const float *input = in(Input);

  SlopeSignal<float> slopedInput = makeSlope(in0(LfoInput), m_input_past);

  SlopeSignal<float> slopedSkew = makeSlope(in0(Skew), m_skew_past);
  SlopeSignal<float> slopedFeedback = makeSlope(in0(Feedback), m_feedback_past);
  SlopeSignal<float> slopedModulation =
      makeSlope(in0(Modulation), m_modulation_past);
  SlopeSignal<float> slopedStages = makeSlope(in0(Stages), m_stages_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float stages = slopedStages.consume();
    chowphasefb.setStages(stages);
    const float modulation = slopedModulation.consume();
    chowphasefb.setModulation(modulation);
    const float feedback = slopedFeedback.consume();
    chowphasefb.setFeedback(feedback);
    const float skew = slopedSkew.consume();
    chowphasefb.setSkew(skew);
    const float lfoinput = slopedInput.consume();

    outbuf[i] = chowphasefb.process(input[i], lfoinput);
  }

  m_input_past = slopedInput.value;
  m_skew_past = slopedSkew.value;
  m_feedback_past = slopedFeedback.value;
  m_modulation_past = slopedModulation.value;
  m_stages_past = slopedStages.value;
}

void AnalogPhaser::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace AnalogPhaser

PluginLoad(AnalogPhaserUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogPhaser::AnalogPhaser>(ft, "AnalogPhaser", false);
}
