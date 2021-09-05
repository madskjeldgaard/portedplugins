// AnalogWavefolder.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "SC_PlugIn.hpp"
#include "AnalogWavefolder.hpp"

static InterfaceTable* ft;

namespace AnalogWavefolder {

AnalogWavefolder::AnalogWavefolder() {
	// const float samplerate = sampleRate();

    mCalcFunc = make_calc_function<AnalogWavefolder, &AnalogWavefolder::next>();
    next(1);
}

AnalogWavefolder::~AnalogWavefolder() {}

void AnalogWavefolder::next(int nSamples) {
    const float* input = in(Input);
	const float gain = in0(Gain);

    float* outbuf = out(Out1);

    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

void AnalogWavefolder::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogWavefolder

PluginLoad(AnalogWavefolderUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<AnalogWavefolder::AnalogWavefolder>(ft, "AnalogWavefolder", false);
}

