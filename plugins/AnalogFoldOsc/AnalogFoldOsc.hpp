// AnalogFoldOsc.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "buchla259waveshaper.hpp"

namespace AnalogFoldOsc {

class AnalogFoldOsc : public SCUnit {
public:
    AnalogFoldOsc();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { F0, Amp, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	buchla259waveshaper::Buchla259WaveShaper waveshaper;

};

} // namespace AnalogFoldOsc

