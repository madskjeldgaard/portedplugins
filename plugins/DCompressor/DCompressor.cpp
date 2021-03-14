// DCompressor.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "DCompressor.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace DCompressor {

DCompressor::DCompressor() {
  const float samplerate = sampleRate();
  comp.Init(samplerate);
  comp.AutoMakeup(true);
  m_ratio_past = comp.GetRatio();
  m_attack_past = comp.GetAttack();
  m_release_past = comp.GetRelease();
  m_threshold_past = comp.GetThreshold();
  m_makeup_past = comp.GetMakeup();

  mCalcFunc = make_calc_function<DCompressor, &DCompressor::next>();
  next(1);
}

/* DCompressor::~DCompressor() {} */

void DCompressor::next(int nSamples) {
  const float *input = in(Input);
  const float *sidechainin = in(SideChainInput);
  const bool sidechain = (in0(SideChain) > 0) ? true : false;

  SlopeSignal<float> slopedAttack = makeSlope(in0(Attack), m_attack_past);

  Print("%f \n", sidechainin[0]);

  // Clip to 1 as lowest value (otherwise it spits out nan)
  const float ratioParam = (in0(Ratio) < 1) ? in0(Ratio) : 1;
  SlopeSignal<float> slopedRatio = makeSlope(ratioParam, m_ratio_past);

  SlopeSignal<float> slopedThreshold =
      makeSlope(in0(Threshold), m_threshold_past);
  SlopeSignal<float> slopedRelease = makeSlope(in0(Release), m_release_past);
  SlopeSignal<float> slopedMakeup = makeSlope(in0(Makeup), m_makeup_past);

  if (in0(AutoMakeup) == 1) {
    comp.AutoMakeup(true);
  } else {
    comp.AutoMakeup(false);
  }

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    comp.SetAttack(slopedAttack.consume());
    comp.SetRelease(slopedRelease.consume());
    comp.SetThreshold(slopedThreshold.consume());
    comp.SetMakeup(slopedMakeup.consume());
    comp.SetRatio(slopedRatio.consume());

    outbuf[i] = sidechain ? comp.Process(input[i], sidechainin[i])
                          : comp.Process(input[i]);
  }

  m_attack_past = slopedAttack.value;
  m_ratio_past = slopedRatio.value;
  m_threshold_past = slopedThreshold.value;
  m_release_past = slopedRelease.value;
  m_makeup_past = slopedMakeup.value;
}

void DCompressor::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace DCompressor

PluginLoad(DCompressorUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<DCompressor::DCompressor>(ft, "DCompressor", false);
}
