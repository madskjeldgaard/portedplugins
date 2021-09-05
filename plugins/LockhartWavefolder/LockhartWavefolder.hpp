// LockhartWavefolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "LockhartCell.hpp"

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

	LockhartCell cell1, cell2, cell3, cell4;

};

} // namespace LockhartWavefolder
