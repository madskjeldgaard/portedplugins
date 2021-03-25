#pragma once

#include "oversampling.hpp"

/** 
    Class to implement an oversampled process, with variable
    oversampling factor. To use, create an object, set the oversampling
    factor using `setOversamplingindex()` and prepare using `reset()`.
    
    Then use the following code to process samples:
    @code
    oversample.upsample(x);
    float* osBuffer = oversample.getOSBuffer();
    for(int k = 0; k < ratio; k++)
        osBuffer[k] = processSample(osBuffer[k]);
    float y = oversample.downsample();
    @endcode
*/
template<int filtN = 4>
class VariableOversampling {
public:
    VariableOversampling() = default;

    /** Prepare the oversampler to process audio at a given sample rate */
    void reset(float sampleRate) {
        for(auto* os : oss)
            os->reset(sampleRate);
    }

    /** Sets the oversampling factor as 2^idx */
    void setOversamplingIndex (int newIdx) { osIdx = newIdx; }

    /** Returns the oversampling index */
    int getOversamplingIndex() const noexcept { return osIdx; }

    /** Upsample a single input sample and update the oversampled buffer */
    inline void upsample(float x) noexcept { oss[osIdx]->upsample(x); }

    /** Output a downsampled output sample from the current oversampled buffer */
    inline float downsample() noexcept { return oss[osIdx]->downsample(); }

    /** Returns a pointer to the oversampled buffer */
    inline float* getOSBuffer() noexcept { return oss[osIdx]->getOSBuffer(); }

    /** Returns the current oversampling factor */
    int getOversamplingRatio() const noexcept { return 1 << osIdx; }


private:
    enum {
        NumOS = 5, // number of oversampling options
    };

    int osIdx = 0;

    Oversampling<1 << 0, filtN> os0; // 1x
    Oversampling<1 << 1, filtN> os1; // 2x
    Oversampling<1 << 2, filtN> os2; // 4x
    Oversampling<1 << 3, filtN> os3; // 8x
    Oversampling<1 << 4, filtN> os4; // 16x
    BaseOversampling* oss[NumOS] = { &os0, &os1, &os2, &os3, &os4 };
};

