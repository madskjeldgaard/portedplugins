#ifndef MK_BUCHLA259WAVESHAPER
#define MK_BUCHLA259WAVESHAPER
#pragma once

#include "buchla259cell.hpp"

namespace buchla259waveshaper {

constexpr int kNumCells = 5;

/**
 * @brief A virtual analog Buchla 259 wavefolder
 * @author Mads Kjeldgaard (C++ / SuperCollider port), Fabian Esqueda and Julian
 * Parker (original authors)
 * @date 12-04-2021
 * @details A realistic, oversampled virtual analog representation of the Buchla
 * 259 wavefolder circuit. See the original paper for details:
 * http://research.spa.aalto.fi/publications/papers/dafx17-wavefolder/
 */

class Buchla259WaveShaper {
public:
  void init(float samplerate);
  float process();

  void setF0(float f0) {
    for (int i = 0; i < kNumCells; i++) {
      cell[i].setF0(f0);
    }
  }

  void setAmplitude(float amp) {
    for (int i = 0; i < kNumCells; i++) {
      cell[i].setAmplitude(amp);
    }
  }

private:
  float m_samplerate;
  buchla259::Buchla259FoldingCell cell[kNumCells];
};
} // namespace buchla259waveshaper

#endif
