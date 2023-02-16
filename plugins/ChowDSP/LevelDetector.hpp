#pragma once

#include <cmath>

/** A simple level detector, that I like better than the JUCE one */
template <typename SampleType>
class LevelDetector
{
public:
    LevelDetector() = default;

    /** Sets the current time constants, in milliseconds. */
    void setParameters (float attackTimeMs, float releaseTimeMs)
    {
        tauAtt = (SampleType) calcTimeConstant (attackTimeMs, expFactor);
        tauRel = (SampleType) calcTimeConstant (releaseTimeMs, expFactor);
    }

    /** Initialises the processor. */
    void prepare (float sampleRate)
    {
        expFactor = -1000.0f / sampleRate;
        reset();
    }

    /** Resets the internal state variables of the processor. */
    void reset()
    {
        yOld = (SampleType) 0;
        increasing = true;
    }

    /** Processes a single sample. Note that this function expects the input to be non-negative */
    virtual inline SampleType processSample (SampleType x) noexcept
    {
        auto tau = increasing ? tauAtt : tauRel;
        x = yOld + tau * (x - yOld);

        // update for next sample
        increasing = x > yOld;
        yOld = x;

        return x;
    }

protected:
    inline float calcTimeConstant (float timeMs, float expFactor)
    {
        return timeMs < 1.0e-3f ? 0.0f : 1.0f - std::exp (expFactor / timeMs);
    }

    float expFactor;
    SampleType yOld;
    bool increasing;

    SampleType tauAtt = (SampleType) 1;
    SampleType tauRel = (SampleType) 1;
};
