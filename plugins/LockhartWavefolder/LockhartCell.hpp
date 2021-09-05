#pragma once
#ifndef NODSP_MK_Lockhart
#define NODSP_MK_Lockhart
#include "../mkutils.hpp"

class LockhartCell {
public:
  float process(float in1);

private:
  float lambert_w(float x);
  double Ln1{0.0}, Fn1{0.0}, xn1{0.0};
};
#endif
