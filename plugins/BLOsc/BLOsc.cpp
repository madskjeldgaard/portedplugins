// BLOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "BLOsc.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace BLOsc {

BLOsc::BLOsc() {
  const float samplerate = sampleRate();
  blosc.Init(samplerate);
  blosc.SetAmp(0.5f);
  blosc.SetWaveform(static_cast<int>(in0(WaveForm)));

  mCalcFunc = make_calc_function<BLOsc, &BLOsc::next>();
  next(1);
}

BLOsc::~BLOsc() {}

void BLOsc::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedPulseWidth =
      makeSlope(in0(PulseWidth), m_pulsewidth_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    blosc.SetFreq(slopedFreq.consume());
    blosc.SetPw(slopedPulseWidth.consume());
    outbuf[i] = blosc.Process();
  }

  m_freq_past = slopedFreq.value;
  m_pulsewidth_past = slopedPulseWidth.value;
}

void BLOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace BLOsc

PluginLoad(BLOscUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<BLOsc::BLOsc>(ft, "BLOsc", false);
}
