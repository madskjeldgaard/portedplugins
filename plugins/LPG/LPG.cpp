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
  lowpassgate.setOutputNum(lpg::LpgFilter::OutputNumber::Vout);

  if (inRate(ControlInput) == calc_FullRate) {
    mCalcFunc = make_calc_function<LPG, &LPG::next_a>();
    next_a(1);
  } else {
    mCalcFunc = make_calc_function<LPG, &LPG::next_k>();
    next_k(1);
  }
}
void LPG::next_k(int nSamples) {
  const float *input = in(Input);
  const int lpmode = in0(LowpassMode);
  const int linmode = in0(Linearity);

  auto slopedControlInput = makeSlope(in0(ControlInput), m_control_input_past);
  auto slopedOffset = makeSlope(in0(ControlOffset), m_control_offset_past);
  auto slopedScale = makeSlope(in0(ControlScale), m_sloped_control_scale);
  auto slopedVcaamount = makeSlope(in0(VCAAmount), m_sloped_vca_amount);
  auto slopedResonance = makeSlope(in0(Resonance), m_sloped_resonance);

  // Lowpass filter mode
  auto lowpass = (lpmode == 1) ? lpg::LpgFilter::LowpassProcessing::On
                               : lpg::LpgFilter::LowpassProcessing::Off;
  lowpassgate.setLowpassMode(lowpass);

  // Linear or nonlinear processing
  auto linearity = (linmode == 1) ? lpg::LpgFilter::Linearity::Linear
                                  : lpg::LpgFilter::Linearity::NonLinear;

  lowpassgate.setLinearity(linearity);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float control_input = slopedControlInput.consume();

    const float offset = slopedOffset.consume();
    const float scale = slopedScale.consume();
    const float vcaamount = slopedVcaamount.consume();
    const float resonance = slopedResonance.consume();

    lowpassgate.setControlOffset(offset);
    lowpassgate.setControlScale(scale);
    lowpassgate.setVCAAmount(vcaamount);
    lowpassgate.setResonance(resonance);

    outbuf[i] = lowpassgate.process(input[i], control_input);
  }

  m_control_offset_past = slopedOffset.value;
  m_sloped_control_scale = slopedScale.value;
  m_sloped_vca_amount = slopedVcaamount.value;
  m_sloped_resonance = slopedResonance.value;
  m_control_input_past = slopedControlInput.value;
}

void LPG::next_a(int nSamples) {
  const float *input = in(Input);
  const float *control_input = in(ControlInput);
  const int lpmode = in0(LowpassMode);
  const int linmode = in0(Linearity);

  auto slopedOffset = makeSlope(in0(ControlOffset), m_control_offset_past);
  auto slopedScale = makeSlope(in0(ControlScale), m_sloped_control_scale);
  auto slopedVcaamount = makeSlope(in0(VCAAmount), m_sloped_vca_amount);
  auto slopedResonance = makeSlope(in0(Resonance), m_sloped_resonance);

  // Lowpass filter mode
  auto lowpass = (lpmode == 1) ? lpg::LpgFilter::LowpassProcessing::On
                               : lpg::LpgFilter::LowpassProcessing::Off;
  lowpassgate.setLowpassMode(lowpass);

  // Linear or nonlinear processing
  auto linearity = (linmode == 1) ? lpg::LpgFilter::Linearity::Linear
                                  : lpg::LpgFilter::Linearity::NonLinear;

  lowpassgate.setLinearity(linearity);

  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    const float offset = slopedOffset.consume();
    const float scale = slopedScale.consume();
    const float vcaamount = slopedVcaamount.consume();
    const float resonance = slopedResonance.consume();

    lowpassgate.setControlOffset(offset);
    lowpassgate.setControlScale(scale);
    lowpassgate.setVCAAmount(vcaamount);
    lowpassgate.setResonance(resonance);

    outbuf[i] = lowpassgate.process(input[i], control_input[i]);
  }

  m_control_offset_past = slopedOffset.value;
  m_sloped_control_scale = slopedScale.value;
  m_sloped_vca_amount = slopedVcaamount.value;
  m_sloped_resonance = slopedResonance.value;
}

void LPG::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace LPG

PluginLoad(LPGUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<LPG::LPG>(ft, "LPG", false);
}
