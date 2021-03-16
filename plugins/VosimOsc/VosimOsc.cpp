// VosimOsc.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "VosimOsc.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace VosimOsc {

VosimOsc::VosimOsc() {
  const float samplerate = sampleRate();
  vosim.Init(samplerate);

  mCalcFunc = make_calc_function<VosimOsc, &VosimOsc::next>();
  next(1);
}

void VosimOsc::next(int nSamples) {
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedForm1 = makeSlope(in0(Form1), m_form1_past);
  SlopeSignal<float> slopedForm2 = makeSlope(in0(Form2), m_form2_past);
  SlopeSignal<float> slopedShape = makeSlope(in0(Shape), m_shape_past);
  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    vosim.SetFreq(slopedFreq.consume());
    vosim.SetForm1Freq(slopedForm1.consume());
    vosim.SetForm2Freq(slopedForm1.consume());
    vosim.SetShape(slopedShape.consume());

    outbuf[i] = vosim.Process();
  }

  m_freq_past = slopedFreq.value;
  m_form1_past = slopedForm1.value;
  m_form2_past = slopedForm2.value;
  m_shape_past = slopedShape.value;
}

void VosimOsc::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace VosimOsc

PluginLoad(VosimOscUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<VosimOsc::VosimOsc>(ft, "VosimOsc", false);
}
