/*
 * @TODO:
 * - Are arrays necessary here?
 *
 */
#include "buchla259cell.hpp"
#include "SC_Constants.h"
#include <iostream>
#include <ostream>

namespace buchla259 {

// Signum function from
// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sign(T val) { return (T(0) < val) - (val < T(0)); }

void Buchla259FoldingCell::init(float samplerate, double f0, double amp,
                                double r1, double r3, double outputscalar) {
  m_output_scalar = outputscalar;
  m_samplerate = samplerate;
  for (int i = 0; i < 4; i++) {
    flags[i] = 0;
    cl_pts[i] = 0;
  }

  setF0(f0);
  setR1(r1);
  setR3(r3);
  setAmplitude(amp);
}

float Buchla259FoldingCell::process() {
  // Read fundamental freq and ampl
  double Ts = 1.0 / m_samplerate;
  double thresh = 0.01;

  // Compute clipping threshold L
  double Vs = 6;
  double R2 = 100e3;
  double L = Vs * (R1 / R2);

  // Dummy variable(s)
  double Vk_ic = 0;

  // Helper constants
  double G = ((R2 * R3) / (R1 * R3 + R2 * R3 + R1 * R2));
  double one_sixth = 1.0 / 6.0;

  // Synthesize input
  double xn = A * sin(twopi * ph);

  // Inverse Clipper
  if (std::abs(xn) > L) {
    Vk_ic = xn;
  } else {
    Vk_ic = sign(xn) * L;
  }

  if (A >= L) {

    // Compute clipping points
    double clp1 = std::asin(L / A) / twopi;
    double clp2 = 0.5 - clp1;
    double clp3 = 0.5 + clp1;
    double clp4 = 1 - clp1;
    cl_pts[0] = clp1;
    cl_pts[1] = clp2;
    cl_pts[2] = clp3;
    cl_pts[3] = clp4;

    // Reset counters and check for that really annoying first case.
    auto fl4 = flags[3];

    if ((ph < delta) && (fl4 == 1)) {

      resetFlags();

    } else if ((ph < delta) && (fl4 == 0)) {

      clp4 = cl_pts[3];

      double d = 1 - ((1 - clp4 + ph) / delta);

      double tripled = d * d * d;
      double h0 = -tripled * one_sixth + 0.5 * (d * d) - 0.5 * d + one_sixth;
      double h1 = tripled * one_sixth;

      double twopiclp4 = twopi * clp4;
      double pol = sign(A * sin(twopiclp4));
      double mu = std::abs(A * cos(twopiclp4) * (twopi * m_f0 * Ts));

      if (mu < thresh) {
        mu = 0;
      }

      Vk_ic = Vk_ic + pol * mu * h1;
      Vk_ic_n1 = Vk_ic_n1 + pol * mu * h0;

      resetFlags();
    }

    // CHECK FOR CLIPPING POINTS
    for (int m = 0; m < 4; m++) {

      auto clp = cl_pts[m];
      auto flg = flags[m];

      if ((ph > clp) && (ph > (ph - delta)) && (flg == 0)) {

        double d = 1 - (ph - clp) / delta;
        double tripled = d * d * d;
        double h0 = -tripled * one_sixth + 0.5 * (d * d) - 0.5 * d + one_sixth;
        double h1 = tripled * one_sixth;

        // @FIXME: the sin and cos here could be optimized
        double twopiclip = twopi * clp;
        double pol = sign(A * sin(twopiclip));
        double mu = std::abs(A * cos(twopiclip) * (twopi * m_f0 * Ts));

        if (mu < thresh) {
          mu = 0;
        }

        Vk_ic = Vk_ic + pol * mu * h1;
        Vk_ic_n1 = Vk_ic_n1 + pol * mu * h0;

        flags[m] = 1;
      }
    }
  }

  // Restore folding function
  double out1 = G * (Vk_ic_n1 - sign(Vk_ic_n1) * L);

  // Update States
  Vk_ic_n1 = Vk_ic;

  // Increase counter
  ph = fmod(delta + ph, 1);

  return out1 * m_output_scalar;
}

} // namespace buchla259
