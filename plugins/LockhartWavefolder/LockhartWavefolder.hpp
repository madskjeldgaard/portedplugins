// LockhartWavefolder.hpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#pragma once

#include "LockhartCell.hpp"
#include "SC_PlugIn.hpp"

namespace LockhartWavefolder {

class LockhartWavefolder : public SCUnit {
public:
  LockhartWavefolder();

  // Destructor
  ~LockhartWavefolder();

private:
  // Calc function
  void next(int nSamples);
  void clear(int nSamples);

  enum InputParams { Input, NumCells, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  /* LockhartCell cell1, cell2, cell3, cell4; */
  LockhartCell *cells;
  int m_numCells;
};

} // namespace LockhartWavefolder
