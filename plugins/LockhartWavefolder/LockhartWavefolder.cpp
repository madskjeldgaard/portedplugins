// LockhartWavefolder.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "LockhartWavefolder.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace LockhartWavefolder {

LockhartWavefolder::LockhartWavefolder() {
  // const float samplerate = sampleRate();

  mCalcFunc =
      make_calc_function<LockhartWavefolder, &LockhartWavefolder::next>();
  m_gain_past = in0(Gain);
  next(1);
}

LockhartWavefolder::~LockhartWavefolder() {}

void LockhartWavefolder::next(int nSamples) {
  const float *input = in(Input);
  SlopeSignal<float> slopedGain = makeSlope(in0(Gain), m_gain_past);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float gain = slopedGain.consume();
    auto in = input[i] * gain;

    in *= 0.33; // @TODO is this needed?
    in = cell1.process(in);
    in = cell2.process(in);
    in = cell3.process(in);
    in = cell4.process(in);
    in *= 3.0; // @TODO is this needed?
    in *= 0.2; // @TODO is this needed?
    in = std::tanh(in);
    outbuf[i] = in;
  }

  m_gain_past = slopedGain.value;
}

void LockhartWavefolder::clear(int nSamples) {
  ClearUnitOutputs(this, nSamples);
}

} // namespace LockhartWavefolder

PluginLoad(LockhartWavefolderUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<LockhartWavefolder::LockhartWavefolder>(ft, "LockhartWavefolder",
                                                       false);
}
