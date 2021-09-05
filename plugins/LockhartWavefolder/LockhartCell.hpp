#pragma once
#ifndef NODSP_MK_Lockhart
#define NODSP_MK_Lockhart
#include "../mkutils.hpp"
#include "SC_InlineUnaryOp.h"

class LockhartCell {
public:
  /* An appropriate RL can be determined empirically. The */
  /* pre- and post- gain can be determined by measuring the */
  /* value of Vout at exactly the folding point. The pre-gain is */
  /* taken to be approximately this value, and the post-gain is */
  /* taken to be its inverse. In this case, RL = 7.5 kΩ was */
  /* chosen, which leads to pre- and post-gains of approx. 1/4 */
  /* and 4, respectively. */
  LockhartCell(double loadResistance = 7.5e3) {
    // Constants
    m_RL = loadResistance;
    m_R = 15e3;
    m_VT = 26e-3;
    m_Is = 10e-16;

    m_a = 2.0 * m_RL / m_R;
    m_b = (m_R + 2.0 * m_RL) / (m_VT * m_R);
    m_d = (m_RL * m_Is) / m_VT;

    // Antialiasing error threshold
    m_thresh = 10e-10;

    Ln1 = 0.0;
    Fn1 = 0.0;
    xn1 = 0.0;
  }

  double process(double in1);

private:
  double lambert_w(double x);
  double Ln1, Fn1, xn1;
  double m_RL, m_R, m_VT, m_Is, m_a, m_b, m_d, m_thresh;
};
#endif
