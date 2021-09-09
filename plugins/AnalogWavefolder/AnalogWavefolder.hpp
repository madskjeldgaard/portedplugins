// AnalogWavefolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"

namespace AnalogWavefolder {

class AnalogWavefolder : public SCUnit {
public:
    AnalogWavefolder();

    // Destructor
    ~AnalogWavefolder();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Gain, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

};

} // namespace AnalogWavefolder

