// PluginChen.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "Chen.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace Chen {

Chen::Chen() {
  RGen &rgen = *this->mParent->mRGen;
  const float initx = rgen.frand();
  const float inity = rgen.frand();
  const float initz = rgen.frand();
  const float initfreq = 0.0001f;
  chengen.setup(initfreq, initx, inity, initz);

  mCalcFunc = make_calc_function<Chen, &Chen::next>();
  next(1);
}

void Chen::next(int nSamples) {
  float *xout = out(X);
  float *yout = out(Y);
  float *zout = out(Z);

  SlopeSignal<float> slopedFreq = makeSlope(in0(Speed), m_freq_past);
  SlopeSignal<float> slopedA = makeSlope(in0(A), m_a_past);
  SlopeSignal<float> slopedB = makeSlope(in0(B), m_b_past);
  SlopeSignal<float> slopedC = makeSlope(in0(C), m_c_past);

  // simple gain function
  for (int i = 0; i < nSamples; ++i) {
    const float f = slopedFreq.consume();

    const float a = slopedA.consume();
    const float b = slopedB.consume();
    const float c = slopedC.consume();

    chengen.setSpeed(f);
    chengen.setParameter(chaos::ChenGen::A, a);
    chengen.setParameter(chaos::ChenGen::B, b);
    chengen.setParameter(chaos::ChenGen::C, c);

    xout[i] = zapgremlins(chengen.process());
    yout[i] = zapgremlins(chengen.getOutput(chaos::ChenGen::Y));
    zout[i] = zapgremlins(chengen.getOutput(chaos::ChenGen::Z));
  }
  m_freq_past = slopedFreq.value;
  m_a_past = slopedA.value;
  m_b_past = slopedB.value;
  m_c_past = slopedC.value;
}

} // namespace Chen

PluginLoad(ChenUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<Chen::Chen>(ft, "Chen", false);
}
