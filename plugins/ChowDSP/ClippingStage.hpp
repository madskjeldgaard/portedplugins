#pragma once
#include "wdf.h"

class ClippingStage {
public:
    ClippingStage() = default;

    void reset (double sampleRate)
    {
        C9  = WDF::make_unique<WDF::Capacitor> (1.0e-6, sampleRate);

        I1 = WDF::make_unique<WDF::PolarityInverter> (&Vin);
        S2 = WDF::make_unique<WDF::WDFSeries> (I1.get(), &R13);
        P1 = WDF::make_unique<WDF::WDFParallel> (S2.get(), C9.get());
        D23.connectToNode (S2.get());
    }

    inline float processSample (float x)
    {
        Vin.setVoltage ((double) x);

        D23.incident (S2->reflected());
        S2->incident (D23.reflected());
        auto y = D23.voltage() * 10.0f;

        return (float) y; // * 500000.0f;
    }

private:
    WDF::ResistiveVoltageSource Vin;
    std::unique_ptr<WDF::Capacitor> C9;
    WDF::Resistor R13 { 1000.0 };
    WDF::DiodePair D23 { 15e-6, 0.02585 };

    std::unique_ptr<WDF::PolarityInverter> I1;
    std::unique_ptr<WDF::WDFSeries> S2;
    std::unique_ptr<WDF::WDFParallel> P1;
};
