#pragma once

namespace mkutils {

// Constrain a number between a minimum and a maxmimum
template <class Number> Number inline constrain(Number input, Number min, Number max) {
  if (input > max) {
    return max;
  } else if (input < min) {
    return min;
  } else {
    return input;
  }
}
} // namespace mkutils
