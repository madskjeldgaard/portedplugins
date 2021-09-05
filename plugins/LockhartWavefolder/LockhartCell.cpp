#include "LockhartCell.hpp"

double LockhartCell::lambert_w(double x) {

  // Error threshold
  constexpr auto thresh = 10e-10;

  // Initial guess (use previous value)
  auto w = Ln1;
  const auto expw = exp(w);

  // Haley's method (Sec. 4.2 of the paper)
  for (int i = 0; i < 1000; i++) {

    /* const auto expw = pow(e, w); */

    const auto p = w * expw - x;
    const auto r = (w + 1.0) * expw;
    const auto s = (w + 2.0) / (2.0 * (w + 1.0));
    const auto err = (p / (r - (p * s)));

    if (std::abs(err) < thresh) {
      break;
    }

    w = w - err;
  }

  return w;
};

float LockhartCell::process(float in1) {
  float out1 = 0.0;

  // Constants
  constexpr auto RL = 7.5e3;
  constexpr auto R = 15e3;
  constexpr auto VT = 26e-3;
  constexpr auto Is = 10e-16;

  constexpr auto a = 2.0 * RL / R;
  constexpr auto b = (R + 2.0 * RL) / (VT * R);
  constexpr auto d = (RL * Is) / VT;

  // Antialiasing error threshold
  constexpr auto thresh = 10e-10;

  // Compute Antiderivative
  const auto l = mkutils::sign(in1);
  /* const auto u = d * pow(e, l * b * in1); */
  auto u = d * exp(l * b * in1);
  auto Ln = lambert_w(u);
  const auto Fn = (0.5 * VT / b) * (Ln * (Ln + 2.0)) - 0.5 * a * in1 * in1;

  // Check for ill-conditioning
  if (std::abs(in1 - xn1) < thresh) {

    // Compute Averaged Wavefolder Output
    const auto xn = 0.5 * (in1 + xn1);
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
