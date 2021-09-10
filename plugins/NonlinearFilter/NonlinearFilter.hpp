// NonlinearFilter.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "FBFilter.h"

namespace NonlinearFilter {

class NonlinearFilter : public SCUnit {
public:
    NonlinearFilter();

    // Destructor
    ~NonlinearFilter();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Freq, Q, Gain, Shape, Saturation, NumInputParams };
    enum Outputs { Out1, NumOutputParams };
	FBFilter filter;

	float m_freq_past, m_q_past, m_gain_past;
};

} // namespace NonlinearFilter
