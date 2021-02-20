#pragma once

namespace lpg {
// A low pass gate filter model
class LpgFilter {
public:
  enum Linearity { Linear, NonLinear };
  enum LowpassProcessing { Off, On };
  enum OutputNumber { Out1, Out2, Out3 };

  float process(float sample);
  void setup(float samplerate, float initVactrolResistance = 2500.0f,
             float initVCAAmount = 1.0f, float initResonance = 0.9f,
             Linearity initProcessingMode = NonLinear);
  inline void setVactrolResistance(float resistance);
  inline void setVCAAmount(float amount);
  inline void setResonance(float resonance);
  inline void setProcessingMode(Linearity mode);
  inline void setLowpassMode(LowpassProcessing mode);
  inline void setOutputNum(OutputNumber outnum);

private:
  // Parameters
  float m_vactrol_resistance{0.f}, m_offset{0.1f}, m_vcaness{0.f},
      m_resonance{0.f}, m_samplerate{0.f};

  Linearity m_processing_mode{Linear};
  LowpassProcessing m_lowpass_mode{On};
  OutputNumber m_output_num{Out1};

  // Filter outputs
  float m_y_x{0.f}, m_y_o{0.f}, m_y_d{0.f};

  // States
  float m_state_x{0.f}, m_state_o{0.f}, m_state_d{0.f}, m_state_xo{0.f},
      m_tanh_xo{0.f};
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
  void setup(float initOffset, float initScale);

  // Set frequency offset
  void setOffset(float offset);
  void setScale(float scale);

private:
  // parameters
  float m_scale{0.5f}, m_offset{0.5f};

  // States
  float m_state_1{0.f};
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
  inline void setVCAAmount(float amount);
  inline void setResonance(float resonance);
  inline void setControlOffset(float offset);
  inline void setControlScale(float scale);
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
