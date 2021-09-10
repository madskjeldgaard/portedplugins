// VALadder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../surgefilters/LadderFilter.h"

namespace VALadder {

class VALadder : public SCUnit {
public:
    VALadder();

    // Destructor
    ~VALadder();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Freq, Res, Overdrive, Type, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	float m_freq_past, m_res_past, m_overdrive_past;
	LadderFilter filter;
};

} // namespace VALadder
