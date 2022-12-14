// AnalogSnareDrum.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogSnareDrum.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogSnareDrum {

AnalogSnareDrum::AnalogSnareDrum() {
  const float samplerate = sampleRate();
  sd.Init(samplerate);

  mCalcFunc = make_calc_function<AnalogSnareDrum, &AnalogSnareDrum::next>();
  next(1);
}

AnalogSnareDrum::~AnalogSnareDrum() {}

void AnalogSnareDrum::next(int nSamples) {
  SlopeSignal<float> slopedAccent = makeSlope(in0(Accent), m_accent_past);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedTone = makeSlope(in0(Tone), m_tone_past);
  SlopeSignal<float> slopedDecay = makeSlope(in0(Decay), m_decay_past);
  SlopeSignal<float> slopedSnappy = makeSlope(in0(Snappy), m_snappy_past);

  bool trig = (in0(Trig) > 0) ? true : false;
  bool infsus = (in0(InfSustain) == 1) ? true : false;
  sd.SetSustain(infsus);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    sd.SetAccent(slopedAccent.consume());
    sd.SetFreq(slopedFreq.consume());
    sd.SetTone(slopedTone.consume());
    sd.SetDecay(slopedDecay.consume());
    sd.SetSnappy(slopedSnappy.consume());

    outbuf[i] = sd.Process(trig);
  }

  m_accent_past = slopedAccent.value;
  m_freq_past = slopedFreq.value;
  m_tone_past = slopedTone.value;
  m_decay_past = slopedDecay.value;
  m_snappy_past = slopedSnappy.value;
}

void AnalogSnareDrum::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace AnalogSnareDrum

PluginLoad(AnalogSnareDrumUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogSnareDrum::AnalogSnareDrum>(ft, "AnalogSnareDrum", false);
}
