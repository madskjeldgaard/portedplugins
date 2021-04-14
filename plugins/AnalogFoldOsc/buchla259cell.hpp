#ifndef MK_BUCHLA259
#define MK_BUCHLA259
#pragma once
#include "SC_BoundsMacros.h"
#include "SC_Constants.h"
#include <iostream>
#include <ostream>

namespace buchla259 {
// Signum function from
// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sign(T val) { return (T(0) < val) - (val < T(0)); }

// Helper constants
constexpr double one_sixth = 1.0 / 6.0;

/**
 * @brief A virtual analog Buchla 259 wavefolder folding cell
 * @author Mads Kjeldgaard (C++ / SuperCollider port), based on research by
 * Fabián Esqueda, Henri Pöntynen, Vesa Välimäki and Julian D. Parker.
 * @date 12-04-2021
 * @details This is a folding cell sub circuit using in a parallel waveshaper
 * ala Buchla 259.
 */
class Buchla259FoldingCell {
public:
  void init(double samplerate, double f0, double amp, double r1, double r3,
            double outputscalar);
  float process();

  void setF0(double f0) {
    m_f0 = f0;

    // Phase increment
    delta = m_f0 / m_samplerate;
  };

  void setAmplitude(float amp) { A = amp; }

  void setR1(double R1val) {
    R1 = R1val;
    recalculateG();
  }

  void setR3(double R3val) {
    R3 = R3val;
    recalculateG();
  }

  void recalculateG() { G = ((R2 * R3) / (R1 * R3 + R2 * R3 + R1 * R2)); }

  void resetFlags() {
    for (int i = 0; i < 4; i++) {
      flags[i] = 0;
    }
  }

  inline void clippingPointCalc(double d, double clippingPoint,
                                double &vk_ic_n1, double &vk_ic) {
    double twod = d * d;
    double tripled = twod * d;
    double h0 = -tripled * one_sixth + 0.5 * twod - 0.5 * d + one_sixth;
    double h1 = tripled * one_sixth;

    // @FIXME: the sin and cos here could be optimized
    double twopiclip = twopi * clippingPoint;
    double pol = sign(A * sin(twopiclip));
    double mu = std::abs(A * cos(twopiclip) * (twopi * m_f0 * Ts));

    if (mu < thresh) {
      mu = 0;
    }

    vk_ic = vk_ic + pol * mu * h1;
    vk_ic_n1 = vk_ic_n1 + pol * mu * h0;
  };

private:
  float m_samplerate;
  double m_f0, A, R1, R2, R3, Ts, thresh, Vs, L;

  double G;

  double m_output_scalar;

  double Vk_ic_n1{0}, delta{0};
  double ph{0};
  double flags[4], cl_pts[4];
};
} // namespace buchla259

#endif
