// AnalogSnareDrum.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace AnalogSnareDrum {

class AnalogSnareDrum : public SCUnit {
public:
    AnalogSnareDrum();

    // Destructor
    ~AnalogSnareDrum();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

	float m_accent_past{0.5f};
	float m_freq_past{100.f};
	float m_tone_past{0.5f};
	float m_decay_past{0.5f};
	float m_snappy_past{0.5f};

    enum InputParams { Trig, InfSustain, Accent, Freq, Tone, Decay, Snappy, NumInputParams };
    enum Outputs { Out1, NumOutputParams };
	daisysp::AnalogSnareDrum sd;

};

} // namespace AnalogSnareDrum

