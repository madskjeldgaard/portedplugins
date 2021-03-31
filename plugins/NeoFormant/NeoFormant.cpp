// NeoFormant.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "NeoFormant.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace NeoFormant {

NeoFormant::NeoFormant() {
  const float samplerate = sampleRate();
  formantosc.Init(samplerate);
  dcblocker.Init(samplerate);

  mCalcFunc = make_calc_function<NeoFormant, &NeoFormant::next>();
  next(1);
}

NeoFormant::~NeoFormant() {}

void NeoFormant::next(int nSamples) {
  float *outbuf = out(Out1);
  SlopeSignal<float> slopedFormantFreq =
      makeSlope(in0(FormantFreq), m_formantfreq_past);

  SlopeSignal<float> slopedCarrierFreq =
      makeSlope(in0(CarrierFreq), m_carrierfreq_past);

  SlopeSignal<float> slopedPhaseShift =
      makeSlope(in0(PhaseShift), m_phaseshift_past);

  for (int i = 0; i < nSamples; ++i) {

    const float carrierfreq = slopedCarrierFreq.consume();
    const float formantfreq = slopedFormantFreq.consume();
    const float phaseshift = slopedPhaseShift.consume();
    formantosc.SetCarrierFreq(carrierfreq);
    formantosc.SetFormantFreq(formantfreq);
    formantosc.SetPhaseShift(phaseshift);
    outbuf[i] = dcblocker.Process(formantosc.Process());
  }

  m_formantfreq_past = slopedFormantFreq.value;
  m_phaseshift_past = slopedPhaseShift.value;
  m_carrierfreq_past = slopedCarrierFreq.value;
}

void NeoFormant::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace NeoFormant

PluginLoad(NeoFormantUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<NeoFormant::NeoFormant>(ft, "NeoFormant", false);
}
