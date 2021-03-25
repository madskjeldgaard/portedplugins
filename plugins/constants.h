#pragma once
#include <cmath>
#ifndef MK_CONSTANTS
#define MK_CONSTANTS

// This is necessary to get M_PI to work on windows
#ifndef M_PI
namespace {
const double M_PI = std::acos(-1.0);
}
#endif

#endif
