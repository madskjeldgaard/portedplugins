// HarmonicOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace HarmonicOsc {

class HarmonicOsc : public SCUnit {
public:
    HarmonicOsc();

    // Destructor
    ~HarmonicOsc();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Freq, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	static const int numHarmonics_{16};
	float m_freq_past{0.f};
	daisysp::HarmonicOscillator<numHarmonics_> harmosc;

};

} // namespace HarmonicOsc

