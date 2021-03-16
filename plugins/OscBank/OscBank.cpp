// OscBank.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "OscBank.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace OscBank {

OscBank::OscBank() {
  const float samplerate = sampleRate();
  oscbank.Init(samplerate);

  mCalcFunc = make_calc_function<OscBank, &OscBank::next>();
  next(1);
}

/* OscBank::~OscBank() {} */

void OscBank::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedGain = makeSlope(in0(Gain), m_gain_past);
  SlopeSignal<float> slopedSaw8 = makeSlope(in0(Saw8), m_saw8_past);
  SlopeSignal<float> slopedSquare8 = makeSlope(in0(Square8), m_square8_past);
  SlopeSignal<float> slopedSaw4 = makeSlope(in0(Saw4), m_saw4_past);
  SlopeSignal<float> slopedSquare4 = makeSlope(in0(Square4), m_square4_past);
  SlopeSignal<float> slopedSaw2 = makeSlope(in0(Saw2), m_saw2_past);
  SlopeSignal<float> slopedSquare2 = makeSlope(in0(Square2), m_square2_past);
  SlopeSignal<float> slopedSaw1 = makeSlope(in0(Saw1), m_saw1_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    oscbank.SetFreq(slopedFreq.consume());
    oscbank.SetGain(slopedGain.consume());
    oscbank.SetSingleAmp(slopedSaw8.consume(), 0);
    oscbank.SetSingleAmp(slopedSquare8.consume(), 1);
    oscbank.SetSingleAmp(slopedSaw4.consume(), 2);
    oscbank.SetSingleAmp(slopedSquare4.consume(), 3);
    oscbank.SetSingleAmp(slopedSaw2.consume(), 4);
    oscbank.SetSingleAmp(slopedSquare2.consume(), 5);
    oscbank.SetSingleAmp(slopedSaw1.consume(), 6);

    outbuf[i] = oscbank.Process();
  }

  m_freq_past = slopedFreq.value;
  m_gain_past = slopedGain.value;
  m_saw8_past = slopedSaw8.value;
  m_square8_past = slopedSquare8.value;
  m_saw4_past = slopedSaw4.value;
  m_square4_past = slopedSquare4.value;
  m_saw2_past = slopedSaw2.value;
  m_square2_past = slopedSquare2.value;
  m_saw1_past = slopedSaw1.value;
}

void OscBank::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace OscBank

PluginLoad(OscBankUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<OscBank::OscBank>(ft, "OscBank", false);
}
