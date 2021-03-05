// Rongs.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "../mutated/mi-modal.cpp"
#include "SC_PlugIn.hpp"

namespace Rongs {

class Rongs : public SCUnit {
public:
  Rongs();

  // Destructor
  ~Rongs();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams {
    Trigger,
    Sustain,
    F0,
    Structure,
    Brightness,
    Damping,
    Accent,
	Stretch,
	Position,
	Loss,
	NumModes,
	CosFreq,
    NumInputParams
  };
  enum Outputs { Output, NumOutputParams };

  mi::ModalVoice modalvoice;
  float *m_internal_noise_buffer;
};

} // namespace Rongs
