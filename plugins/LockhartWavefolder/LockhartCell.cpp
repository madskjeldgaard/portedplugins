#include "LockhartCell.hpp"
#include "SC_InlineUnaryOp.h"
#include <iostream>

double LockhartCell::lambert_w(double x) {

  // Error threshold
  constexpr auto thresh = 10e-10;

  // Initial guess (use previous value)
  auto w = Ln1;

  // Haley's method (Sec. 4.2 of the paper)
  for (int i = 0; i < 1000; i++) {

    const auto expw = exp(w);
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

/*

From the paper:
In order to build a well-behaved cascade of wavefolders,
we need to make sure the individual folders satisfy two cri-
teria. Firstly, the individual folders must provide approxi-
mately unity gain when Vin ≈ 0, and approximately nega-
tive unit gain beyond the folding point, when |Vin| >> 0.
Secondly, each stage should start folding at the same point
with respect to its individual input. We can achieve this
with the model described above by appropriate setting of
RL and the addition of static pre- and post-gain stages.
An appropriate RL can be determined empirically. The
pre- and post- gain can be determined by measuring the
value of Vout at exactly the folding point. The pre-gain is
taken to be approximately this value, and the post-gain is
taken to be its inverse. In this case, RL = 7.5 kΩ was
chosen, which leads to pre- and post-gains of approx. 1/4
and 4, respectively.

*/
double LockhartCell::process(double in1) {
  auto out1 = 0.0;

  // Constants
  constexpr auto RL = 7.5e3;
  /* An appropriate RL can be determined empirically. The */
  /* pre- and post- gain can be determined by measuring the */
  /* value of Vout at exactly the folding point. The pre-gain is */
  /* taken to be approximately this value, and the post-gain is */
  /* taken to be its inverse. In this case, RL = 7.5 kΩ was */
  /* chosen, which leads to pre- and post-gains of approx. 1/4 */
  /* and 4, respectively. */

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

  // @FIXME exp becomes infinity when in1 is too high
  auto u = d * exp(l * b * in1);

  /*
  if (std::isnan(u) || std::isinf(u))
    std::cout << "u: " << u << " l: " << l << " b : " << b << " xn1: " << xn1
              << " d : " << d << "in1: "<< in1 << std::endl;
			  */

  auto Ln = lambert_w(u);
  const auto Fn = (0.5 * VT / b) * (Ln * (Ln + 2.0)) - 0.5 * a * in1 * in1;

  // Check for ill-conditioning
  if (std::abs(in1 - xn1) < thresh) {

    // Compute Averaged Wavefolder Output
    const auto xn = 0.5 * (in1 + xn1);

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
