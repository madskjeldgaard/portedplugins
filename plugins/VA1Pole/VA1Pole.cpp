// VA1Pole.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "VA1Pole.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace VA1Pole {

VA1Pole::VA1Pole() {
  // const float samplerate = sampleRate();
  m_feedback_past = in0(Feedback);
  filter.reset();
  filter.setSampleRate(sampleRate());
  filter.setLP();
  if (static_cast<int>(in0(Type)) == 1) filter.setHP();

  auto initFreq = in0(Freq);
  filter.setFreq(initFreq);
  m_freq_past = initFreq;

  mCalcFunc = make_calc_function<VA1Pole, &VA1Pole::next>();
  next(1);
}

/* VA1Pole::~VA1Pole() {} */

void VA1Pole::next(int nSamples) {
  const float *input = in(Input);

  SlopeSignal<float> slopedFeedback = makeSlope(in0(Feedback), m_feedback_past);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float freq = slopedFreq.consume();
    const double feedback = slopedFeedback.consume();
    filter.setFreq(freq);
    filter.setFeedback(feedback);
    outbuf[i] = filter.doFilter(input[i]);
  }

  m_freq_past = slopedFreq.value;
  m_feedback_past = slopedFeedback.value;
}

void VA1Pole::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace VA1Pole

PluginLoad(VA1PoleUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<VA1Pole::VA1Pole>(ft, "VA1Pole", false);
}
