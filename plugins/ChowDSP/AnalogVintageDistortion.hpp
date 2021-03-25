// AnalogVintageDistortion.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "VariableOversampling.hpp"
#include "shelf_filter.hpp"
#include "ClippingStage.hpp"
#include "BaxandallEQ.cpp"
#include "../dcblocker.h"

namespace AnalogVintageDistortion {

class AnalogVintageDistortion : public SCUnit {
public:
    AnalogVintageDistortion();

    // Destructor
    ~AnalogVintageDistortion();

	VariableOversampling<> oversample;
private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

	enum class OverSamplingAmounts { None, Double, Four, Eight, Sixteen };
    enum InputParams { Input, DriveGain, Bias, LowGain, HighGain, ShelvingFreq, OverSample, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	float m_shelvingfreq_past {600.f};
	float m_bias_past {0.f};
	float m_lowgain_past {0.5f};
	float m_highgain_past {0.5f};
	float m_drivegain_past {0.5f};

	ShelfFilter shelfFilter;
	ClippingStage clipper;
	dcblocker::Dcblocker dcblocker;

};

} // namespace AnalogVintageDistortion

