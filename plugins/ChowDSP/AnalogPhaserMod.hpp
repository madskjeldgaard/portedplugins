// AnalogPhaserMod.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "chowphasermod.hpp"

namespace AnalogPhaserMod {

class AnalogPhaserMod : public SCUnit {
public:
    AnalogPhaserMod();

private:
    // Calc function
    void next_a(int nSamples);
	void next_k(int nSamples);

    void clear(int nSamples);

    enum InputParams { Input, Skew, Modulation, Stages, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	chow::ChowPhaserMod phasermod;

	float m_skew_past{0.f};
	float m_modulation_past{0.5f};
	float m_stages_past{1.f};
	float m_input_past{0.f};
};

} // namespace AnalogPhaserMod

