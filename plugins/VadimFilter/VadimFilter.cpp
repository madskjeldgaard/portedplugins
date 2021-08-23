// VadimFilter.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)
#include "VadimFilter.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace vadim {

VadimFilter::VadimFilter() {
  const auto samplerate = sampleRate();
  type = static_cast<VadimFilterType>(in0(Type));

  this->z1_1 = 0.0f;
  this->z1_2 = 0.0f;
  this->z1_3 = 0.0f;
  this->z1_4 = 0.0f;
  this->m_freq_past = in0(Freq);
  this->m_resonance_past = in0(Resonance);

  const auto halft = 0.5 / samplerate;
  const auto twoslasht = 1.0 / halft;

  this->halft = static_cast<float>(halft);
  this->twoslasht = static_cast<float>(twoslasht);

  mCalcFunc = make_calc_function<VadimFilter, &VadimFilter::next>();
  next(1);
}

VadimFilter::~VadimFilter() {}

void VadimFilter::next(int nSamples) {
  const float *input = in(Input);

  SlopeSignal<float> slopedFreq = makeSlope(in0(Freq), m_freq_past);

  SlopeSignal<float> slopedResonance =
      makeSlope(in0(Resonance), m_resonance_past);
  float *outbuf = out(Out1);

  float halft = this->halft;
  float twoslasht = this->twoslasht;
  float z1_1 = this->z1_1;
  float z1_2 = this->z1_2;
  float z1_3 = this->z1_3;
  float z1_4 = this->z1_4;

  for (int n = 0; n < nSamples; ++n) {
    const float xn = input[n];

    const float K = slopedResonance.consume();
    const float freq = slopedFreq.consume();

    const float wd = twopi * freq;
    // @FIXME find cheaper tanf
    const float wa = twoslasht * tanf(wd * halft);

    const float g = wa * halft;
    const float alpha = g / (1.0f + g);
    const float beta4 = 1.0f / (1.0f + g);
    const float beta3 = beta4 * alpha;
    const float beta2 = beta3 * alpha;
    const float beta1 = beta2 * alpha;
    const float gamma = alpha * alpha * alpha * alpha;

    const float alpha0 = 1.0f / (1.0f + (gamma * K * 4.0f));

    const float sigma =
        z1_1 * beta1 + z1_2 * beta2 + z1_3 * beta3 + z1_4 * beta4;

    const float u = (xn - K * sigma) * alpha0;

    float vn = (u - z1_1) * alpha;
    const float lpf1 = vn + z1_1;
    z1_1 = vn + lpf1;

    vn = (lpf1 - z1_2) * alpha;
    const float lpf2 = vn + z1_2;
    z1_2 = vn + lpf2;

    vn = (lpf2 - z1_3) * alpha;
    const float lpf3 = vn + z1_3;
    z1_3 = vn + lpf3;

    vn = (lpf3 - z1_4) * alpha;
    const float lpf4 = vn + z1_4;
    z1_4 = vn + lpf4;

    // TODO template output
    switch (type) {
    case LPF2:
      outbuf[n] = lpf2;
      break;
    case LPF4:
      outbuf[n] = lpf4;
      break;
			case BPF2:
      outbuf[n] = lpf1 * 2.0f + lpf2 * -2.0f;
      break;
    case BPF4:
      outbuf[n] = lpf2 * 4.0f + lpf3 * -8.0f + lpf4 * 4.0f;
      break;

case HPF2:
      outbuf[n] = u + (lpf1 * -2.0f) + lpf2;
      break;
    case HPF4:
      outbuf[n] = u + (lpf1 * -4.0f) + (lpf2 * 6.0f) + (lpf3 * -4.0f) + lpf4;
      break;

    default:
      outbuf[n] = 0;
		}
  }

  m_resonance_past = slopedResonance.value;
  m_freq_past = slopedFreq.value;

  this->z1_1 = z1_1;
  this->z1_2 = z1_2;
  this->z1_3 = z1_3;
  this->z1_4 = z1_4;
}

void VadimFilter::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace vadim

PluginLoad(VadimFilterUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<vadim::VadimFilter>(ft, "VadimFilter", false);
}
