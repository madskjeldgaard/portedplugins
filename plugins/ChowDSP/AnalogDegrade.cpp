// AnalogDegrade.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogDegrade.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace AnalogDegrade {

AnalogDegrade::AnalogDegrade() {
  samplerate = sampleRate();

  noiseProc.prepare(samplerate);
  filterProc.reset(samplerate, int(samplerate * 0.05f));
  levelDetector.prepare(samplerate);
  gainProc.reset((double)samplerate, 0.05f);
  cookParams(samplerate, 0.5f, 0.5f, 0.5f, 0.5f);

  mCalcFunc = make_calc_function<AnalogDegrade, &AnalogDegrade::next>();
  next(1);
}

AnalogDegrade::~AnalogDegrade() {}

float AnalogDegrade::uniform() { return ((float)rand() / (RAND_MAX)); }

void AnalogDegrade::cookParams(float fs, float depthParam, float amtParam,
                               float varParam, float envParam) {
  const auto freqHz = 200.0f * std::pow(20000.0f / 200.0f, 1.0f - amtParam);
  const auto gainDB = -24.0f * depthParam;

  noiseProc.setGain(0.33f * depthParam * amtParam);
  filterProc.setFreq(std::min(
      freqHz + (varParam * (freqHz / 0.6f) * (uniform() - 0.5f)), 0.49f * fs));

  const auto envSkew = 1.0f - std::pow(envParam, 0.8f);
  levelDetector.setParameters(10.0f,
                              20.0f * std::pow(5000.0f / 20.0f, envSkew));
  gainProc.setTargetValue(std::pow(
      10.0f, 0.05f * std::min(gainDB + (varParam * 36.0f * (uniform() - 0.5f)),
                              3.0f)));
}

void AnalogDegrade::next(int nSamples) {
  float depth = in0(Depth);
  float amt = in0(Amount);
  float var = in0(Variance);
  float env = in0(Envelope);

  cookParams(samplerate, depth, amt, var, env);

  const float *input = in(Input);
  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    // get input
    float x = mkutils::constrain(input[i], -1.0f, 1.0f);
    auto level = levelDetector.processSample(x);
    auto noise = noiseProc.processSample(0.0f);
    if (env > 0.0f) {
      noise *= level;
    }
    x += noise;
    x = filterProc.processSample(x);
    x *= gainProc.getNextValue();
    outbuf[i] = x;
  }
}

void AnalogDegrade::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

}  // namespace AnalogDegrade

PluginLoad(AnalogDegradeUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogDegrade::AnalogDegrade>(ft, "AnalogDegrade", false);
}
