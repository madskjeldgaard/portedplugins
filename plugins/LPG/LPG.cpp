// LPG.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "LPG.hpp"
#include "SC_PlugIn.hpp"

InterfaceTable *ft;

namespace LPG {

LPG::LPG() {
  const float samplerate = sampleRate();
  const float initOffset = 0.0f;
  const float initScale = 1.0f;
  const float initVCA = 1.0f;
  const float initResonance = 1.5f;
  const lpg::LpgFilter::Linearity initLinearity =
      lpg::LpgFilter::Linearity::NonLinear;

  lowpassgate.setup(samplerate, initOffset, initScale, initVCA, initResonance,
                    initLinearity);

  lowpassgate.setLowpassMode(lpg::LpgFilter::LowpassProcessing::On);

  mCalcFunc = make_calc_function<LPG, &LPG::next>();
  next(1);
}

void LPG::next(int nSamples) {
  const float *input = in(Input);
  const float *control_input = in(ControlInput);
  const float offset = in0(ControlOffset);
  const float scale = in0(ControlScale);
  const float vcaamount = in0(VCAAmount);
  const float resonance = in0(Resonance);
  const int lpmode = in0(LowpassMode);
  const int linmode = in0(Linearity);
  const int outnum = in0(OutNum);

  lowpassgate.setControlOffset(offset);
  lowpassgate.setControlScale(scale);
  lowpassgate.setVCAAmount(vcaamount);
  lowpassgate.setResonance(resonance);

  // Lowpass filter mode
  auto lowpass = (lpmode == 1) ? lpg::LpgFilter::LowpassProcessing::On
                               : lpg::LpgFilter::LowpassProcessing::Off;
  lowpassgate.setLowpassMode(lowpass);

  // Linear or nonlinear processing
  auto linearity = (linmode == 1) ? lpg::LpgFilter::Linearity::Linear
                                  : lpg::LpgFilter::Linearity::NonLinear;

  lowpassgate.setLinearity(linearity);

  if (outnum == 0) {
    lowpassgate.setOutputNum(lpg::LpgFilter::OutputNumber::Vout);
  } else if (outnum == 1) {
    lowpassgate.setOutputNum(lpg::LpgFilter::OutputNumber::VactrolOut);
  } else if (outnum == 2) {
    lowpassgate.setOutputNum(lpg::LpgFilter::OutputNumber::Differentiator);
  }

  /* Print("%f \n", control_input[0]); */
  /* Print("input: %f \n", input[0]); */

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    outbuf[i] = lowpassgate.process(input[i], control_input[i]);
  }
}

void LPG::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace LPG

PluginLoad(LPGUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<LPG::LPG>(ft, "LPG", false);
}
