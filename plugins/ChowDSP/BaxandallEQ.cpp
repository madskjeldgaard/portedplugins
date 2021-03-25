#include "BaxandallEQ.hpp"
#include "../mkutils.hpp"

using namespace WDF;

namespace {
    constexpr float bassTaper = 5.644f;
    constexpr float trebleTaper = 3.889f;
    constexpr float minFloat = (float) 1.0e-6;
    constexpr float maxFloat = 1.0f - (float) 1.0e-6;
}

float resistorTaper(float val01, float alpha) {
  return std::pow(mkutils::constrain(val01, minFloat, maxFloat), alpha);
}

//////////////////////////////////
void BaxandallEQ::reset(double sampleRate) {
    treble.reset(sampleRate);
    bass.reset(sampleRate);
}

void BaxandallEQ::setTreble(float trebleParam) {
    if(oldTrebleParam == trebleParam) // no update needed
        return;

    oldTrebleParam = trebleParam;
    treble.setTreble(trebleParam);
    bass.setTreble(trebleParam);
}

void BaxandallEQ::setBass(float bassParam) {
    if(oldBassParam == bassParam)  // no update needed
        return;

    oldBassParam = bassParam;
    bass.setBass(bassParam);
}


//////////////////////////////////
void BaxandallTreble::reset(double sampleRate) {
    C3 = make_unique<Capacitor>(2.2e-9, sampleRate);
    C4 = make_unique<Capacitor>(22e-9, sampleRate);

    S1 = make_unique<WDFSeries>(&VR2b, C4.get());
    S2 = make_unique<WDFSeries>(S1.get(), &VR2a);
    S3 = make_unique<WDFSeries>(C3.get(), S2.get());

    Vin.connectToNode(S3.get());
}

void BaxandallTreble::setTreble(float treble) {
    treble = resistorTaper(treble, trebleTaper);
    VR2a.setResistanceValue(100000.0 * (1.0 - (double) treble));
    VR2b.setResistanceValue(100000.0 * (double) treble);
}

//////////////////////////////////
void BaxandallBass::reset(double sampleRate) {
    C1 = make_unique<Capacitor>(22e-9, sampleRate);
    C2 = make_unique<Capacitor>(220e-9, sampleRate);
    C4 = make_unique<Capacitor>(22e-9, sampleRate);

    P1 = make_unique<WDFParallel>(C1.get(), &VR1a);
    P2 = make_unique<WDFParallel>(C2.get(), &VR1b);

    S1 = make_unique<WDFSeries>(C4.get(), &VR2b);
    S2 = make_unique<WDFSeries>(S1.get(), &R3);

    S3 = make_unique<WDFSeries>(P1.get(), &R2);
    P3 = make_unique<WDFParallel>(S3.get(), S2.get());
    S4 = make_unique<WDFSeries>(P3.get(), P2.get());
    S5 = make_unique<WDFSeries>(S4.get(), &R1);

    Vin.connectToNode(S5.get());
}

void BaxandallBass::setTreble(float treble) {
    treble = resistorTaper(treble, trebleTaper);
    VR2b.setResistanceValue(100000.0 * (double) treble);
}

void BaxandallBass::setBass(float bass) {
    bass = resistorTaper(bass, bassTaper);
    VR1b.setResistanceValue(100000.0 * (1.0 - (double) bass));
    VR1a.setResistanceValue(100000.0 * (double) bass);
}
