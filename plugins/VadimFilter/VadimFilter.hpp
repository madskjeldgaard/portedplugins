// VadimFilter.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

/*
 *
 * Ported from folderkit: https://codeberg.org/nonmateria/folderkit
 *
 * implemented thanks to the W. Pirke book "Designing Software Synthesizer
 * Plug-Ins in C++" algorithm based on the paper "The Art of VA Filter Design"
 * by Vadim Zavalishin
 * https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_1.1.1.pdf
 */

#pragma once

#include "SC_Constants.h"
#include "SC_PlugIn.hpp"

namespace vadim {

enum VadimFilterType { LPF2, LPF4, BPF2, BPF4, HPF2, HPF4 };

class VadimFilter : public SCUnit {
public:
  VadimFilter();

  // Destructor
  ~VadimFilter();

private:
  float z1_1;
  float z1_2;
  float z1_3;
  float z1_4;
  float halft;
  float twoslasht;
  float m_freq_past, m_resonance_past;

  VadimFilterType type;
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  /* void lpf4(); */
  /* void lpf2(); */
  /* void bpf4(); */
  /* void bpf2(); */
  /* void hpf4(); */
  /* void hpf2(); */

  enum InputParams { Type, Input, Freq, Resonance, NumInputParams };
  enum Outputs { Out1, NumOutputParams };
};

} // namespace vadim
