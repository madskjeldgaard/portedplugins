#ifndef LDR_H_INCLUDED
#define LDR_H_INCLUDED

#include <cmath>
#include "../mkutils.hpp"
namespace chow {

class LDR {
public:
  static inline float getLDRResistance(float lfo, float skew) noexcept {
    constexpr float maxDepth = 20.0f;
    const auto skewVal = std::pow(2.0f, skew);
    const auto lfoVal = lightShape(lfo, skewVal);
    const auto lightVal = (maxDepth + 0.1f) - (lfoVal * maxDepth);

    return 100000.0f * std::pow(lightVal / 0.1f, -0.75f);
  }

private:
  LDR() {} // you can't create an instance of this class

  // reshape an LFO with some skew power
  static inline float lightShape(float x, float skewPow) noexcept {
    x = mkutils::constrain(x, -1.0f, 1.0f);
    return (std::pow((x + 1.0f) / 2.0f, skewPow) * 2.0f) - 1.0f;
  }
};

} // namespace chow
#endif // LDR_H_INCLUDED
