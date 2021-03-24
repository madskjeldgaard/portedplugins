#ifndef MODE_H_INCLUDED
#define MODE_H_INCLUDED

#include <complex>

namespace chow{
/**
 * A modal oscillator using the Max Mathews phasor filter.
 * For more information, see: https://ccrma.stanford.edu/~jos/smac03maxjos/
 */ 
class Mode
{
public:
    Mode() = default;

    void init (double sampleRate)
    {
        fs = (float) sampleRate;

        decayFactor = pow (0.001f, 1.0f / (t60 * fs));
        oscCoef = exp (jImag * 2.0f * (float) M_PI * (freq / fs));
        updateParams();

        reset();
    }

    inline void reset() noexcept
    {
        y1 = 0.0f;
    }

    inline void setAmp (float amp, float phase) noexcept
    {
        A = std::polar(amp, phase);
    }

    inline void setDecay (float newT60, float dampFactor = 1.0f) noexcept
    {
        if (t60 == newT60)
            return;

        t60 = newT60;
        decayFactor = pow (0.001f, 1.0f / (t60 * fs)) * dampFactor;
    }

    inline void setFreq (float newFreq) noexcept
    {
        if (freq == newFreq)
            return;

        freq = newFreq;
        oscCoef = exp (jImag * 2.0f * (float) M_PI * (freq / fs));
    }

    inline void updateParams() noexcept { filtCoef = decayFactor * oscCoef; }

    inline float process (float x) noexcept
    {
        auto y = A * x + filtCoef * y1;

        y1 = y;
        return std::imag (y);
    }

private:
    std::complex<float> filtCoef = 0.0f;
    float decayFactor = 0.0f;
    std::complex<float> oscCoef = 0.0f;

    std::complex<float> y1 = 0.0f;

    float freq = 100.0f;
    float t60 = 1.0f;
    std::complex<float> A;

    float fs = 44100.0f;
    const std::complex<float> jImag = std::complex<float> (0, 1);
};

}
#endif // MODEOSCILLATOR_H_INCLUDED
