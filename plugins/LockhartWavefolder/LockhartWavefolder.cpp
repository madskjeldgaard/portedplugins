// LockhartWavefolder.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "LockhartWavefolder.hpp"

#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace LockhartWavefolder {

LockhartWavefolder::LockhartWavefolder() {
  // Initialize cells
  m_numCells = in0(NumCells);
  cells = (LockhartCell *)RTAlloc(mWorld, m_numCells * sizeof(LockhartCell));
  for (size_t i = 0; i < m_numCells; i++) {
    cells[i] = LockhartCell();
  };

  // Freak out if init went wrong
  if (cells == NULL || cells == nullptr) {
    mCalcFunc =
        make_calc_function<LockhartWavefolder, &LockhartWavefolder::clear>();
    Print("LockhartCells not allocated\n");
  } else {
    mCalcFunc =
        make_calc_function<LockhartWavefolder, &LockhartWavefolder::next>();
    next(1);
  }
}

LockhartWavefolder::~LockhartWavefolder() { RTFree(mWorld, cells); }

void LockhartWavefolder::next(int nSamples) {
  const float *input = in(Input);
  float *outbuf = out(Out1);

  for (int i = 0; i < nSamples; ++i) {
    auto in = static_cast<double>(input[i]);

    // Scale input to number of cells
    in *= (1.0 / m_numCells);

    // @TODO this is a dirty hack to make sure the exp() functions in the
    // cells don't become inf (with RL at 7.5e3 this happens around signals at
    // +/-9.5)
    in = sc_clip(in, -9.0, 9.0);

    for (int cell_num = 0; cell_num < m_numCells; cell_num++) {
      in = cells[cell_num].process(in);
    };

    in *= static_cast<double>(m_numCells);
    in = std::tanh(in);

    // Remove nan, inf etc
    in = zapgremlins(in);

    outbuf[i] = static_cast<float>(in);
  }
}

void LockhartWavefolder::clear(int nSamples) {
  ClearUnitOutputs(this, nSamples);
}

}  // namespace LockhartWavefolder

PluginLoad(LockhartWavefolderUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<LockhartWavefolder::LockhartWavefolder>(ft, "LockhartWavefolder",
                                                       false);
}
