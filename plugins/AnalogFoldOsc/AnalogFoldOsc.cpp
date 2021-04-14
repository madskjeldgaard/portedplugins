// AnalogFoldOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "SC_PlugIn.hpp"
#include "AnalogFoldOsc.hpp"

static InterfaceTable* ft;

namespace AnalogFoldOsc {

AnalogFoldOsc::AnalogFoldOsc() {
  const float samplerate = sampleRate();
  waveshaper.init(samplerate);

  mCalcFunc = make_calc_function<AnalogFoldOsc, &AnalogFoldOsc::next>();
  next(1);
}

void AnalogFoldOsc::next(int nSamples) {
    const float amp = in0(Amp);
    const float freq = in0(F0);
    waveshaper.setAmplitude(amp);
    waveshaper.setF0(freq);

    float* outbuf = out(Out1);

    for (int i = 0; i < nSamples; ++i) {
      outbuf[i] = waveshaper.process();
    }
}

void AnalogFoldOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogFoldOsc

PluginLoad(AnalogFoldOscUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<AnalogFoldOsc::AnalogFoldOsc>(ft, "AnalogFoldOsc", false);
}

