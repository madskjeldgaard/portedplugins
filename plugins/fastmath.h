#include "constants.h"

namespace mkfastmath {

enum FrequencyApproximation {
  FREQUENCY_EXACT,
  FREQUENCY_ACCURATE,
  FREQUENCY_FAST,
  FREQUENCY_DIRTY
};

// Fast tan approximation by Emilie Pichenettes / Mutable Instruments
template <FrequencyApproximation approximation>
static inline float fasttan(float f) {
  if (approximation == FREQUENCY_EXACT) {
    // Clip coefficient to about 100.
    f = f < 0.497f ? f : 0.497f;
    return tanf(M_PI * f);
  } else if (approximation == FREQUENCY_DIRTY) {
    // Optimized for frequencies below 8kHz.
    const float a = 3.736e-01 * M_PI_POW_3;
    return f * (M_PI_F + a * f * f);
  } else if (approximation == FREQUENCY_FAST) {
    // The usual tangent approximation uses 3.1755e-01 and 2.033e-01, but
    // the coefficients used here are optimized to minimize error for the
    // 16Hz to 16kHz range, with a sample rate of 48kHz.
    const float a = 3.260e-01 * M_PI_POW_3;
    const float b = 1.823e-01 * M_PI_POW_5;
    float f2 = f * f;
    return f * (M_PI_F + f2 * (a + b * f2));
  } else if (approximation == FREQUENCY_ACCURATE) {
    // These coefficients don't need to be tweaked for the audio range.
    const float a = 3.333314036e-01 * M_PI_POW_3;
    const float b = 1.333923995e-01 * M_PI_POW_5;
    const float c = 5.33740603e-02 * M_PI_POW_7;
    const float d = 2.900525e-03 * M_PI_POW_9;
    const float e = 9.5168091e-03 * M_PI_POW_11;
    float f2 = f * f;
    return f * (M_PI_F + f2 * (a + f2 * (b + f2 * (c + f2 * (d + f2 * e)))));
  }
}

}  // namespace mkfastmath
