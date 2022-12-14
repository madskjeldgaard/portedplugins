// VASEM12.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "VASEM12.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace VASEM12 {

VASEM12::VASEM12() {
  const float samplerate = sampleRate();
  filter.setSampleRate(samplerate);
  filter.reset();
  filter.setup();
  auto initRes = in0(Res);
  filter.setResControl(initRes);
  auto initFreq = in0(Freq);
  filter.setFreq(initFreq);
  auto initTrans = in0(Transition);
  filter.setTransition(initTrans);
  m_transition_past = initTrans;
  m_freq_past = initFreq;
  m_res_past = initRes;

  mCalcFunc = make_calc_function<VASEM12, &VASEM12::next>();
  next(1);
}

VASEM12::~VASEM12() {}

void VASEM12::next(int nSamples) {
  const float *input = in(Input);
  float *outbuf = out(Out1);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedRes = makeSlope(in0(Res), m_res_past);
  SlopeSignal<float> slopedTransition =
      makeSlope(in0(Transition), m_transition_past);

  for (int i = 0; i < nSamples; ++i) {
    const double res = slopedRes.consume();
    const double freq = slopedFreq.consume();
    const double trans = slopedTransition.consume() * (-1.0);
    filter.setTransition(trans);
    filter.setResControl(res);
    filter.setFreq(freq);
    outbuf[i] = filter.doFilter(input[i]);
  }

  m_transition_past = slopedTransition.value;
  m_freq_past = slopedFreq.value;
  m_res_past = slopedRes.value;
}

void VASEM12::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace VASEM12

PluginLoad(VASEM12UGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<VASEM12::VASEM12>(ft, "VASEM12", false);
}
