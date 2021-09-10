/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

// The code for this filter is based on the diagrams and implementations in Will
// Pirkles book "Designing Software Synthesizer Plug-Ins in C++". Find out more
// on https://willpirkle.com

#pragma once
#include <math.h>
#include "../constants.h"
class VAOnePoleFilter {
public:
  VAOnePoleFilter();
  virtual ~VAOnePoleFilter();

  void setFreq(double freq) {
    m_freq_modded = freq;
    update();
  }

  void setFeedback(double fb) { m_feedback = fb; }

  double getFeedbackOutput() { return m_beta * (m_z_1 + m_feedback * m_delta); }

  void setSampleRate(double sr) { m_samplerate = sr; }
  void reset() {
    m_z_1 = 0;
    m_feedback = 0;
  }

  double doFilter(double xn);
  void update();

  inline void setLP() { m_is_lowpass = true; }

  inline void setHP() { m_is_lowpass = false; }

  // these are public for easy access
  double m_alpha; // Feed Forward coeff
  double m_beta;
  double m_gamma;    // Pre-Gain
  double m_delta;    // FB_IN Coeff
  double m_epsilon;  // FB_OUT scalar
  double m_a_0;      // input gain
  double m_feedback; // our own feedback coeff from S
  double m_freq_modded;
  double m_samplerate;

protected:
  bool m_is_lowpass = true; // else highpass
  double m_z_1;             // our z-1 storage location
};
