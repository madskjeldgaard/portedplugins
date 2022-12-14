// PhasorModal.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "PhasorModal.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace PhasorModal {

PhasorModal::PhasorModal() {
  const double samplerate = sampleRate();
  mode.init(samplerate);

  mCalcFunc = make_calc_function<PhasorModal, &PhasorModal::next>();
  next(1);
}

/* PhasorModal::~PhasorModal() {} */

void PhasorModal::next(int nSamples) {
  const float *input = in(Input);
  float *outbuf = out(Out1);

  SlopeSignal<float> slopedDamp = makeSlope(in0(Damp), m_damp_past);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedDecay = makeSlope(in0(Decay), m_decay_past);
  SlopeSignal<float> slopedAmp = makeSlope(in0(Amp), m_amp_past);
  SlopeSignal<float> slopedPhase = makeSlope(in0(Phase), m_phase_past);

  for (int i = 0; i < nSamples; ++i) {
    const float damp = slopedDamp.consume();
    /* const float decay = mkutils::constrain(slopedDecay.consume(),
     * 0.0f, 1.5f); */
    const float decay = slopedDecay.consume();

    mode.setDecay(decay, damp);

    const float freq = slopedFreq.consume();
    mode.setFreq(freq);

    const float amp = slopedAmp.consume();
    const float phase = slopedPhase.consume();
    mode.setAmp(amp, phase);

    mode.updateParams();
    // @TODO: DC Filter
    outbuf[i] = mode.process(input[i]);
  }

  m_damp_past = slopedDamp.value;
  m_amp_past = slopedAmp.value;
  m_phase_past = slopedPhase.value;
  m_decay_past = slopedDecay.value;
  m_freq_past = slopedFreq.value;
}

void PhasorModal::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace PhasorModal

PluginLoad(PhasorModalUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<PhasorModal::PhasorModal>(ft, "PhasorModal", false);
}
