#include "buchla259waveshaper.hpp"

namespace buchla259waveshaper {
void Buchla259WaveShaper::init(float samplerate) {
  m_samplerate = samplerate;
  double f0init = 100;
  double ampinit = 0.5;

  // Side one
  cell[0].init(samplerate, f0init, ampinit, 91000, 56000, 21.427999);
  cell[1].init(samplerate, f0init, ampinit, 49900, 43200, 27.777);
  cell[2].init(samplerate, f0init, ampinit, 10000, 100000, 12);

  // Side two
  cell[3].init(samplerate, f0init, ampinit, 30000, 68000, 0.3662);
  cell[4].init(samplerate, f0init, ampinit, 68000, 33000, 0.7545);

  sine.init(samplerate, f0init, ampinit);
}

// @TODO Buchla sin
float Buchla259WaveShaper::process() {
  /* side one */
  const auto sideone =
      cell[0].process() + cell[1].process() + cell[2].process();

  /* side two */
  auto sidetwo =
      cell[3].process() + cell[4].process() + (sine.process() * 0.1037);
  sidetwo *= -1.0;
  sidetwo *= 48.192799;

  /* generate output */
  float output = sideone + sidetwo;
  output *= -1;
  output *= 0.05;

  // @FIXME: The original circuit/paper has a onepole lowpass filter here set at
  // 1330 hz to act as a tone control.
  return output;
}

}  // namespace buchla259waveshaper
