#include "mi-modal.hpp"
#include <cstdint>
#include <iostream>

namespace mi {

/*
 * Random generation
 */
/* class Random { */
/* public: */
/*   static inline uint32_t state() { return rng_state_; } */

/*   static inline void Seed(uint32_t seed) { rng_state_ = seed; } */

/*   static inline uint32_t GetWord() { */
/*     rng_state_ = rng_state_ * 1664525L + 1013904223L; */
/*     return state(); */
/*   } */

/*   static inline int16_t GetSample() { */
/*     return static_cast<int16_t>(GetWord() >> 16); */
/*   } */

/*   static inline float GetFloat() { */
/*     return static_cast<float>(GetWord()) / 4294967296.0f; */
/*   } */

/* private: */
/*   static uint32_t rng_state_; */
/* }; */

inline float ModalVoice::Dust(float frequency) {
  float inv_frequency = 1.0f / frequency;
  float u = random_dist_(mersenne_engine_);
  if (u < frequency) {
    return u * inv_frequency;
  } else {
    return 0.0f;
  }
}

void ModalVoice::Init(ResonatorProcessingMode mode, float initCosFreq) {
  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 mersenne_engine_(
      rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<float> random_dist_(0.0f, 1.0f);

  excitation_filter_.Init();

  resonator_.Init(mode, initCosFreq);
}

void ModalVoice::Render(bool sustain, bool trigger, float accent, float f0,
                        float structure, float brightness, float damping,
                        float stretch, float position, float loss, float *temp,
                        float *out, size_t size) {
  fill(&out[0], &out[size], 0.0f);

  // Scale f0 so that inputs from 0.0 to 1.0 make better sense.
  f0 = (f0 * 0.05) + 0.00001;

  const float density = brightness * brightness;

  // NOTE: Is this position?
  brightness += position * accent * (1.0f - brightness);
  damping += position * accent * (1.0f - damping);

  const float range = sustain ? 36.0f : 60.0f;
  /* const float range = 60.f; */
  /* const float f = sustain ? 4.0f * f0 : 2.0f * f0; */
  const float f = 2.0f * f0;
  const float cutoff = min(
      f * SemitonesToRatio((brightness * (2.0f - brightness) - 0.5f) * range),
      0.499f);
  const float q = sustain ? 0.7f : 1.5f;
  /* const float q = 0.7f; */

  // Synthesize excitation signal.
  if (sustain) {
    const float dust_f = 0.00005f + 0.99995f * density * density;
    for (size_t i = 0; i < size; ++i) {
      temp[i] = Dust(dust_f) * (4.0f - dust_f * 3.0f) * accent;
    }
  } else {
    fill(&temp[0], &temp[size], 0.0f);
    if (trigger) {
      const float attenuation = 1.0f - damping * 0.5f;
      const float amplitude = (0.12f + 0.08f * accent) * attenuation;
      temp[0] = amplitude * SemitonesToRatio(cutoff * cutoff * 24.0f) / cutoff;
    }
  }
  const float one = 1.0f;
  excitation_filter_.Process<FILTER_MODE_LOW_PASS, false>(&cutoff, &q, &one,
                                                          temp, temp, size);
  resonator_.Process(f0, structure, brightness, damping, stretch, loss, temp,
                     out, size);
}

extern const float lut_stiffness[];

} // namespace mi
