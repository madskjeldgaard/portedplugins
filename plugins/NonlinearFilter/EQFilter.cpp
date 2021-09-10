#include "EQFilter.h"

/*
* @TODO
* - Replace the fast math approximations of tan, sin and cos that were here before from juce with something else
*
*/

enum {
  smoothSteps = 500,
};

EQFilter::EQFilter(World *worldIn, float sampleRate) : Biquad(worldIn) {
  freq = 250.0;
  Q = 1;
  gain = 1;
  fs = sampleRate;
  calcCoefs = [this](float fc, float Q, float gain) {
    calcCoefsBell(fc, Q, gain);
  };
};

void EQFilter::setFrequency(float newFreq) {
  if (newFreq != freq)
    freq = std::min(newFreq, fs / 2.0f - 100.0f);
}

void EQFilter::setQ(float newQ) {
  if (newQ != Q)
    Q = newQ;
}

void EQFilter::setGain(float newGain) {
  if (newGain != gain)
    gain = newGain;
}

void EQFilter::toggleOnOff(bool shouldBeOn) {
  if (isOn != shouldBeOn)
    isChanging = true;
  else
    isChanging = false;
}

void EQFilter::setOrder(float newOrder) { filterOrd = newOrder; }

void EQFilter::setSaturator(SatType type) {
  saturator = Saturators::getSaturator(type);
}

void EQFilter::setEqShape(EqShape newShape) {
  if (eqShape == newShape)
    return;

  eqShape = newShape;

  switch (eqShape) // Set calcCoefs lambda to correct function for this shape
  {
  case bell:
    calcCoefs = [this](float fc, float Q, float gain) {
      calcCoefsBell(fc, Q, gain);
    };
    break;

  case notch:
    calcCoefs = [this](float fc, float Q, float gain) {
      calcCoefsNotch(fc, Q, gain);
    };
    break;

  case lowShelf:
    calcCoefs = [this](float fc, float Q, float gain) {
      calcCoefsLowShelf(fc, Q, gain);
    };
    break;

  case highShelf:
    calcCoefs = [this](float fc, float Q, float gain) {
      calcCoefsHighShelf(fc, Q, gain);
    };
    break;

  case lowPass:
    calcCoefs = [this](float fc, float Q, float gain) {
      calcCoefsLowPass(fc, Q, gain);
    };
    break;

  case highPass:
    calcCoefs = [this](float fc, float Q, float gain) {
      calcCoefsHighPass(fc, Q, gain);
    };
    break;

  default:
    return;
  }

  calcCoefs(freq, Q, gain);
}

void EQFilter::reset(double sampleRate) {
  Biquad::reset(sampleRate);

  calcCoefs(freq, Q, gain);
  fs = (float)sampleRate;
}

void EQFilter::calcCoefsBell(float newFreq, float newQ, float newGain) {
  float wc = twopi * newFreq / fs;
  float c = 1.0f / tan(wc / 2.0f);
  float phi = c * c;
  float Knum = c / newQ;
  float Kdenom = Knum;

  if (newGain > 1.0f)
    Knum *= newGain;
  else if (newGain < 1.0f)
    Kdenom /= newGain;

  float a0 = phi + Kdenom + 1.0f;

  b[0] = (phi + Knum + 1.0f) / a0;
  b[1] = 2.0f * (1.0f - phi) / a0;
  b[2] = (phi - Knum + 1.0f) / a0;

  a[1] = 2.0f * (1.0f - phi) / a0;
  a[2] = (phi - Kdenom + 1.0f) / a0;
}

void EQFilter::calcCoefsNotch(float newFreq, float newQ, float /*newGain*/) {
  float wc = twopi * newFreq / fs;
  float wS = sin(wc);
  // @TODO: Replace with fast approximation
  float wC = cos(wc);
  float alpha = wS / (2.0f * newQ);

  float a0 = 1.0f + alpha;

  b[0] = 1.0f / a0;
  b[1] = -2.0f * wC / a0;
  b[2] = 1.0f / a0;

  a[1] = -2.0f * wC / a0;
  a[2] = (1.0f - alpha) / a0;
}

void EQFilter::calcCoefsLowShelf(float newFreq, float newQ, float newGain) {
  float A = sqrtf(newGain);
  float wc = twopi * newFreq / fs;
  // @TODO: Replace with fast approximation
  float wS = sin(wc);
  float wC = cos(wc);

  float beta = sqrtf(A) / newQ;

  float a0 = ((A + 1.0f) + ((A - 1.0f) * wC) + (beta * wS));

  b[0] = A * ((A + 1.0f) - ((A - 1.0f) * wC) + (beta * wS)) / a0;
  b[1] = 2.0f * A * ((A - 1.0f) - ((A + 1.0f) * wC)) / a0;
  b[2] = A * ((A + 1.0f) - ((A - 1.0f) * wC) - (beta * wS)) / a0;

  a[1] = -2.0f * ((A - 1.0f) + ((A + 1.0f) * wC)) / a0;
  a[2] = ((A + 1.0f) + ((A - 1.0f) * wC) - (beta * wS)) / a0;
}

void EQFilter::calcCoefsHighShelf(float newFreq, float newQ, float newGain) {
  float A = sqrtf(newGain);
  float wc = twopi * newFreq / fs;
  float wS = sin(wc);
  float wC = cos(wc);
  float beta = sqrtf(A) / newQ;

  float a0 = ((A + 1.0f) - ((A - 1.0f) * wC) + (beta * wS));

  b[0] = A * ((A + 1.0f) + ((A - 1.0f) * wC) + (beta * wS)) / a0;
  b[1] = -2.0f * A * ((A - 1.0f) + ((A + 1.0f) * wC)) / a0;
  b[2] = A * ((A + 1.0f) + ((A - 1.0f) * wC) - (beta * wS)) / a0;

  a[1] = 2.0f * ((A - 1.0f) - ((A + 1.0f) * wC)) / a0;
  a[2] = ((A + 1.0f) - ((A - 1.0f) * wC) - (beta * wS)) / a0;
}

void EQFilter::calcCoefsLowPass(float newFreq, float newQ, float /*newGain*/) {
  float wc = twopi * newFreq / fs;

  float c = 1.0f / tan(wc / 2.0f);
  float phi = c * c;
  float K = c / newQ;
  float a0 = phi + K + 1.0f;

  b[0] = 1.0f / a0;
  b[1] = 2.0f * b[0];
  b[2] = b[0];
  a[1] = 2.0f * (1.0f - phi) / a0;
  a[2] = (phi - K + 1.0f) / a0;
}

void EQFilter::calcCoefsHighPass(float newFreq, float newQ, float /*newGain*/) {
  float wc = twopi * newFreq / fs;
  float c = 1.0f / tan(wc / 2.0f);
  float phi = c * c;
  float K = c / newQ;
  float a0 = phi + K + 1.0f;

  b[0] = phi / a0;
  b[1] = -2.0f * b[0];
  b[2] = b[0];
  a[1] = 2.0f * (1.0f - phi) / a0;
  a[2] = (phi - K + 1.0f) / a0;
}

/* void EQFilter::processBlock (float* buffer, int numSamples) */
/* { */
/*     if (isOn && ! isChanging) // normal operation */
/*     { */
/*         for (int n = 0; n < numSamples; n++) */
/*         { */
/*             if (freq.isSmoothing() || Q.isSmoothing() || gain.isSmoothing())
 */
/*                 calcCoefs (freq.getNextValue(), Q.getNextValue(),
 * gain.getNextValue()); */
/*             buffer[n] = process (buffer[n]); */
/*         } */
/*     } */
/*     else if (isChanging && ! isOn) // turning on */
/*     { */
/*         reset (fs); */

/*         for (int n = 0; n < numSamples; n++) */
/*         { */
/*             if (freq.isSmoothing() || Q.isSmoothing() || gain.isSmoothing())
 */
/*                 calcCoefs (freq.getNextValue(), Q.getNextValue(),
 * gain.getNextValue()); */

/*             buffer[n] = process (buffer[n]) * (float (n) / float
 * (numSamples)) */
/*                       + buffer[n] * (1.0f - (float (n) / float
 * (numSamples))); */
/*         } */

/*         isOn = true; */
/*         isChanging = false; */
/*     } */
/*     else if (isChanging && isOn) // turning off */
/*     { */
/*         for (int n = 0; n < numSamples; n++) */
/*         { */
/*             if (freq.isSmoothing() || Q.isSmoothing() || gain.isSmoothing())
 */
/*                 calcCoefs (freq.getNextValue(), Q.getNextValue(),
 * gain.getNextValue()); */

/*             buffer[n] = process (buffer[n]) * (1.0f - (float (n) / float
 * (numSamples))) */
/*                       + buffer[n] * (float (n) / float (numSamples)); */
/*         } */

/*         isOn = false; */
/*         isChanging = false; */
/*         reset (fs); */
/*     } */
/* } */

void EQFilter::applyFilterGain(float *buffer, int numSamples) {
  for (int n = 0; n < numSamples; ++n)
    buffer[n] *= gain;
}
