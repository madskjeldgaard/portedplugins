// VAKorg35.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../odinfilters/Korg35Filter.h"

namespace VAKorg35 {

class VAKorg35 : public SCUnit {
public:
    VAKorg35();

    // Destructor
    ~VAKorg35();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Freq, Res, Overdrive, Type, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	float m_freq_past, m_res_past, m_overdrive_past ;
	Korg35Filter filter;
};

} // namespace VAKorg35
