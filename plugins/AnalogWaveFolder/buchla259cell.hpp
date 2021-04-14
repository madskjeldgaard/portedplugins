#ifndef MK_BUCHLA259
#define MK_BUCHLA259
#pragma once

namespace buchla259 {

/**
 * @brief A virtual analog Buchla 259 wavefolder folding cell
 * @author Mads Kjeldgaard (C++ / SuperCollider port), Fabian Esqueda and Julian
 * Parker (original authors)
 * @date 12-04-2021
 * @details This is a folding cell sub circuit using in a parallel waveshaper ala Buchla 259.
 */

class Buchla259FoldingCell {
public:
  void init(float samplerate, double f0, double amp, double r1, double r3, double outputscalar);
  float process();

  void setF0(float f0) {
    m_f0 = f0;

    // Phase increment
    delta = m_f0 / m_samplerate;
  };

  void setAmplitude(float amp) { A = amp; }

  void setR1(double R1val) { R1 = R1val; }

  void setR3(double R3val) { R3 = R3val; }

  void resetFlags() {
    for (int i = 0; i < 4; i++) {
      flags[i] = 0;
    }
  }

private:
  float m_samplerate;
  double m_f0, A, R1, R3;

  double m_output_scalar;

  double Vk_ic_n1{0}, delta{0};
  double ph{0};
  double flags[4], cl_pts[4];
};
} // namespace buchla259

#endif

