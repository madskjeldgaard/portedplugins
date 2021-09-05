#include "LockhartCell.hpp"
#include "SC_InlineUnaryOp.h"

double LockhartCell::lambert_w(double x) {

  // Error threshold
  constexpr double thresh = 10e-10;

  // Initial guess (use previous value)
  double w = Ln1;

  // Haley's method (Sec. 4.2 of the paper)
  for (int i = 0; i < 1000; i++) {

    /* const auto expw = pow(e, w); */

    const double expw = exp(w);
    const double p = w * expw - x;
    const double r = (w + 1.0) * expw;
    const double s = (w + 2.0) / (2.0 * (w + 1.0));
    const double err = (p / (r - (p * s)));

    if (abs(err) < thresh) {
      break;
    }

    w = w - err;
  }

  return w;
};

double LockhartCell::process(double in1) {
  double out1 = 0.0;

  // Constants
  constexpr auto RL = 7.5e3;
  constexpr auto R = 15e3;
  constexpr auto VT = 26e-3;
  constexpr auto Is = 10e-16;

  constexpr auto a = 2 * RL / R;
  constexpr auto b = (R + 2 * RL) / (VT * R);
  constexpr auto d = (RL * Is) / VT;

  // Antialiasing error threshold
  constexpr auto thresh = 10e-10;

  // Compute Antiderivative
  const double l = mkutils::sign(in1);

  /* const auto u = d * pow(e, l * b * in1); */
  double u = d * exp(l * b * in1);
  double Ln = lambert_w(u);
  const double Fn = (0.5 * VT / b) * (Ln * (Ln + 2)) - 0.5 * a * in1 * in1;

  // Check for ill-conditioning
  if (std::fabs(in1 - xn1) < thresh) {

    // Compute Averaged Wavefolder Output
    const double xn = 0.5 * (in1 + xn1);
    /* u = d * pow(e, l * b * xn); */

    u = d * exp(l * b * xn);
    Ln = lambert_w(u);
    out1 = l * VT * Ln - a * xn;

  } else {

    // Apply AA Form
    out1 = (Fn - Fn1) / (in1 - xn1);
  }

  // Update States
  Ln1 = Ln;
  Fn1 = Fn;
  xn1 = in1;

  return out1;
};
