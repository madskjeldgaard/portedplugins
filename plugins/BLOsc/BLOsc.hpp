// BLOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace BLOsc {

class BLOsc : public SCUnit {
public:
    BLOsc();

    // Destructor
    ~BLOsc();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Freq, PulseWidth, WaveForm, NumInputParams };
    enum Outputs { Out1, NumOutputParams };
	float m_freq_past{100.f}, m_pulsewidth_past{0.f};
	daisysp::BlOsc blosc;

};

} // namespace BLOsc

