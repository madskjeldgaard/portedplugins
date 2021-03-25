// Copyright 2016 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Simple modal synthesis voice with a mallet exciter:
// click -> LPF -> resonator.
//
// The click is replaced by continuous white noise when the trigger input
// of the module is not patched.

#include "../mkutils.hpp"
#include "math.h"
#include "mi-lookuptables.h"
#include <algorithm>
#include "../constants.h"

#pragma once

namespace mi {
using namespace std;

extern const float lut_pitch_ratio_high[];
extern const float lut_pitch_ratio_low[];

/*
 * MI utilities
 */
#define MAKE_INTEGRAL_FRACTIONAL(x)                                            \
  int32_t x##_integral = static_cast<int32_t>(x);                              \
  float x##_fractional = x - static_cast<float>(x##_integral);

inline float Interpolate(const float *table, float index, float size) {
  index *= size;
  MAKE_INTEGRAL_FRACTIONAL(index)
  float a = table[index_integral];
  float b = table[index_integral + 1];
  return a + (b - a) * index_fractional;
}

inline float SemitonesToRatio(float semitones) {
  float pitch = semitones + 128.0f;
  MAKE_INTEGRAL_FRACTIONAL(pitch)

  return lut_pitch_ratio_high[pitch_integral] *
         lut_pitch_ratio_low[static_cast<int32_t>(pitch_fractional * 256.0f)];
}

inline float NthHarmonicCompensation(int n, float stiffness) {
  float stretch_factor = 1.0f;
  for (int i = 0; i < n - 1; ++i) {
    stretch_factor += stiffness;
    if (stiffness < 0.0f) {
      stiffness *= 0.93f;
    } else {
      stiffness *= 0.98f;
    }
  }
  return 1.0f / stretch_factor;
}
enum FrequencyApproximation {
  FREQUENCY_EXACT,
  FREQUENCY_ACCURATE,
  FREQUENCY_FAST,
  FREQUENCY_DIRTY
};

enum FilterMode {
  FILTER_MODE_LOW_PASS,
  FILTER_MODE_BAND_PASS,
  FILTER_MODE_BAND_PASS_NORMALIZED,
  FILTER_MODE_HIGH_PASS
};

#define M_PI_F float(M_PI)
#define M_PI_POW_2 M_PI *M_PI
#define M_PI_POW_3 M_PI_POW_2 *M_PI
#define M_PI_POW_5 M_PI_POW_3 *M_PI_POW_2
#define M_PI_POW_7 M_PI_POW_5 *M_PI_POW_2
#define M_PI_POW_9 M_PI_POW_7 *M_PI_POW_2
#define M_PI_POW_11 M_PI_POW_9 *M_PI_POW_2

enum CosineOscillatorMode {
  COSINE_OSCILLATOR_APPROXIMATE,
  COSINE_OSCILLATOR_EXACT
};

class CosineOscillator {
public:
  CosineOscillator() {}
  ~CosineOscillator() {}

  template <CosineOscillatorMode mode> inline void Init(float frequency) {
    if (mode == COSINE_OSCILLATOR_EXACT) {
      InitApproximate(frequency);
    } else {
      iir_coefficient_ = 2.0f * cosf(2.0f * M_PI * frequency);
      initial_amplitude_ = iir_coefficient_ * 0.25f;
    }
    Start();
  }

  inline void InitApproximate(float frequency) {
    float sign = 16.0f;
    frequency -= 0.25f;
    if (frequency < 0.0f) {
      frequency = -frequency;
    } else {
      if (frequency > 0.5f) {
        frequency -= 0.5f;
      } else {
        sign = -16.0f;
      }
    }
    iir_coefficient_ = sign * frequency * (1.0f - 2.0f * frequency);
    initial_amplitude_ = iir_coefficient_ * 0.25f;
  }

  inline void Start() {
    y1_ = initial_amplitude_;
    y0_ = 0.5f;
  }

  inline float value() const { return y1_ + 0.5f; }

  inline float Next() {
    float temp = y0_;
    y0_ = iir_coefficient_ * y0_ - y1_;
    y1_ = temp;
    return temp + 0.5f;
  }

private:
  float y1_;
  float y0_;
  float iir_coefficient_;
  float initial_amplitude_;
};
/*
 * MI OnePole filter used in filter abstractions.
 */
class OnePole {
public:
  OnePole() {}
  ~OnePole() {}

  void Init() {
    set_f<FREQUENCY_DIRTY>(0.01f);
    Reset();
  }

  void Reset() { state_ = 0.0f; }

  template <FrequencyApproximation approximation>
  static inline float tan(float f) {
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

  // Set frequency and resonance from true units. Various approximations
  // are available to avoid the cost of tanf.
  template <FrequencyApproximation approximation> inline void set_f(float f) {
    g_ = tan<approximation>(f);
    gi_ = 1.0f / (1.0f + g_);
  }

  template <FilterMode mode> inline float Process(float in) {
    float lp;
    lp = (g_ * in + state_) * gi_;
    state_ = mkutils::flushed(g_ * (in - lp) + lp);

    if (mode == FILTER_MODE_LOW_PASS) {
      return lp;
    } else if (mode == FILTER_MODE_HIGH_PASS) {
      return in - lp;
    } else {
      return 0.0f;
    }
  }

  template <FilterMode mode> inline void Process(float *in_out, size_t size) {
    while (size--) {
      *in_out = Process<mode>(*in_out);
      ++in_out;
    }
  }

private:
  FrequencyApproximation m_approx;
  float g_;
  float gi_;
  float state_;
};

/*
 * RESONATOR STATE VARIABLE FILTER.
 * A component used in the Resonator filter itself.
 */
template <int batch_size> class ResonatorSVF {
public:
  ResonatorSVF() {}
  ~ResonatorSVF() {}

  void Init() {
    for (int i = 0; i < batch_size; ++i) {
      state_1_[i] = state_2_[i] = 0.0f;
    }
  }

  template <FilterMode mode, bool add>
  void Process(const float *f, const float *q, const float *gain,
               const float *in, float *out, size_t size) {
    float g[batch_size];
    float r[batch_size];
    float r_plus_g[batch_size];
    float h[batch_size];
    float state_1[batch_size];
    float state_2[batch_size];
    float gains[batch_size];
    for (int i = 0; i < batch_size; ++i) {
      g[i] = OnePole::tan<FREQUENCY_EXACT>(f[i]);
      r[i] = 1.0f / q[i];
      h[i] = 1.0f / (1.0f + r[i] * g[i] + g[i] * g[i]);
      r_plus_g[i] = r[i] + g[i];
      state_1[i] = state_1_[i];
      state_2[i] = state_2_[i];
      gains[i] = gain[i];
    }

    while (size--) {
      float s_in = *in++;
      float s_out = 0.0f;
      for (int i = 0; i < batch_size; ++i) {
        const float hp = (s_in - r_plus_g[i] * state_1[i] - state_2[i]) * h[i];
        const float bp = g[i] * hp + state_1[i];
        state_1[i] = g[i] * hp + bp;
        const float lp = g[i] * bp + state_2[i];
        state_2[i] = g[i] * bp + lp;
        s_out += gains[i] * ((mode == FILTER_MODE_LOW_PASS) ? lp : bp);
      }
      if (add) {
        *out++ += s_out;
      } else {
        *out++ = s_out;
      }
    }
    for (int i = 0; i < batch_size; ++i) {
      state_1_[i] = state_1[i];
      state_2_[i] = state_2[i];
    }
  }

private:
  float state_1_[batch_size];
  float state_2_[batch_size];
};

enum ResonatorProcessingMode {
  CHAEP,
  BUDGET,
  PRETTYGOOD,
  EXPENSIVE,
  LUXURY,
  EXTREME,
  NumProcessingModes
};

// Must be power of two
const int kMaxNumModes = 512;
const int kModeBatchSize = 8;

/*
 * The resonator used in Rings, this is the floating point version from Plaits.
 */
class Resonator {
public:
  Resonator() {}
  ~Resonator() {}

  void Init(ResonatorProcessingMode mode, float cosFreq) {
    setModes(mode, cosFreq);

	// NOTE: Not sure what the point of this is.,..
    /* resolution_ = min(resolution, numModes_); */
  };

  void setModes(ResonatorProcessingMode mode, float cosFreq) {
    // Set number of modes
    switch (mode) {
    case CHAEP:
      numModes_ = 16;
      break;
    case BUDGET:
      numModes_ = 32;
      break;
    case PRETTYGOOD:
      numModes_ = 64;
      break;
    case EXPENSIVE:
      numModes_ = 128;
      break;
    case LUXURY:
      numModes_ = 256;
      break;
    case EXTREME:
      numModes_ = 512;
      break;
    default:
      numModes_ = 32;
    }

	resolution_ = numModes_;

    float filterAmplitudesScale = 0.125f;

    amplitudes_.Init<COSINE_OSCILLATOR_APPROXIMATE>(cosFreq);
	/* amplitudes_.Init<COSINE_OSCILLATOR_EXACT>(cosFreq); */

    for (int i = 0; i < resolution_; ++i) {
      mode_amplitude_[i] = amplitudes_.Next() * filterAmplitudesScale;
    }

    for (int i = 0; i < numModes_ / kModeBatchSize; ++i) {
      mode_filters_[i].Init();
    }
  };

  void Process(float f0, float structure, float brightness, float damping,
               float stretch, float loss, const float *in, float *out,
               size_t size) {

    float stiffness = Interpolate(lut_stiffness, structure, 64.0f);
    f0 *= NthHarmonicCompensation(3, stiffness);

    // Offset stretch param so that it doesn't go below 1.0
    stretch += 1.0f;

    // This makes more sense: The higher the damping param, the more it dampens
    damping = 1.0f - damping;
    /* loss = (1.0f - loss); //1* 0.99f + 0.01f; */

    float harmonic = f0;
    float q_sqrt = SemitonesToRatio(damping * 79.7f);
    float q = 500.0f * q_sqrt * q_sqrt;

    brightness *= 1.0f - structure * 0.3f;
    brightness *= 1.0f - damping * 0.3f;
    float q_loss = brightness * (2.0f - brightness) * (loss) + (1.0f - loss);

    float mode_q[kModeBatchSize];
    float mode_f[kModeBatchSize];
    float mode_a[kModeBatchSize];
    int batch_counter = 0;

    ResonatorSVF<kModeBatchSize> *batch_processor = &mode_filters_[0];

    for (int i = 0; i < resolution_; ++i) {
      float mode_frequency = harmonic * stretch;
      if (mode_frequency >= 0.499f) {
        mode_frequency = 0.499f;
      }
      const float mode_attenuation = 1.0f - mode_frequency * 2.0f;

      mode_f[batch_counter] = mode_frequency;
      mode_q[batch_counter] = 1.0f + mode_frequency * q;
      mode_a[batch_counter] = mode_amplitude_[i] * mode_attenuation;
      ++batch_counter;

      if (batch_counter == kModeBatchSize) {
        batch_counter = 0;
        batch_processor->Process<FILTER_MODE_BAND_PASS, true>(
            mode_f, mode_q, mode_a, in, out, size);
        ++batch_processor;
      }

      stretch += stiffness;
      if (stiffness < 0.0f) {
        // Make sure that the partials do not fold back into negative
        // frequencies.
        stiffness *= 0.93f;
      } else {
        // This helps adding a few extra partials in the highest frequencies.
        stiffness *= 0.98f;
      }
      harmonic += f0;
      q *= q_loss;
    }
  };

private:
  int resolution_{0};
  int numModes_{0};

  CosineOscillator amplitudes_;

  float mode_amplitude_[kMaxNumModes];
  ResonatorSVF<kModeBatchSize> mode_filters_[kMaxNumModes / kModeBatchSize];
};

} // namespace mi
