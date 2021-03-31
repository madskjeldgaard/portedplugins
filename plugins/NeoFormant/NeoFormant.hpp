// NeoFormant.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "daisysp.h"

namespace NeoFormant {

class NeoFormant : public SCUnit {
public:
    NeoFormant();

    // Destructor
    ~NeoFormant();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { FormantFreq, CarrierFreq, PhaseShift, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	float m_formantfreq_past {100.f};
	float m_phaseshift_past {100.f};
	float m_carrierfreq_past {0.f};

	daisysp::FormantOscillator formantosc;
	daisysp::DcBlock dcblocker;

};

} // namespace NeoFormant

