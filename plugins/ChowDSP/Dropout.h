#ifndef DROPOUT_H_INCLUDED
#define DROPOUT_H_INCLUDED

#include "SmoothedValue.h"

class Dropout
{
public:
    Dropout() {}

    void setMix (float newMix)
    {
        mixSmooth.setTargetValue (newMix);
    }

    void setPower (float newPow)
    {
        powerSmooth.setTargetValue (newPow);
    }

    void prepare (double sr)
    {
        mixSmooth.reset (sr, 0.01);
        mixSmooth.setCurrentAndTargetValue (mixSmooth.getTargetValue());

        powerSmooth.reset (sr, 0.005);
        powerSmooth.setCurrentAndTargetValue (powerSmooth.getTargetValue());
    }

    inline float processSample (float x)
    {
        auto mix = mixSmooth.getNextValue();
        if (mix == 0.0f)
            return x;

        return mix * dropout (x) + (1.0f - mix) * x;
    }

    /** Signum function to determine the sign of the input. */
    template <typename T>
    inline int signum (T val) const
    {
        return (T (0) < val) - (val < T (0));
    }

    inline float dropout (float x)
    {
        auto sign = (float) signum (x);
        return pow (abs (x), powerSmooth.getNextValue()) * sign;
    }

private:
    SmoothedValue<float, ValueSmoothingTypes::Linear> mixSmooth;
    SmoothedValue<float, ValueSmoothingTypes::Linear> powerSmooth;
};

#endif // DROPOUT_H_INCLUDED
