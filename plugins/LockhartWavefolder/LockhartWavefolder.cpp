// LockhartWavefolder.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "SC_PlugIn.hpp"
#include "LockhartWavefolder.hpp"

static InterfaceTable* ft;

namespace LockhartWavefolder {

LockhartWavefolder::LockhartWavefolder() {
	// const float samplerate = sampleRate();

    mCalcFunc = make_calc_function<LockhartWavefolder, &LockhartWavefolder::next>();
    next(1);
}

LockhartWavefolder::~LockhartWavefolder() {}

void LockhartWavefolder::next(int nSamples) {
    const float* input = in(Input);
	const float gain = in0(Gain);

    float* outbuf = out(Out1);

    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

void LockhartWavefolder::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace LockhartWavefolder

PluginLoad(LockhartWavefolderUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<LockhartWavefolder::LockhartWavefolder>(ft, "LockhartWavefolder", false);
}

