#pragma once

namespace lpg {
// A low pass gate filter model
class LpgFilter {
public:
  enum Linearity { Linear, NonLinear };
  enum LowpassProcessing { Off, On };
  enum OutputNumber { Vout, VactrolOut, Differentiator };

  float process(float sample);
  void setup(float samplerate, float initVactrolResistance = 2500.0f,
             float initVCAAmount = 1.0f, float initResonance = 0.9f,
             Linearity initProcessingMode = NonLinear);
  inline void setVactrolResistance(float resistance);
  inline void setVCAAmount(double amount);
  inline void setResonance(double resonance);
  inline void setProcessingMode(Linearity mode);
  inline void setLowpassMode(LowpassProcessing mode);
  inline void setOutputNum(OutputNumber outnum);

private:
  // Parameters
  double m_vactrol_resistance{0.}, m_offset{0.1}, m_vcaness{0.},
      m_resonance{0.}, m_samplerate{0.};

  Linearity m_processing_mode{Linear};
  LowpassProcessing m_lowpass_mode{On};
  OutputNumber m_output_num{Vout};

  // States
  double m_state_x{0.}, m_state_o{0.}, m_state_d{0.}, m_state_xo{0.},
      m_tanh_xo{0.};
};

// Models the behaviour of a Vactrol that takes a voltage as input and then
// sluggishly outputs another voltage
class LpgVactrol {
public:
  float process(float sample);
  void setup(float samplerate);

private:
  float m_samplerate{0.};

  // States
  float m_state_1{0.f};
};

// Used with LpgVactrol, this will calculate the resistance/frequency control
// signal portion of the vactrol part of the LPG It models the nonlinearity in
// the resistance of the vactrol as if it takes a voltage (V_b) as input and
// returns R_F (vactrol Resistance)
class LpgControlCircuit {
public:
  // Calculate resistance/frequency from voltage input
  float process(float sample);
  void setup(double initOffset, double initScale);

  // Set frequency offset
  void setOffset(double offset);
  void setScale(double scale);

private:
  // parameters
  double m_scale{0.5}, m_offset{0.5};

  // States
  double m_state_1{0.};
};

/*
 * A Buchla low pass gate model based on the work of Julian Parker and Stefano
 * D'angelo - "A digital model of the Buchla Lowpass-gate", DAFX-13, 2013:
 * https://www.dafx.de/paper-archive/2013/papers/44.dafx2013_submission_56.pdf
 */
class LowPassGate {
public:
  void setup(float samplerate, float initOffset = 0.f, float initScale = 0.f,
             float initVCA = 0.5f, float initResonance = 0.5f,
             LpgFilter::Linearity initLinearity = LpgFilter::NonLinear);

  // Process a sound sample using the LPG model.
  // sample is a sound sample, vactrol_in is the control signal "pinging" the
  // vactrol. This may be a gate signal or an envelope of sorts.
  float process(float sample, float vactrol_in);
  inline void setVCAAmount(double amount);
  inline void setResonance(double resonance);
  inline void setControlOffset(double offset);
  inline void setControlScale(double scale);
  inline void setLowpassMode(LpgFilter::LowpassProcessing mode);
  inline void setLinearity(LpgFilter::Linearity linearity);
  inline void setOutputNum(LpgFilter::OutputNumber outnum);

private:
  // Parameters
  float m_samplerate{0.f};

  // Components
  LpgControlCircuit controlcircuit;
  LpgVactrol vactrol;
  LpgFilter filter;
};

} // namespace lpg
