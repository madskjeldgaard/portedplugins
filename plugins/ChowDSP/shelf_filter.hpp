#ifndef SHELFFILTER_H_INCLUDED
#define SHELFFILTER_H_INCLUDED

#include "iir.hpp"

// first order shelving filter
class ShelfFilter : public OnePole {
public:
    ShelfFilter() {}

    void calcCoefs(float lowGain, float highGain, float fc, float fs) {
        // reduce to simple gain element
        if (lowGain == highGain)
        {
            b[0] = lowGain; b[1] = 0.0f;
            a[0] = 0.0f;    a[1] = 0.0f;
            return;
        }

        auto wc = 2.0f * M_PI * fc;
        auto p = std::sqrt (wc*wc * (highGain*highGain - lowGain*highGain) / (lowGain*highGain - lowGain*lowGain));
        auto K = p / std::tan (p / (2.0f * fs));

        auto b0 = highGain / p;
        auto b1 = lowGain;
        auto a0 = 1.0f / p;
        auto a1 = 1.0f;

        auto a0_z = a0*K + a1;

        // bilinear transform
        b[0] = ( b0*K + b1) / a0_z;
        b[1] = (-b0*K + b1) / a0_z;
        a[1] = (-a0*K + a1) / a0_z;
    }
};

#endif // SHELFFILTER_H_INCLUDED
