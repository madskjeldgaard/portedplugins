#pragma once
#include "wdf.h"

/** Wave digital filters to eumlate the Baxandall EQ circuit. */

class BaxandallTreble {
public:
    BaxandallTreble() = default;

    void reset(double sampleRate);
    void setTreble(float treble);

    inline float processSample(float x) noexcept {
        Vin.setVoltage((double) x);

        Vin.incident(S3->reflected());
        S3->incident(Vin.reflected());
        auto y = C4->voltage() + VR2b.voltage();

        return (float) y;
    }

private:
    WDF::IdealVoltageSource Vin;
    WDF::Resistor VR2a { 100000.0 };
    WDF::Resistor VR2b { 100000.0 };

    std::unique_ptr<WDF::Capacitor> C3, C4;
    std::unique_ptr<WDF::WDFSeries> S1, S2, S3;
};

//////////////////////////////////
class BaxandallBass {
public:
    BaxandallBass() = default;

    void reset(double sampleRate);
    void setTreble(float treble);
    void setBass(float bass);

    inline float processSample(float x) noexcept {
        Vin.setVoltage((double) x);

        Vin.incident(S5->reflected());
        S5->incident(Vin.reflected());
        auto y = C4->voltage() + VR2b.voltage();

        return (float) y;
    }

private:
    WDF::IdealVoltageSource Vin;
    WDF::Resistor R1 { 10000.0 };
    WDF::Resistor R2 { 1000.0 };
    WDF::Resistor R3 { 10000.0 };

    WDF::Resistor VR1a { 100000.0 };
    WDF::Resistor VR1b { 100000.0 };
    WDF::Resistor VR2b { 100000.0 };

    std::unique_ptr<WDF::Capacitor> C1, C2, C4;
    std::unique_ptr<WDF::WDFSeries> S1, S2, S3, S4, S5;
    std::unique_ptr<WDF::WDFParallel> P1, P2, P3;
};

//////////////////////////////////
class BaxandallEQ {
public:
    BaxandallEQ() = default;

    void reset(double sampleRate);
    void setTreble(float trebleParam);
    void setBass(float bassParam);

    inline float processSample(float x) noexcept {
        return bass.processSample(x) + treble.processSample(x);
    }

private:
    BaxandallTreble treble;
    BaxandallBass bass;

    float oldTrebleParam = 0.0f;
    float oldBassParam = 0.0f;
};
