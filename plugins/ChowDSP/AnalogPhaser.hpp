// AnalogPhaser.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "chowphaserfb.hpp"

namespace AnalogPhaser {

class AnalogPhaser : public SCUnit {
public:
    AnalogPhaser();
private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, LfoInput, Skew, Feedback, Modulation, Stages, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	chow::ChowPhaserFeedback chowphasefb;

	float m_skew_past{0.f};
	float m_feedback_past{0.f};
	float m_modulation_past{0.f};
	float m_stages_past{0.f};

};

} // namespace AnalogPhaser

