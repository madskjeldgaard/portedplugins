// VADiodeFilter.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "VADiodeFilter.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace VADiodeFilter {

VADiodeFilter::VADiodeFilter() : filter(DiodeFilter()) {
  const float samplerate = sampleRate();
  filter.setSampleRate(samplerate);
  filter.reset();
  filter.setup();
  auto initRes = in0(Res);
  filter.setResControl(initRes);
  auto initFreq = in0(Freq);
  filter.setFreq(initFreq);
  auto initOd = in0(Overdrive);
  filter.setOverdrive(initOd);
  m_overdrive_past = initOd;
  m_freq_past = initFreq;
  m_res_past = initRes;
  mCalcFunc = make_calc_function<VADiodeFilter, &VADiodeFilter::next>();
  next(1);
}

VADiodeFilter::~VADiodeFilter() {}

void VADiodeFilter::next(int nSamples) {
  const float *input = in(Input);
  float *outbuf = out(Out1);
  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);
  SlopeSignal<float> slopedRes = makeSlope(in0(Res), m_res_past);
  SlopeSignal<float> slopedOverdrive =
      makeSlope(in0(Overdrive), m_overdrive_past);

  for (int i = 0; i < nSamples; ++i) {
    const float res = slopedRes.consume();
    const float freq = slopedFreq.consume();
    const float overdrive = slopedOverdrive.consume();
    filter.setOverdrive(overdrive);
    filter.setResControl(res);
    filter.setFreq(freq);
    outbuf[i] = filter.doFilter(input[i]);
  }

  m_overdrive_past = slopedOverdrive.value;
  m_freq_past = slopedFreq.value;
  m_res_past = slopedRes.value;
}

void VADiodeFilter::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace VADiodeFilter

PluginLoad(VADiodeFilterUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<VADiodeFilter::VADiodeFilter>(ft, "VADiodeFilter", false);
}
