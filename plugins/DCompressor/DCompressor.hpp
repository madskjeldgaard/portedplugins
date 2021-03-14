// DCompressor.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "SC_PlugIn.hpp"
/* #include "../comp.c" */
#include "Dynamics/compressor.cpp"

namespace DCompressor {

class DCompressor : public SCUnit {
public:
    DCompressor();

private:
    // Calc function
    void next(int nSamples);
    void clear(int nSamples);

    enum InputParams { Input, SideChainInput, SideChain, Ratio, Threshold, Attack, Release, Makeup, AutoMakeup, NumInputParams };
    enum Outputs { Out1, NumOutputParams };

	// States
	float m_attack_past{0.f}, m_ratio_past{1.f}, m_threshold_past{0.f}, m_release_past{0.f}, m_makeup_past{0.f};

	daisysp::Compressor comp;

};

} // namespace DCompressor

