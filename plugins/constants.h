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

#define M_TWOPI M_PI * 2.0

#ifndef M_PI_F
#define M_PI_F static_cast<float>(M_PI)
#endif
#define M_PI_POW_2 M_PI *M_PI
#define M_PI_POW_3 M_PI_POW_2 *M_PI
#define M_PI_POW_5 M_PI_POW_3 *M_PI_POW_2
#define M_PI_POW_7 M_PI_POW_5 *M_PI_POW_2
#define M_PI_POW_9 M_PI_POW_7 *M_PI_POW_2
#define M_PI_POW_11 M_PI_POW_9 *M_PI_POW_2

#endif
