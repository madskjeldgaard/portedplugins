// ZOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "ZOsc.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace ZOsc {

ZOsc::ZOsc() {
  const float samplerate = sampleRate();
  zosc.Init(samplerate);
  dcblocker.Init(samplerate);

  mCalcFunc = make_calc_function<ZOsc, &ZOsc::next>();
  next(1);
}

void ZOsc::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedFormantFreq =
      makeSlope(in0(FormantFreq), m_formantfreq_past);
  SlopeSignal<float> slopedShape = makeSlope(in0(Shape), m_shape_past);
  SlopeSignal<float> slopedMode = makeSlope(in0(Mode), m_mode_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    zosc.SetFreq(slopedFreq.consume());
    zosc.SetFormantFreq(slopedFormantFreq.consume());
    zosc.SetMode(slopedMode.consume());
    zosc.SetShape(slopedShape.consume());

    // @TODO dc blocker should be temporary
    outbuf[i] = dcblocker.Process(zosc.Process());
  }

  m_freq_past = slopedFreq.value;
  m_formantfreq_past = slopedFormantFreq.value;
  m_shape_past = slopedShape.value;
  m_mode_past = slopedMode.value;
}

void ZOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace ZOsc

PluginLoad(ZOscUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<ZOsc::ZOsc>(ft, "ZOsc", false);
}
