#ifndef MK_BUCHLA259WAVESHAPER
#define MK_BUCHLA259WAVESHAPER
#pragma once

#include "SC_Constants.h"
#include "buchla259cell.hpp"
#include <cmath>

namespace buchla259waveshaper {

constexpr int kNumCells = 5;

/**
 * @brief A virtual analog Buchla 259 wavefolder
 * @author Mads Kjeldgaard (C++ / SuperCollider port), based on research by
 * Fabián Esqueda, Henri Pöntynen, Vesa Välimäki and Julian D. Parker.
 * @date 12-04-2021
 * @details A realistic, oversampled virtual analog representation of the Buchla
 * 259 wavefolder circuit. See the original paper for details:
 * http://research.spa.aalto.fi/publications/papers/dafx17-wavefolder/
 */

struct BuchlaSine {
  void init(float samplerate, double initF0, double initAmp) {
    Fs = samplerate;
    setAmplitude(initAmp);
    setF0(initF0);
  }

  void setAmplitude(double amp) { A = amp; }

  void setF0(double freq) {
    f0 = freq;
    delta = f0 / Fs;
  }

  float process() {
    // Synthesize input
    double xn = A * sin(2 * pi * ph);

    double out1 = xn1;

    // Increase counter
    ph = std::fmod(ph + delta, 1);

    // Update state
    xn1 = xn;

    return out1;
  }

  // Read fundamental freq and ampl
  double A, f0, Fs, delta, xn1{0}, ph{0.0};
};

class Buchla259WaveShaper {
public:
  void init(float samplerate);
  float process();

  void setF0(float f0) {
    for (int i = 0; i < kNumCells; i++) {
      cell[i].setF0(f0);
    }

    sine.setF0(f0);
  }

  void setAmplitude(float amp) {
    amp *= 10.0;

    for (int i = 0; i < kNumCells; i++) {
      cell[i].setAmplitude(amp);
    }

    sine.setAmplitude(amp);
  }

private:
  float m_samplerate;
  buchla259::Buchla259FoldingCell cell[kNumCells];
  BuchlaSine sine;
};
} // namespace buchla259waveshaper

#endif
