#pragma once

#include "AAFilter.hpp"

/**
 * Base class for oversampling of any order
 */ 
class BaseOversampling {
public:
    BaseOversampling() = default;
    virtual ~BaseOversampling() {}

    /** Resets the oversampler for processing at some base sample rate */
    virtual void reset(float /*baseSampleRate*/) = 0;

    /** Upsample a single input sample and update the oversampled buffer */
    virtual void upsample(float) noexcept = 0;

    /** Output a downsampled output sample from the current oversampled buffer */
    virtual float downsample() noexcept = 0;

    /** Returns a pointer to the oversampled buffer */
    virtual float* getOSBuffer() noexcept = 0;
};

/** 
    Class to implement an oversampled process.
    To use, create an object and prepare using `reset()`.
    
    Then use the following code to process samples:
    @code
    oversample.upsample(x);
    for(int k = 0; k < ratio; k++)
        oversample.osBuffer[k] = processSample(oversample.osBuffer[k]);
    float y = oversample.downsample();
    @endcode
*/
template<int ratio, int filtN = 4>
class Oversampling : public BaseOversampling
{
public:
    Oversampling() = default;
    virtual ~Oversampling() {}

    void reset(float baseSampleRate) override {
        aaFilter.reset(baseSampleRate, ratio);
        aiFilter.reset(baseSampleRate, ratio);
        std::fill(osBuffer, &osBuffer[ratio], 0.0f);
    }

    inline void upsample(float x) noexcept override {
        osBuffer[0] = ratio * x;
        std::fill(&osBuffer[1], &osBuffer[ratio], 0.0f);

        for(int k = 0; k < ratio; k++)
            osBuffer[k] = aiFilter.process(osBuffer[k]);
    }

    inline float downsample() noexcept override {
        float y = 0.0f;
        for(int k = 0; k < ratio; k++)
            y = aaFilter.process(osBuffer[k]);

        return y;
    }

    inline float* getOSBuffer() noexcept override {
        return osBuffer;
    }

    float osBuffer[ratio];

private:
    AAFilter<filtN> aaFilter; // anti-aliasing filter
    AAFilter<filtN> aiFilter; // anti-imaging filter
};
