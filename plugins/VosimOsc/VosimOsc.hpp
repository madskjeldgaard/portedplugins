// VosimOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace VosimOsc {

class VosimOsc : public SCUnit {
public:
    VosimOsc();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Freq, Form1, Form2, Shape, NumInputParams };
    enum Outputs { Out1, NumOutputParams };
	daisysp::VosimOscillator vosim;
	daisysp::DcBlock dcblocker;

	float m_freq_past{100.f}, m_form1_past{100.f},m_form2_past{100.f},  m_shape_past{0.f};

};

} // namespace VosimOsc

