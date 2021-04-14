/*
 * @TODO:
 * - Are arrays necessary here?
 *
 */
#include "buchla259cell.hpp"

namespace buchla259 {

void Buchla259FoldingCell::init(double samplerate, double f0, double amp,
                                double r1, double r3, double outputscalar) {
  m_output_scalar = outputscalar;
  m_samplerate = samplerate;

  for (int i = 0; i < 4; i++) {
    flags[i] = 0;
    cl_pts[i] = 0;
  }

  Ts = 1.0 / m_samplerate;
  thresh = 0.01;
  Vs = 6;

  setF0(f0);
  R2 = 100e3;
  setR1(r1);
  setR3(r3);
  recalculateG();
  setAmplitude(amp);

  L = Vs * (R1 / R2);
}

float Buchla259FoldingCell::process() {
  double Vk_ic = 0;

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

    auto fl4 = flags[3];
    if ((ph < delta) && (fl4 == 1)) {

      resetFlags();

    } else if ((ph < delta) && (fl4 == 0)) {

      clp4 = cl_pts[3];

      double d = 1 - ((1 - clp4 + ph) / delta);
      d = sc_clip(d, 0.0, 1.0);
      clippingPointCalc(d, clp4, Vk_ic_n1, Vk_ic);
      resetFlags();
    }

    // CHECK FOR CLIPPING POINTS
    for (int m = 0; m < 4; m++) {

      auto clp = cl_pts[m];
      auto flg = flags[m];

      if ((ph > clp) && (ph > (ph - delta)) && (flg == 0)) {

        // @FIXME: d occasionally exceeds it's limits of 0.0-1.0. This is hard
        // clipped at the moment but that only removes the symptom and not the
        // cause.
        double d = 1 - (ph - clp) / delta;
        d = sc_clip(d, 0.0, 1.0);
        clippingPointCalc(d, clp, Vk_ic_n1, Vk_ic);
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
