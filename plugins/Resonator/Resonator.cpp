// Resonator.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "Resonator.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace Resonator {

Resonator::Resonator() {
  const float samplerate = sampleRate();
  const float initPos = in0(Position);
  const int initRes = in0(Resolution);
  resonator.Init(initPos, initRes, samplerate);

  mCalcFunc = make_calc_function<Resonator, &Resonator::next>();
  next(1);
}

void Resonator::next(int nSamples) {
  const float *input = in(Input);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);

  SlopeSignal<float> slopedStructure =
      makeSlope(in0(Structure), m_structure_past);
  SlopeSignal<float> slopedBrightness =
      makeSlope(in0(Brightness), m_brightness_past);

  SlopeSignal<float> slopedDamping = makeSlope(in0(Damping), m_damping_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float structure = slopedStructure.consume();
    const float brightness = slopedBrightness.consume();
    const float damping = slopedDamping.consume();
    const float freq = slopedFreq.consume();
    resonator.SetFreq(freq);
    resonator.SetStructure(structure);
    resonator.SetDamping(damping);
    resonator.SetBrightness(brightness);
    outbuf[i] = resonator.Process(input[i]);
  }

  m_freq_past = slopedFreq.value;
  m_structure_past = slopedStructure.value;
  m_brightness_past = slopedBrightness.value;
  m_damping_past = slopedDamping.value;
}

void Resonator::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace Resonator

PluginLoad(ResonatorUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<Resonator::Resonator>(ft, "Resonator", false);
}
