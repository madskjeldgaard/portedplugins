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

#pragma once
#include "../fastmath.h"

class SEMFilter12 {
 public:
  SEMFilter12();
  ~SEMFilter12();

  void setup() {
    m_alpha_0 = 1.0;
    m_alpha = 1.0;
    m_rho = 1.0;

    // @FIXME: not needed?
    m_transition_mod = 0;
    setTransition(-1);

    reset();

    setResControl(0.1);
    setSampleRate(44100);

    // @FIXME not necessary?
    m_res_mod = 0;

    m_resonance = 0.5f;
    m_resonance_modded = m_resonance;
  }
  void reset();
  void setFreq(double freq) {
    m_freq_modded = freq;
    update();
  };

  void update();
  double doFilter(double xn);
  void setResControl(double res);

  double m_transition;
  double m_resonance;
  double m_resonance_modded;
  double m_alpha;
  double m_alpha_0;
  double m_rho;

  void setTransition(double trans) { m_transition = trans; }
  /* void setTransitionModPointer(float *p_pointer) { */
  /*   m_transition_mod = p_pointer; */
  /* } */
  void setSampleRate(double p_sr);

 protected:
  double m_last_freq_modded = -1;

  float m_transition_mod;

  double m_freq_modded, m_res_mod;
  double m_samplerate, m_one_over_samplerate;

  double m_z_1;
  double m_z_2;
};
