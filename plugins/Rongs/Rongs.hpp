// Rongs.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "../mi-modal.hpp"
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
    NumInputParams
  };
  enum Outputs { Output, AuxOut, NumOutputParams };

  mi::ModalVoice modalvoice;
  float *temp_buffer_;
};

} // namespace Rongs
