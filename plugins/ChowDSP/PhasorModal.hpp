// PhasorModal.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "Mode.hpp"
#include "SC_PlugIn.hpp"
#include "../mkutils.hpp"

namespace PhasorModal {

class PhasorModal : public SCUnit {
public:
  PhasorModal();

  // Destructor
  /* ~PhasorModal(); */

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { Input, Freq, Decay, Damp, Amp, Phase, NumInputParams };
  enum Outputs { Out1, NumOutputParams };
  chow::Mode mode;

  float m_damp_past{0.5f};
  float m_amp_past{0.25f};
  float m_phase_past{0.0f};
  float m_decay_past{0.5f};
  float m_freq_past{100.0f};
};

} // namespace PhasorModal
