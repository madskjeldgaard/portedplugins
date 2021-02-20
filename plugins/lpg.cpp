#include "lpg.hpp"
#include "math.h"
#include "mkutils.hpp"
#include <iostream>

namespace lpg {

// -------------------------------------------
// Low pass gate emulation
// -------------------------------------------
void LowPassGate::setup(float samplerate, float initOffset, float initScale,
                        float initVCA, float initResonance,
                        LpgFilter::Linearity initLinearity) {
  m_samplerate = samplerate;

  // Setup vactrol
  vactrol.setup(samplerate);

  // Setup control circuit
  controlcircuit.setup(initOffset, initScale);

  // Setup filter
  float initResistance = 2500.f;
  filter.setup(samplerate, initResistance, initVCA, initResonance,
               initLinearity);
}

inline void LowPassGate::setControlOffset(float offset) {
  controlcircuit.setOffset(offset);
}

inline void LowPassGate::setControlScale(float scale) {
  controlcircuit.setScale(scale);
}

inline void LowPassGate::setVCAAmount(float amount) {
  filter.setVCAAmount(amount);
}

inline void LowPassGate::setResonance(float resonance) {
  filter.setResonance(resonance);
}

inline void LowPassGate::setLowpassMode(LpgFilter::LowpassProcessing mode) {
  filter.setLowpassMode(mode);
}

inline void LowPassGate::setLinearity(LpgFilter::Linearity linearity) {
  filter.setProcessingMode(linearity);
}

inline void LowPassGate::setOutputNum(LpgFilter::OutputNumber outnum) {
  filter.setOutputNum(outnum);
};

float LowPassGate::process(float sample, float vactrol_in) {
  const float vactrol_output = vactrol.process(vactrol_in);

  const float control_output = controlcircuit.process(vactrol_output * 10.0f);
  filter.setVactrolResistance(control_output);
  /* std::cout << "resistance: " << control_output << std::endl; */
  float filter_output = filter.process(sample);

  return filter_output;
}

// -------------------------------------------
// Low pass gate filter emulation
// -------------------------------------------

void LpgFilter::setup(float samplerate, float initVactrolResistance,
                      float initVCAAmount, float initResonance,
                      Linearity initProcessingMode) {

  m_samplerate = samplerate;
  setVactrolResistance(initVactrolResistance);
  setVCAAmount(initVCAAmount);
  setResonance(initResonance);
  setProcessingMode(initProcessingMode);
}

inline void LpgFilter::setVactrolResistance(float rf) {
  m_vactrol_resistance = rf;
}

inline void LpgFilter::setVCAAmount(float amount) { m_vcaness = amount; }

inline void LpgFilter::setProcessingMode(Linearity mode) {
  m_processing_mode = mode;
}

inline void LpgFilter::setLowpassMode(LowpassProcessing mode) {
  m_lowpass_mode = mode;
}

inline void LpgFilter::setResonance(float resonance) {
  // Not sure why this is necessary to be honest:
  /* resonance = (1.0f - resonance) * 5e6f + 10e3f; */
  m_resonance = resonance;
};

inline void LpgFilter::setOutputNum(OutputNumber outnum) {
  m_output_num = outnum;
};

// Filter
float LpgFilter::process(float sample) {
  float out1, out2, out3;
  float c1 = (1e-09f);
  float c2 = (2.2e-10f);
  float c3 = (4.7e-09f);

  if (m_lowpass_mode == LpgFilter::LowpassProcessing::Off) {
    c3 = 0.f;
  }

  // r1 = 1e3;
  float rf = m_vactrol_resistance;

  // rf = 30e3;
  float r3 = m_vcaness;

  float max_res = 1.0f * (2.0f * c1 * r3 + (c2 + c3) * (r3 + rf)) / (c3 * r3);
  float a = mkutils::constrain(m_resonance, 0.0f, max_res);
  /* float a = m_resonance * max_res; */
  float x = sample;

  float freq = 0.5f / m_samplerate;
  // float freq = 2*pi * (m_vactrol_resistance+1e-3)*0.5/m_samplerate;

  float a1 = 1.0f / (c1 * rf);
  float a2 = -(1.0f / rf + 1.0f / r3) / c1;
  float b1 = 1.0f / (rf * c2);
  float b2 = -2.0f / (rf * c2);
  float b3 = 1.0f / (rf * c2);
  float b4 = c3 / c2;
  float d1 = a;
  float d2 = -1.f;
  m_tanh_xo = tanh(m_state_xo);
  /* std::cout << "tanh_state: " << std::to_string(m_state_xo) << std::endl; */
  // b2 = (a*c3/c1 -1 - 1/(1+r1/rf))/(rf*(c2+c3));			//
  // Extra Feedback resistor b3 =(1/(1+r1/rf)- (1+rf/r3)*a*c3/c1)/(rf*(c2+c3));
  // // Extra Feedback resistor

  float Dx = 1.0f / (1.0f - b2 * freq);
  float Do = 1.0f / (1.0f - a2 * freq);

  float yx, yo, yd;

  /* std::cout << "linearity" << m_processing_mode << std::endl; */
  if (m_processing_mode == NonLinear) {

    float Dmas =
        1.0f /
        (1.0f - Dx * (freq * freq * b3 * Do * a1 +
                      b4 * freq * d1 * (1.0f - pow(m_tanh_xo, 2.0f)) * Do * a1 +
                      b4 * d2));

    yx = (m_state_x + freq * b1 * x + freq * b3 * Do * m_state_o +
          freq * b4 *
              (m_state_d +
               (1.0f / freq) * d1 *
                   (m_tanh_xo - m_state_xo * (1.0f - pow(m_tanh_xo, 2.0f)))) +
          b4 * d1 * (1.0f - pow(m_tanh_xo, 2.0f)) * Do * m_state_o) *
         Dx * Dmas;

    yo = (m_state_o + freq * a1 * yx) * Do;

    yd = (m_state_d +
          (1.0f / freq) * d1 *
              (m_tanh_xo - m_state_xo * (1.0f - pow(m_tanh_xo, 2.0f)))) +
         (1.0f / freq) * (d1 * ((1.0f - pow(m_tanh_xo, 2.0f))) * yo + d2 * yx);

    m_state_x =
        m_state_x + 2.0f * freq * (b1 * x + b2 * yx + b3 * yo + b4 * yd);

    m_state_o = m_state_o + 2.0f * freq * (a1 * yx + a2 * yo);

    m_state_d =
        -(m_state_d +
          (2.0f / freq) * d1 *
              (m_tanh_xo - m_state_xo * (1.0f - pow(m_tanh_xo, 2.0f)))) -
        (2.0f / freq) * (d1 * (1.0f - pow(m_tanh_xo, 2.0f)) * yo + d2 * yx);

    // Fixme: Is this correct?
    m_state_xo = yo;

  } else {
    float Dmas = 1.0f / (1.0f - Dx * (freq * freq * b3 * Do * a1 +
                                      b4 * freq * d1 * Do * a1 + b4 * d2));

    yx = (m_state_x + freq * b1 * x + freq * b3 * Do * m_state_o +
          freq * b4 * m_state_d + b4 * d1 * Do * m_state_o) *
         Dx * Dmas;

    yo = (m_state_o + freq * a1 * yx) * Do;

    yd = m_state_d + (1.0f / freq) * (d1 * yo + d2 * yx);

    m_state_x =
        m_state_x + 2.0f * freq * (b1 * x + b2 * yx + b3 * yo + b4 * yd);

    m_state_o = m_state_o + 2.0f * freq * (a1 * yx + a2 * yo);

    m_state_d = -m_state_d - (2.0f / freq) * (d1 * yo + d2 * yx);
  }

  /* std::cout << "out1: " << out1 << std::endl; */
  /* std::cout << "out2: " << out2 << std::endl; */
  /* std::cout << "out3: " << out3 << std::endl; */
  /* std::cout << "rf: " << rf << std::endl; */

  out1 = yo;
  out2 = yx;
  out3 = yd;

  if (m_output_num == LpgFilter::Out3) {
    return out3;
  } else if (m_output_num == LpgFilter::Out2) {
    return out2;
  } else {
    return out1;
  }
}

// -------------------------------------------
// Vactrol emulation
// -------------------------------------------

void LpgVactrol::setup(float samplerate) { m_samplerate = samplerate; }

// Vactrol emulation, process an input voltage
float LpgVactrol::process(float sample) {

  // Vactrol time-domain behaviour filter
  float a_base = 1000.0f * M_PI / (m_samplerate);
  float t_down = 3e3; // Fall time
  t_down = 10.0f + t_down * (1.0f - 0.9f * m_state_1);
  float a_down = a_base / t_down;
  float t_up = 20.0f; // Rise time
  t_up = 1.0f + t_up * (1.0f - 0.999f * m_state_1);
  float a_up = a_base / t_up;

  float dm_state_1 = sample - m_state_1;

  float x = (sample - m_state_1) * a_up / (1.0f + a_up);
  if (dm_state_1 < 0.0f) {
    x = (sample - m_state_1) * a_down / (1.0f + a_down);
  }

  float y = x + m_state_1;
  m_state_1 = y + x;
  float out1 = y;

  return out1;
}

// -------------------------------------------
// Vactrol control circuit emulation
// -------------------------------------------

void LpgControlCircuit::setup(float initOffset, float initScale) {
  setOffset(initOffset);
  setScale(initScale);
}

void LpgControlCircuit::setOffset(float offset) { m_offset = offset; }
void LpgControlCircuit::setScale(float scale) { m_scale = scale; }

float LpgControlCircuit::process(float sample) {
  // Inputs
  float Vb = mkutils::constrain(sample, -10.0f, 50.0f);
  float offset = 0.9999f * m_offset + 0.0001f;
  float scale = mkutils::constrain(m_scale, 0.0f, 1.0f);
  // FIXME:
  /* float scale = 0.48f; // This value is tuned for appropriate input range. */
  /* std::cout << "voltage base: " << Vb << std::endl; */

  // Constants
  const float A = 3.4645912f;
  const float B = 1136.2129956f;
  const float Ifmin = 10.1e-6f;
  const float Ifmax = 40e-3f;
  const float R2max = 10e3f;
  const float R6max = 20e3f;
  const float R7 = 33e3f;
  const float R3 = 150e3f;
  const float R5 = 100e3f;
  const float R4 = 470e3f;
  const float R8 = 4.7e3f;
  const float R9 = 470.f;
  const float VB = 3.9f;
  const float VF = 0.7f;
  const float VT = 26e-3f;
  const float n = 3.9696f;
  const float kl = 6.3862f;
  const float G = 2e5f;
  const float Vs = 15.f;
  const float k0 = 1.468e2f;
  const float k1 = 4.9202e-1f;
  const float k2 = 4.1667e-4f;
  const float k3 = 7.3915e-9f;
  const float gamma = 0.0001f;

  const float R6 = scale * R6max;
  const float R1 = (1.f - offset) * R2max;
  const float R2 = offset * R2max;

  float alpha = 1.0f + (R6 + R7) * (1.0f / R3 + 1.0f / R5);
  float beta = ((1.0f / alpha) - 1.0f) / (R6 + R7) - 1.0f / R8;

  float bound1 =
      600.f * alpha * n * VT / (G * (R6 + R7 - 1.f / (alpha * beta)));

  float Ia = Vb / R5 + Vs / (R3 * (1.f + R1 / R2));

  float V3 = 0.f;
  float If = 0.f;
  float flag = 0.f;
  if (Ia <= -bound1) {
    V3 = -Ia / (alpha * beta);
  } else if (Ia < bound1) {
    const float x =
        G * Ia * (R6 + R7 - 1.f / (alpha * beta)) / (alpha * n * VT);
    const float w = k0 + k1 * x + k2 * pow(x, 2.f) + k3 * pow(x, 3.f);
    V3 = -(alpha / G) * n * VT * w - Ia / (alpha * beta);
  } else {
    V3 = kl * alpha / G * n * VT - Ia * (R6 + R7);
  }

  float Ifbound1 = alpha * (Ifmin - beta * V3);
  float Ifbound2 = VB / (R6 + R7);
  float Ifbound3 = (gamma * G * VB + alpha * R9 * (VB * beta + Ifmax)) /
                   (gamma * G * (R6 + R7) + R9);

  if (Ia <= Ifbound1) {
    If = Ifmin;
  } else if (Ia <= Ifbound2) {
    If = beta * V3 + Ia / alpha;
  } else if (Ia <= Ifbound3) {
    If = gamma * G * (Ia * (R6 + R7) - VB) / (alpha * R9) - beta * VB +
         Ia / alpha;
  } else {
    If = Ifmax;
  }

  float Rf = B + A / pow(If, 1.4f);
  float out = Rf;

  return out;
}

} // namespace lpg
