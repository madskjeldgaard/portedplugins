// VA1Pole.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../odinfilters/VAOnePoleFilter.h"

namespace VA1Pole {

class VA1Pole : public SCUnit {
public:
    VA1Pole();

    // Destructor
    /* ~VA1Pole(); */

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);
	float m_feedback_past, m_freq_past;

    enum InputParams { Input, Freq, Feedback, Type, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	VAOnePoleFilter filter;

};

} // namespace VA1Pole
