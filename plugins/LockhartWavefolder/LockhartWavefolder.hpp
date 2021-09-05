// LockhartWavefolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"

namespace LockhartWavefolder {

class LockhartWavefolder : public SCUnit {
public:
    LockhartWavefolder();

    // Destructor
    ~LockhartWavefolder();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, Gain, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

};

} // namespace LockhartWavefolder

