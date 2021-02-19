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
  void setup(float initspeed, float initX, float initY, float initZ);

  void setParameter(Parameter type, float param_value);
  void setSpeed(float speed);

  float getOutput(OutputName outname);

private:
  float x_{0.f}, y_{0.f}, z_{0.f};
  float m_a{42.0f}, m_b{3.f}, m_c{28.f};
  float m_speed{0.f};
};
} // namespace chaos
