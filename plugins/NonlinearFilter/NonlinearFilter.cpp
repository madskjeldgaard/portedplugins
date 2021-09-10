// NonlinearFilter.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "NonlinearFilter.hpp"
#include "SC_PlugIn.hpp"

InterfaceTable *ft;

namespace NonlinearFilter {

NonlinearFilter::NonlinearFilter() : filter(mWorld, sampleRate()) {
  m_freq_past = in0(Freq);
  m_q_past = in0(Q);
  m_gain_past = in0(Gain);

  filter.alloc();
  filter.flush();
  filter.reset(sampleRate());

  /* bell, notch, highShelf, lowShelf, highPass, lowPass */
  filter.setEqShape(static_cast<EqShape>(in0(Shape)));

  /* none, hard, soft, hyptan, ahypsin */
  filter.setSaturator(static_cast<SatType>(in0(Saturation)));

  mCalcFunc = make_calc_function<NonlinearFilter, &NonlinearFilter::next>();
  next(1);
}

NonlinearFilter::~NonlinearFilter() { filter.free(); }

void NonlinearFilter::next(int nSamples) {
  const float *input = in(Input);
  SlopeSignal<float> slopedGain = makeSlope(in0(Gain), m_gain_past);

  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);

  SlopeSignal<float> slopedQ = makeSlope(in0(Q), m_q_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {

    const float gain = slopedGain.consume();
    const float freq = slopedFreq.consume();

    // Q range is 0.1 to 18.0
    const float q = sc_clip(slopedQ.consume() * 18.0, 0.1, 18.0);

    filter.setFrequency(freq);
    filter.setQ(q);
    filter.setGain(gain);
    filter.calcCoefs(freq, q, gain);
    outbuf[i] = filter.process(input[i]);
  }

  m_freq_past = slopedFreq.value;
  m_gain_past = slopedGain.value;
  m_q_past = slopedQ.value;
}

void NonlinearFilter::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace NonlinearFilter

PluginLoad(NonlinearFilterUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<NonlinearFilter::NonlinearFilter>(ft, "NonlinearFilter", false);
}
