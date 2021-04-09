// AnalogWaveFolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"

namespace AnalogWaveFolder {

class AnalogWaveFolder : public SCUnit {
public:
    AnalogWaveFolder();

    // Destructor
    ~AnalogWaveFolder();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Gain, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

};

} // namespace AnalogWaveFolder

