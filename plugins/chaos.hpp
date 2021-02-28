#pragma once
#include "mkutils.hpp"

namespace chaos {

// An implementation of Chen's double scroll chaotic attractor, largely based on
// Bryan Head's code for the alternative firmware for the Mutable Instruments
// Stages
class ChenGen {
public:
  enum Parameter { A, B, C };
  enum OutputName { X, Y, Z };

  float process();
  void setup(float initspeed, float initX, float initY, float initZ,
             float initA, float initB, float initC);

  inline void setSpeed(float speed);
  inline float getOutput(OutputName outname);
  inline void setParameter(Parameter type, float param_value);

private:
  float x_{0.f}, y_{0.f}, z_{0.f};
  float m_a{0.2f}, m_b{0.2f}, m_c{28.f};
  float m_speed{0.f};
};

// Based on
// https://nathanselikoff.com/training/tutorial-strange-attractors-in-c-and-opengl
class KingsDreamGen {
public:
  void setup(float initX = 0.1f, float initY = 0.1f);
  float process();
  float getX();
  float getY();

private:
  float m_x{0.0f}, m_y{0.0f}, m_x_state{0.1f}, m_y_state{0.1f};
};

class ChuaGen {
public:
  void setup(float initX = 0.7f, float initY = 0.0f, float initZ = 0.0f);
  float process();
  float getX();
  float getY();
  float getZ();

private:
  float m_x{0.f}, m_y{0.f}, m_z{0.f};
  float m_x_state{0.f}, m_y_state{0.f}, m_z_state{0.f};
};

} // namespace chaos
