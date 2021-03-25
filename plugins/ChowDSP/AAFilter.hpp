#pragma once

#include "iir.hpp"
#include <vector>

/** 
    High-order filter to be used for anti-aliasing or anti-imaging.
    The template parameter N should be 1/2 the desired filter order.

    Currently uses an 2*N-th order Butterworth filter.
    @TODO: implement Chebyshev, Elliptic filter options.
*/
template<int N>
class AAFilter
{
public:
    AAFilter() = default;

    /** Calculate Q values for a Butterworth filter of a given order */
    static std::vector<float> calculateButterQs(int order) {
        const int lim = int (order / 2);
        std::vector<float> Qs;

        for(int k = 1; k <= lim; ++k) {
            auto b = -2.0f * std::cos((2.0f * k + order - 1) * 3.14159 / (2.0f * order));
            Qs.push_back(1.0f / b);
        }

        std::reverse(Qs.begin(), Qs.end());
        return Qs;
    }

    /**
     * Resets the filter to process at a new sample rate.
     * 
     * @param sampleRate: The base (i.e. pre-oversampling) sample rate of the audio being processed
     * @param osRatio: The oversampling ratio at which the filter is being used
     */ 
    void reset(float sampleRate, int osRatio) {
        float fc = 0.98f * (sampleRate / 2.0f);
        auto Qs = calculateButterQs(2*N);

        for(int i = 0; i < N; ++i)
            filters[i].setParameters(BiquadFilter::Type::LOWPASS, fc / (osRatio * sampleRate), Qs[i], 1.0f);
    }
    
    inline float process(float x) noexcept {
        for(int i = 0; i < N; ++i)
            x = filters[i].process(x);
        
        return x;
    }

private:
    BiquadFilter filters[N];
};
