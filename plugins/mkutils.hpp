#pragma once

#include <cmath>
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

// Flush nan and inf from number to 0
template <class Number> Number inline flushed(Number input){
	if(!std::isfinite(input)){
		return 0.0;
	}

	return input;
}
} // namespace mkutils
