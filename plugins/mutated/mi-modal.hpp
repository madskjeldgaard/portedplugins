#pragma once

#include "mi-resonator.cpp"
#include <random>

namespace mi {

/*
 * A mutable instruments modal voice, originally from Rings, then repurposed in
 * Plaits. And now, repurposed here.
 */
class ModalVoice {
public:
  ModalVoice() {}
  ~ModalVoice() {}

  void Init(ResonatorProcessingMode mode, float initCosFreq, float samplerate);
  void Render(bool sustain, bool trigger, float accent, float f0,
              float structure, float brightness, float damping, float stretch, float position, float loss, float *temp,
              float *out, size_t size);
  float Dust(float frequency);

private:
  ResonatorSVF<1> excitation_filter_;
  Resonator resonator_;

  std::uniform_real_distribution<float> random_dist_;
  std::mt19937 mersenne_engine_;
};

} // namespace mi
