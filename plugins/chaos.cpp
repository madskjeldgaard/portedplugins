#include "chaos.hpp"
/* #include <iostream> */

namespace chaos {

void ChenGen::setup(float initspeed, float initX, float initY, float initZ) {
  setSpeed(initspeed);
  // Set initial state
  x_ = initX;
  y_ = initY;
  z_ = initZ;
}

// Set "speeduency", that is speed of the oscillators
void ChenGen::setSpeed(float speed) {
  m_speed = mkutils::constrain(speed, 0.0f, 1.0f);
}
void ChenGen::setParameter(Parameter type, float param_value) {
  if (type == A) {
    m_a = param_value;
  } else if (type == B) {
    m_b = param_value;
  } else if (type == C) {
    m_c = param_value;
  };
}

float ChenGen::getOutput(OutputName outname) {
  float output;
  if (outname == X) {
    output = x_;
  } else if (outname == Y) {
    output = y_;
  } else if (outname == Z) {
    output = z_;
  } else {
    return 0.f;
  }

  output = (output + 18.0f) / 36.0f;
  output = mkutils::constrain(output, 0.0f, 1.0f);

  return output;
};

float ChenGen::process() {
  const float minspeed = 0.0f;
  const float maxspeed = 0.0090f;
  float speed = m_speed * maxspeed;

  const float a = m_a;
  const float max_b = 6.0f;
  const float min_b = 1.0f;
  /* const float b = ((max_b - min_b) * m_b + min_b); */
  const float b = m_b;
  const float c = m_c;

  float x = x_;
  float y = y_;
  float z = z_;

  // Right now, behavior changes a good bit with dt. Could try runge-kutta to
  // fix
  const float dx = a * (y - x);
  const float dy = (c - a) * x - x * z + c * y;
  const float dz = x * y - b * z;
  x += speed * dx;
  y += speed * dy;
  z += speed * dz;

  float output = (x + 18.0f) / 36.0f;
  output = mkutils::constrain(output, 0.0f, 1.0f);

  x_ = x;
  y_ = y;
  z_ = z;

  return output;
}

} // namespace chaos
