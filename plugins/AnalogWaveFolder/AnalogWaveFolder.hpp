// AnalogWaveFolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "buchla259waveshaper.hpp"

namespace AnalogWaveFolder {

class AnalogWaveFolder : public SCUnit {
public:
    AnalogWaveFolder();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, F0, Amp, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	buchla259waveshaper::Buchla259WaveShaper waveshaper;

};

} // namespace AnalogWaveFolder

