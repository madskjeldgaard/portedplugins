// AnalogBassDrum.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogBassDrum.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogBassDrum {

AnalogBassDrum::AnalogBassDrum() {
  const float samplerate = sampleRate();
  bd.Init(samplerate);

  mCalcFunc = make_calc_function<AnalogBassDrum, &AnalogBassDrum::next>();
  next(1);
}

void AnalogBassDrum::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedAccent = makeSlope(in0(Accent), m_accent_past);

  SlopeSignal<float> slopedTone = makeSlope(in0(Tone), m_tone_past);
  SlopeSignal<float> slopedDecay = makeSlope(in0(Decay), m_decay_past);
  SlopeSignal<float> slopedAttackFM = makeSlope(in0(AttackFM), m_attackfm_past);
  SlopeSignal<float> slopedSelfFM = makeSlope(in0(SelfFM), m_selffm_past);

  /* if (in0(Trig) > 0) { */
  /*   bd.Trig(); */
  /* } */
  bool trig = (in0(Trig) > 0) ? true : false;

  if (in0(Sustain) == 1) {
    bd.SetSustain(true);
  } else {
    bd.SetSustain(false);
  }

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    bd.SetAccent(slopedAccent.consume());
    bd.SetFreq(slopedFreq.consume());
    bd.SetTone(slopedTone.consume());
    bd.SetDecay(slopedDecay.consume());
    bd.SetSelfFmAmount(slopedSelfFM.consume());
    bd.SetAttackFmAmount(slopedAttackFM.consume());
    outbuf[i] = bd.Process(trig);
  }

  m_freq_past = slopedFreq.value;
  m_accent_past = slopedAccent.value;
  m_tone_past = slopedTone.value;
  m_decay_past = slopedDecay.value;
  m_attackfm_past = slopedAttackFM.value;
  m_selffm_past = slopedSelfFM.value;
}

void AnalogBassDrum::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogBassDrum

PluginLoad(AnalogBassDrumUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogBassDrum::AnalogBassDrum>(ft, "AnalogBassDrum", false);
}
