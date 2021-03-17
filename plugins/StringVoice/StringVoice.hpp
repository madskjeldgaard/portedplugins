// StringVoice.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace StringVoice {

class StringVoice : public SCUnit {
public:
    StringVoice();

    // Destructor
    ~StringVoice();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

	float m_damping_past{0.5f};
	float m_brightness_past{0.5f};
	float m_structure_past{0.5f};
	float m_accent_past{0.5f};
	float m_freq_past{100.f};

    enum InputParams { Trig, InfSustain, Freq, Accent, Structure, Brightness, Damping, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	daisysp::StringVoice stringvoice;

};

} // namespace StringVoice

