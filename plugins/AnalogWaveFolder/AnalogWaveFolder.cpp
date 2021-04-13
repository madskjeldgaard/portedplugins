// AnalogWaveFolder.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "SC_PlugIn.hpp"
#include "AnalogWaveFolder.hpp"

static InterfaceTable* ft;

namespace AnalogWaveFolder {

AnalogWaveFolder::AnalogWaveFolder() {
  const float samplerate = sampleRate();
  waveshaper.init(samplerate);

  mCalcFunc = make_calc_function<AnalogWaveFolder, &AnalogWaveFolder::next>();
  next(1);
}

void AnalogWaveFolder::next(int nSamples) {
    const float* input = in(Input);
    const float amp = in0(Amp);
    const float freq = in0(F0);
    waveshaper.setAmplitude(amp);
    waveshaper.setF0(freq);

    float* outbuf = out(Out1);

    for (int i = 0; i < nSamples; ++i) {
      outbuf[i] = waveshaper.process();
    }
}

void AnalogWaveFolder::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogWaveFolder

PluginLoad(AnalogWaveFolderUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<AnalogWaveFolder::AnalogWaveFolder>(ft, "AnalogWaveFolder", false);
}

