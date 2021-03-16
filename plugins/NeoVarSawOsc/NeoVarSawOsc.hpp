// NeoVarSawOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace NeoVarSawOsc {

class NeoVarSawOsc : public SCUnit {
public:
    NeoVarSawOsc();

    // Destructor
    ~NeoVarSawOsc();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Freq, PW, Waveshape, NumInputParams };
	float m_freq_past{0.f}; 
	float m_pw_past{0.f}; 
	float m_waveshape_past{0.f}; 
	daisysp::VariableSawOscillator varsawosc;

    enum Outputs { Out1, NumOutputParams };

};

} // namespace NeoVarSawOsc

