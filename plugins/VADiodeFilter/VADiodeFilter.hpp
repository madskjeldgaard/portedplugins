// VADiodeFilter.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../surgefilters/DiodeFilter.h"

namespace VADiodeFilter {

class VADiodeFilter : public SCUnit {
public:
    VADiodeFilter() ;

    // Destructor
    ~VADiodeFilter();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Freq, Res, Overdrive, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	float m_freq_past, m_res_past, m_overdrive_past ;

	DiodeFilter filter;

};

} // namespace VADiodeFilter
