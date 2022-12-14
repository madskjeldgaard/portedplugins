#include "LockhartCell.hpp"

double LockhartCell::lambert_w(double x) {
  // Initial guess (use previous value)
  auto w = Ln1;

  // Haley's method (Sec. 4.2 of the paper)
  for (int i = 0; i < 1000; i++) {
    const auto expw = exp(w);
    const auto p = w * expw - x;
    const auto r = (w + 1.0) * expw;
    const auto s = (w + 2.0) / (2.0 * (w + 1.0));
    const auto err = (p / (r - (p * s)));

    if (std::abs(err) < m_thresh) {
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

  // Compute Antiderivative
  const auto l = mkutils::sign(in1);
  auto u = m_d * exp(l * m_b *
                     in1);  // @FIXME exp becomes infinity when in1 is too high
  auto Ln = lambert_w(u);
  const auto Fn =
      (0.5 * m_VT / m_b) * (Ln * (Ln + 2.0)) - 0.5 * m_a * in1 * in1;

  // Check for ill-conditioning
  if (std::abs(in1 - xn1) < m_thresh) {
    // Compute Averaged Wavefolder Output
    const auto xn = 0.5 * (in1 + xn1);

    u = m_d * exp(l * m_b * xn);

    Ln = lambert_w(u);
    out1 = l * m_VT * Ln - m_a * xn;

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
