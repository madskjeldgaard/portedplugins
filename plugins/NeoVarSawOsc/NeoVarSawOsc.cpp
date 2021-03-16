// NeoVarSawOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "NeoVarSawOsc.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace NeoVarSawOsc {

NeoVarSawOsc::NeoVarSawOsc() {
  const float samplerate = sampleRate();
  varsawosc.Init(samplerate);

  mCalcFunc = make_calc_function<NeoVarSawOsc, &NeoVarSawOsc::next>();
  next(1);
}

NeoVarSawOsc::~NeoVarSawOsc() {}

void NeoVarSawOsc::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedPW = makeSlope(in0(PW), m_pw_past);
  SlopeSignal<float> slopedwaveshape =
      makeSlope(in0(Waveshape), m_waveshape_past);
  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    varsawosc.SetFreq(slopedFreq.consume());
    varsawosc.SetPW(slopedPW.consume());
    varsawosc.SetWaveshape(slopedwaveshape.consume());
    outbuf[i] = varsawosc.Process();
  }

  m_freq_past = slopedFreq.value;
  m_pw_past = slopedPW.value;
  m_waveshape_past = slopedwaveshape.value;
}

void NeoVarSawOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace NeoVarSawOsc

PluginLoad(NeoVarSawOscUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<NeoVarSawOsc::NeoVarSawOsc>(ft, "NeoVarSawOsc", false);
}
