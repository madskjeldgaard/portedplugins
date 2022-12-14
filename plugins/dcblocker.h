#pragma once

// Based on
// https://ccrma.stanford.edu/~jos/filters/DC_Blocker_Software_Implementations.html
namespace dcblocker {
class Dcblocker {
 public:
  void init() {
    xm1 = 0.0;
    ym1 = 0.0;

    r = 0.995f;
  };

  float process(float sample) {
    float y = sample - xm1 + r * ym1;
    xm1 = sample;
    ym1 = y;
    return y;
  };

  void setR(float newR) { r = newR; }

 private:
  float xm1, ym1, r;
};
}  // namespace dcblocker
