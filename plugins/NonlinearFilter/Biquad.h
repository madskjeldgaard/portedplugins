#ifndef BIQUAD_H_INCLUDED
#define BIQUAD_H_INCLUDED

#include "SC_PlugIn.hpp"
#include "Saturators.h"

extern InterfaceTable *ft;

class Biquad {
 public:
  Biquad(World *inWorld) : mWorld(inWorld){};

  void alloc() {
    a = (float *)RTAlloc(mWorld, (order + 1) * sizeof(float));
    b = (float *)RTAlloc(mWorld, (order + 1) * sizeof(float));
    z = (float *)RTAlloc(mWorld, (order + 1) * sizeof(float));
    flush();
  }

  void flush() {
    memset(a, 0, order + 1 * sizeof(float));
    memset(b, 0, order + 1 * sizeof(float));
    memset(z, 0, order + 1 * sizeof(float));
  }

  virtual void free() {
    RTFree(mWorld, a);
    RTFree(mWorld, b);
    RTFree(mWorld, z);
  }

  virtual ~Biquad(){};

  virtual void reset(double sampleRate) {
    // clear coefficients
    for (int n = 0; n < order + 1; ++n) z[n] = 0.0f;

    fs = (float)sampleRate;
  }

  virtual inline float process(float x) {
    // process input sample, direct form II transposed
    float y = z[1] + x * b[0];
    z[1] = saturator(z[2] + x * b[1] - y * a[1]);
    z[2] = saturator(x * b[2] - y * a[2]);

    return y;
  }

  virtual void calcCoefs(float /*fc*/, float /*Q*/) {}

  virtual void processBlock(float *buffer, int numSamples) {
    for (int n = 0; n < numSamples; ++n) buffer[n] = process(buffer[n]);
  }

 protected:
  float fs = 48000.0f;
  float *a;  // IIR Coefficients
  float *b;  // FIR Coefficients
  float *z;  // Filter state
  World *mWorld;
  SatFunc saturator = Saturators::getSaturator(SatType::none);

 private:
  const int order = 2;
};

#endif  // BIQUAD_H_INCLUDED
