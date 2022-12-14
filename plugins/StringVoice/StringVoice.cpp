// StringVoice.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "StringVoice.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace StringVoice {

StringVoice::StringVoice() {
  const float samplerate = sampleRate();
  stringvoice.Init(samplerate);

  mCalcFunc = make_calc_function<StringVoice, &StringVoice::next>();
  next(1);
}

StringVoice::~StringVoice() {}

void StringVoice::next(int nSamples) {
  float *outbuf = out(Out1);

  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedAccent = makeSlope(in0(Accent), m_accent_past);
  SlopeSignal<float> slopedStructure =
      makeSlope(in0(Structure), m_structure_past);
  SlopeSignal<float> slopedBrightness =
      makeSlope(in0(Brightness), m_brightness_past);
  SlopeSignal<float> slopedDamping = makeSlope(in0(Damping), m_damping_past);

  bool infsus = (in0(InfSustain) == 1) ? true : false;
  bool trig = (in0(Trig) > 0) ? true : false;

  for (int i = 0; i < nSamples; ++i) {
    const float damping = slopedDamping.consume();
    const float brightness = slopedBrightness.consume();
    const float structure = slopedStructure.consume();
    const float accent = slopedAccent.consume();
    const float freq = slopedFreq.consume();

    stringvoice.SetFreq(freq);
    stringvoice.SetAccent(accent);
    stringvoice.SetBrightness(brightness);
    stringvoice.SetDamping(damping);
    stringvoice.SetStructure(structure);
    outbuf[i] = stringvoice.Process(trig);
  }

  m_damping_past = slopedDamping.value;
  m_brightness_past = slopedBrightness.value;
  m_structure_past = slopedStructure.value;
  m_accent_past = slopedAccent.value;
  m_freq_past = slopedFreq.value;
}

void StringVoice::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace StringVoice

PluginLoad(StringVoiceUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<StringVoice::StringVoice>(ft, "StringVoice", false);
}
