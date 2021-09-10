#ifndef EQFILTER_H_INCLUDED
#define EQFILTER_H_INCLUDED

#include "Biquad.h"

enum EqShape {
  bell,
  notch,
  highShelf,
  lowShelf,
  highPass,
  lowPass,
};

/* Filter for a single EQ band */
class EQFilter : public Biquad {
public:
  EQFilter(World *worldIn, float sampleRate );
  virtual ~EQFilter() { }

  void setFrequency(float newFreq);
  void setQ(float newQ);
  void setGain(float newGain);
  void toggleOnOff(bool shouldBeOn);
  void setEqShape(EqShape newShape);
  void setOrder(float newOrder);
  void setSaturator(SatType type);

  /* Calculate filter coefficients for an EQ band (see "Audio EQ Cookbook") */
  void calcCoefsBell(float newFreq, float newQ, float newGain);
  void calcCoefsNotch(float newFreq, float newQ, float newGain);
  void calcCoefsLowShelf(float newFreq, float newQ, float newGain);
  void calcCoefsHighShelf(float newFreq, float newQ, float newGain);
  void calcCoefsLowPass(float newFreq, float newQ, float newGain);
  void calcCoefsHighPass(float newFreq, float newQ, float newGain);

  /* void processBlock (float* buffer, int numSamples) override; */
  void applyFilterGain(float *buffer, int numSamples);
  void reset(double sampleRate) override;

  typedef std::function<void(float, float, float)> CalcCoefsLambda;
  CalcCoefsLambda calcCoefs =
      {}; // lambda function to calculate coefficients for any shape
private:
  float freq, Q, gain;
  float filterOrd = 1;

  EqShape eqShape = bell;

  bool isOn = true;
  bool isChanging = false;

  float fs = 48000.0f;
};

#endif // EQFILTER_H_INCLUDED
