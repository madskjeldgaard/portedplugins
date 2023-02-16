// AnalogChew.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogChew.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogChew {

AnalogChew::AnalogChew() {
  samplerate = sampleRate();

  filt.reset(samplerate, int(samplerate * 0.02f));
  dropout.prepare((double)samplerate);
  cookParams(samplerate, 0.5, 0.5, 0.5);

  mCalcFunc = make_calc_function<AnalogChew, &AnalogChew::next>();
  next(1);
}

AnalogChew::~AnalogChew() {}

float AnalogChew::uniform() { return ((float)rand() / (RAND_MAX)); }

void AnalogChew::cookParams(float fs, float depthParam, float freqParam,
                            float varParam) {
  const float highFreq = std::min(22000.0f, 0.49f * fs);
  const float freqChange = highFreq - 5000.0f;

  auto depth = depthParam;
  auto freq = freqParam;
  if (freq == 0.0f) {
    mix = 0.0f;
    filt.setFreq(highFreq);
  } else if (freq == 1.0f) {
    mix = 1.0f;
    power = 3.0f * depth;
    filt.setFreq(highFreq - freqChange * depth);
  } else if (sampleCounter >= samplesUntilChange) {
    sampleCounter = 0;
    isCrinkled = !isCrinkled;

    if (isCrinkled)  // start crinkle
    {
      mix = 1.0f;
      power = (1.0f + 2.0f * uniform()) * depth;
      filt.setFreq(highFreq - freqChange * depth);
      samplesUntilChange = getWetTime(freq, depth, varParam);
    } else  // end crinkle
    {
      mix = 0.0f;
      filt.setFreq(highFreq);
      samplesUntilChange = getDryTime(freq, varParam);
    }
  } else {
    power = (1.0f + 2.0f * uniform()) * depth;
    if (isCrinkled) {
      filt.setFreq(highFreq - freqChange * depth);
      filt.setFreq(highFreq - freqChange * depth);
    }
  }

  dropout.setMix(mix);
  dropout.setPower(1.0f + power);
}

void AnalogChew::next(int nSamples) {
  float depth = in0(Depth);
  float freq = in0(Frequency);
  float var = in0(Variance);

  cookParams(samplerate, depth, freq, var);

  const float *input = in(Input);
  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    // get input
    float x = mkutils::constrain(input[i], -1.0f, 1.0f);
    x = dropout.processSample(x);
    x = filt.processSample(x);
    outbuf[i] = x;
    sampleCounter++;
  }
}

void AnalogChew::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace AnalogChew

PluginLoad(AnalogChewUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogChew::AnalogChew>(ft, "AnalogChew", false);
}
