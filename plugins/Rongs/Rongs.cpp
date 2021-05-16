// Rongs.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "Rongs.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace Rongs {

Rongs::Rongs() {
  int modeNum = in0(NumModes);
  const float cosFreq = in0(CosFreq);
  const int maxnum = mi::ResonatorProcessingMode::NumProcessingModes;
  modeNum = (modeNum >= maxnum || modeNum < 0) ? 0 : modeNum;

  mi::ResonatorProcessingMode mode;
  switch (modeNum) {
  case 0:
    mode = mi::ResonatorProcessingMode::CHAEP;
    break;
  case 1:
    mode = mi::ResonatorProcessingMode::BUDGET;
    break;
  case 2:
    mode = mi::ResonatorProcessingMode::PRETTYGOOD;
    break;
  case 3:
    mode = mi::ResonatorProcessingMode::EXPENSIVE;
    break;
  case 4:
    mode = mi::ResonatorProcessingMode::LUXURY;
    break;
  case 5:
    mode = mi::ResonatorProcessingMode::EXTREME;
    break;
  default:
    mode = mi::ResonatorProcessingMode::BUDGET;
  }

  const auto sr = sampleRate();
  modalvoice.Init(mode, cosFreq, sr);
  const size_t temp_buffer_size = bufferSize() * sizeof(float);
  m_internal_noise_buffer = (float *)RTAlloc(mWorld, temp_buffer_size);

  if (m_internal_noise_buffer == NULL) {
    mCalcFunc = make_calc_function<Rongs, &Rongs::clear>();
    clear(1);

    if (mWorld->mVerbosity > -2) {
      Print("Failed to allocate memory for AnalogEcho ugen.\n");
    }
  } else {
    mCalcFunc = make_calc_function<Rongs, &Rongs::next>();
    next(1);
  }

  // Fill the buffer with zeros.
  memset(m_internal_noise_buffer, 0, temp_buffer_size);
}

Rongs::~Rongs() { RTFree(mWorld, m_internal_noise_buffer); }

void Rongs::next(int nSamples) {
  /* const float *input = in(Input); */
  const float triggerIn = in0(Trigger);
  const float sustainIn = in0(Sustain);

  bool sustain = (sustainIn > 0.0f) ? true : false;
  bool trigger = (triggerIn > 0.0f) ? true : false;
  float accent = in0(Accent);
  float f0 = in0(F0);
  float structure = in0(Structure);
  float brightness = in0(Brightness);
  float damping = in0(Damping);
  float stretch = in0(Stretch);
  float position = in0(Position);
  float loss = in0(Loss);

  float *outbuf = out(Output);

  modalvoice.Render(sustain, trigger, accent, f0, structure, brightness,
                    damping, stretch, position, loss, m_internal_noise_buffer,
                    outbuf, nSamples);
}

void Rongs::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace Rongs

PluginLoad(RongsUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<Rongs::Rongs>(ft, "Rongs", false);
}
