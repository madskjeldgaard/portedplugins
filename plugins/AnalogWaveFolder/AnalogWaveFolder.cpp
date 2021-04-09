// AnalogWaveFolder.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "SC_PlugIn.hpp"
#include "AnalogWaveFolder.hpp"

static InterfaceTable* ft;

namespace AnalogWaveFolder {

AnalogWaveFolder::AnalogWaveFolder() {
	// const float samplerate = sampleRate();

    mCalcFunc = make_calc_function<AnalogWaveFolder, &AnalogWaveFolder::next>();
    next(1);
}

AnalogWaveFolder::~AnalogWaveFolder() {}

void AnalogWaveFolder::next(int nSamples) {
    const float* input = in(Input);
	const float gain = in0(Gain);

    float* outbuf = out(Out1);

    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

void AnalogWaveFolder::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogWaveFolder

PluginLoad(AnalogWaveFolderUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<AnalogWaveFolder::AnalogWaveFolder>(ft, "AnalogWaveFolder", false);
}

