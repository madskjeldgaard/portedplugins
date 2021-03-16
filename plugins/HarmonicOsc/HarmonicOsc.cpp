// HarmonicOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "HarmonicOsc.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace HarmonicOsc {

HarmonicOsc::HarmonicOsc() {
  const float samplerate = sampleRate();
  harmosc.Init(samplerate);

  for (int i = 0; i < numHarmonics_; i++) {
    harmosc.SetSingleAmp(0.25, i);
  };

  mCalcFunc = make_calc_function<HarmonicOsc, &HarmonicOsc::next>();
  next(1);
}

HarmonicOsc::~HarmonicOsc() {}

void HarmonicOsc::next(int nSamples) {
  float *outbuf = out(Out1);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);

  for (int i = 0; i < nSamples; ++i) {
    harmosc.SetFreq(slopedFreq.consume());
    outbuf[i] = harmosc.Process();
  }
  m_freq_past = slopedFreq.value;
}

void HarmonicOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace HarmonicOsc

PluginLoad(HarmonicOscUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<HarmonicOsc::HarmonicOsc>(ft, "HarmonicOsc", false);
}
