#include "chaos.hpp"
/* #include <iostream> */

namespace chaos {

void ChenGen::setup(float initspeed, float initX, float initY, float initZ,
                    float initA = 0.5f, float initB = 0.5f,
                    float initC = 1.0f) {

  setSpeed(initspeed);
  setParameter(A, initA);
  setParameter(B, initB);
  setParameter(C, initC);

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

  /*
   *
   * The speed, a, b and c parameters of the algorithm are wrapped to values
   * that seem to keep it moving, the exact borders of which were found by
   * experimentation. This way, the user may put in a value between 0.0 and 1.0
   * and achieve nice results.
   *
   */

  const float minspeed = 0.0f;
  const float maxspeed = 0.0090f;
  float speed = m_speed * maxspeed;

  // Calculate a
  const float min_a = 32.f;
  const float max_a = 48.f;
  const float a = ((max_a - min_a) * m_a + min_a);

  // Calculate b
  const float max_b = 6.0f;
  const float min_b = 1.0f;
  const float b = ((max_b - min_b) * m_b + min_b);

  /* const float b = m_b; */
  /* const float c = m_c; */

  // Calculate c
  const float max_c = 24.0f;
  const float min_c = 28.0f;
  const float c = ((max_c - min_c) * m_c + min_c);

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

  x_ = mkutils::flushed(x);
  y_ = mkutils::flushed(y);
  z_ = mkutils::flushed(z);

  return getOutput(X);
}

} // namespace chaos
