// VarShapeOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "VarShapeOsc.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace VarShapeOsc {

VarShapeOsc::VarShapeOsc() {
  const float samplerate = sampleRate();
  varshapeosc.Init(samplerate);

  mCalcFunc = make_calc_function<VarShapeOsc, &VarShapeOsc::next>();
  next(1);
}

VarShapeOsc::~VarShapeOsc() {}

void VarShapeOsc::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedPW = makeSlope(in0(PW), m_pw_past);
  SlopeSignal<float> slopedwaveshape =
      makeSlope(in0(Waveshape), m_waveshape_past);
  SlopeSignal<float> slopedSyncFreq = makeSlope(in0(SyncFreq), m_syncfreq_past);

  const bool sync = (in0(Sync) == 1.0f) ? true : false;
  varshapeosc.SetSync(sync);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    varshapeosc.SetFreq(slopedFreq.consume());
    varshapeosc.SetSyncFreq(slopedSyncFreq.consume());
    varshapeosc.SetPW(slopedPW.consume());
    varshapeosc.SetWaveshape(slopedwaveshape.consume());
    outbuf[i] = varshapeosc.Process();
  }

  m_freq_past = slopedFreq.value;
  m_pw_past = slopedPW.value;
  m_waveshape_past = slopedwaveshape.value;
  m_syncfreq_past = slopedSyncFreq.value;
}

void VarShapeOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace VarShapeOsc

PluginLoad(VarShapeOscUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<VarShapeOsc::VarShapeOsc>(ft, "VarShapeOsc", false);
}
