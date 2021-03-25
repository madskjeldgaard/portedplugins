#pragma once

// Fucking Windows bollocks...

#include <cmath>
#ifndef MK_CONSTANTS
#define MK_CONSTANTS

// This is necessary to get M_PI to work on windows
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

#endif
