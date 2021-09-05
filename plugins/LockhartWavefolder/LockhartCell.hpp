#pragma once
#ifndef NODSP_MK_Lockhart
#define NODSP_MK_Lockhart
#include "../mkutils.hpp"
#include "SC_InlineUnaryOp.h"

class LockhartCell {
public:
  double process(double in1);

private:
  double lambert_w(double x);
  double Ln1{0.0}, Fn1{0.0}, xn1{0.0};
};
#endif
