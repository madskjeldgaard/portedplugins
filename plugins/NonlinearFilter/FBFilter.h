#ifndef FBFILTER_H_INCLUDED
#define FBFILTER_H_INCLUDED

#include "EQFilter.h"
#include "Saturators.h"

class FBFilter : public EQFilter {
 public:
  FBFilter(World *worldIn, float sampleRate) : EQFilter(worldIn, sampleRate) {}
  virtual ~FBFilter() {}

  inline float process(float x) override {
    // process input sample, direct form II transposed
    float y = z[1] + x * b[0];
    z[1] = z[2] + x * b[1] - saturator(y) * a[1];
    z[2] = x * b[2] - saturator(y) * a[2];

    return y;
  }

 private:
};

#endif  // FBFILTER_H_INCLUDED
