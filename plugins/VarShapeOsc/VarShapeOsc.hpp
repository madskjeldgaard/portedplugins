// VarShapeOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace VarShapeOsc {

class VarShapeOsc : public SCUnit {
public:
    VarShapeOsc();

    // Destructor
    ~VarShapeOsc();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum Outputs { Out1, NumOutputParams };
    enum InputParams { Freq, PW, Waveshape, Sync, SyncFreq, NumInputParams };
	float m_freq_past{0.f}; 
	float m_pw_past{0.f}; 
	float m_waveshape_past{0.f}; 
	float m_syncfreq_past{0.f};

	daisysp::VariableShapeOscillator varshapeosc;
};

} // namespace VarShapeOsc

