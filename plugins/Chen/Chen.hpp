// PluginChen.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
#include "../chaos.cpp"

namespace Chen {

class Chen : public SCUnit {
public:
    Chen();

    // Destructor
    // ~Chen();

private:
    // Calc function
    void next(int nSamples);

	enum Inputs { Speed, A, B, C};
	enum Outputs {X, Y, Z};

    // Member variables
	float m_freq_past{0.f};
	float m_a_past{0.f}, m_b_past{0.f}, m_c_past{0.f};
	chaos::ChenGen chengen;
};

} // namespace Chen
