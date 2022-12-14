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
#include "../fastmath.h"
#include "VAOnePoleFilter.h"

class DiodeFilter {
 public:
  /* DiodeFilter(); */
  /* ~DiodeFilter(); */
  void reset();
  void update();
  void setup();
  double doFilter(double xn);
  void setFreq(double freq) {
    m_freq_modded = freq;
    update();
  };
  void setResControl(double res);
  void setSampleRate(double d);

  void setOverdrive(double od) {
    m_overdrive = od;
    // @FIXME
    m_saturation_mod = 1;
  };

  inline float fasttanh(float p_input, float p_tanh_factor) {
    return tanh(p_tanh_factor * p_input);
  }
  inline void applyOverdrive(double &pio_input, float p_tanh_factor = 3.5f) {
    float overdrive_modded = m_overdrive + 2 * (m_saturation_mod);
    overdrive_modded = overdrive_modded < 0 ? 0 : overdrive_modded;
    if (overdrive_modded > 0.01f && overdrive_modded < 1.f) {
      // interpolate here so we have possibility of pure linear Processing
      pio_input = pio_input * (1. - overdrive_modded) +
                  overdrive_modded * fasttanh(pio_input, p_tanh_factor);
    } else if (overdrive_modded >= 1.f) {
      pio_input = fasttanh(overdrive_modded * pio_input, p_tanh_factor);
    }
  }

 protected:
  double m_last_freq_modded = -1;
  double m_freq_modded, m_res_mod;
  double m_k;
  double m_gamma;
  double m_sg1;
  double m_sg2;
  double m_sg3;
  double m_sg4;

  double m_samplerate, m_one_over_samplerate, m_overdrive, m_saturation_mod;

  VAOnePoleFilter m_LPF1;
  VAOnePoleFilter m_LPF2;
  VAOnePoleFilter m_LPF3;
  VAOnePoleFilter m_LPF4;
};
