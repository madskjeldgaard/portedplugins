#include "bbd_line.h"
#include <algorithm>
#include <cassert>
#include <iostream>

extern InterfaceTable *ft;

static unsigned interp_size = 128;

void BBD_Line::free(World *mWorld) {
  RTFree(mWorld, mem_);
  RTFree(mWorld, Xin_);
  RTFree(mWorld, Xout_);
  RTFree(mWorld, Xout_mem_);
  RTFree(mWorld, Gin_);
  RTFree(mWorld, Gout_);
};

void BBD_Line::setup(World *mWorld, double fs, unsigned ns,
                     const BBD_Filter_Spec &fsin,
                     const BBD_Filter_Spec &fsout) {

  // This is a trade off: Make the memory realtime safe, no longer making it
  // possible to change size after setup.
  mem_size = ns * sizeof(float);
  mem_ = (float *)RTAlloc(mWorld, mem_size);

  const BBD_Filter_Coef &fin =
      BBD::compute_filter_cached(fs, interp_size, fsin);
  const BBD_Filter_Coef &fout =
      BBD::compute_filter_cached(fs, interp_size, fsout);
  fin_ = &fin;
  fout_ = &fout;

  unsigned Min = fin.M;
  unsigned Mout = fout.M;

  const auto cdoubleSize = sizeof(cdouble);

  Xin_size = Min * cdoubleSize;
  Xin_ = (cdouble *)RTAlloc(mWorld, Xin_size);

  Xout_size = Mout * cdoubleSize;
  Xout_ = (cdouble *)RTAlloc(mWorld, Xout_size);

  Xout_mem_size = cdoubleSize * Mout;
  Xout_mem_ = (cdouble *)RTAlloc(mWorld, Xout_mem_size);

  Gin_size = cdoubleSize * Min;
  Gin_ = (cdouble *)RTAlloc(mWorld, Gin_size);

  Gout_size = cdoubleSize * Mout;
  Gout_ = (cdouble *)RTAlloc(mWorld, Mout * cdoubleSize);

  /* set_delay_size(ns); */

  imem_ = 0;
  ns_ = ns;
  clear();
}

/* void BBD_Line::set_delay_size(unsigned ns) { */
/*   mem_.clear(); */
/*   mem_.resize(ns); */
/*   imem_ = 0; */
/*   ns_ = ns; */
/* } */

void BBD_Line::clear() {
  memset(mem_, 0.0, mem_size);

  imem_ = 0;
  pclk_ = 0;
  ptick_ = 0;
  ybbd_old_ = 0;
  unsigned Min = fin_->M;
  unsigned Mout = fout_->M;

  memset(Xin_, 0.0, Xin_size);
  memset(Xout_, 0.0, Xout_size);
  memset(Xout_mem_, 0.0, Xout_mem_size);
}

void BBD_Line::process(unsigned n, const float *input, float *output,
                       const float *clock) {
  unsigned ns = ns_;
  float *mem = mem_;
  unsigned imem = imem_;
  double pclk = pclk_;
  unsigned ptick = ptick_;
  double ybbd_old = ybbd_old_;

  const BBD_Filter_Coef &fin = *fin_, &fout = *fout_;
  unsigned Min = fin.M, Mout = fout.M;
  cdouble *Xin = Xin_, *Xout = Xout_;
  cdouble *Xout_mem = Xout_mem_;
  cdouble *Gin = Gin_, *Gout = Gout_;
  const cdouble *Pin = fin.P.get(), *Pout = fout.P.get();

  for (unsigned i = 0; i < n; ++i) {
    double fclk = clock[i];

    for (unsigned m = 0; m < Mout; ++m)
      Xout[m] = 0;

    if (fclk > 0) {
      double pclk_old = pclk;
      pclk += fclk;
      unsigned tick_count = (unsigned)pclk;
      pclk -= tick_count;
      for (unsigned tick = 0; tick < tick_count; ++tick) {
        double d = (1 - pclk_old + tick) * (1 / fclk);
        d -= (unsigned)d;
        if ((ptick & 1) == 0) {
          fin.interpolate_G(d, Gin);
          cdouble s = 0;
          for (unsigned m = 0; m < Min; ++m)
            s += Gin[m] * Xin[m];
          mem[imem] = s.real();
          imem = ((imem + 1) < ns) ? (imem + 1) : 0;
        } else {
          fout.interpolate_G(d, Gout);
          double ybbd = mem[imem];
          double delta = ybbd - ybbd_old;
          ybbd_old = ybbd;
          for (unsigned m = 0; m < Mout; ++m)
            Xout[m] += Gout[m] * delta;
        }
        ++ptick;
      }
    }

    for (unsigned m = 0; m < Min; ++m)
      Xin[m] = Pin[m] * Xin[m] + cdouble(input[i]);

    cdouble y = fout.H * ybbd_old;
    for (unsigned m = 0; m < Mout; ++m) {
      cdouble xout = Pout[m] * Xout_mem[m] + Xout[m];
      Xout_mem[m] = xout;
      y += xout;
    }

    output[i] = y.real();
  }

  imem_ = imem;
  pclk_ = pclk;
  ptick_ = ptick;
  ybbd_old_ = ybbd_old;
}

void BBD_Line::process(unsigned n, float *inout, const float *clock) {
  process(n, inout, inout, clock);
}

float BBD_Line::process_single(float input, float fclk /*clock*/) {
  unsigned ns = ns_;
  float *mem = mem_;
  unsigned imem = imem_;
  double pclk = pclk_;
  unsigned ptick = ptick_;
  double ybbd_old = ybbd_old_;

  const BBD_Filter_Coef &fin = *fin_, &fout = *fout_;
  unsigned Min = fin.M, Mout = fout.M;
  cdouble *Xin = Xin_, *Xout = Xout_;
  cdouble *Xout_mem = Xout_mem_;
  cdouble *Gin = Gin_, *Gout = Gout_;
  const cdouble *Pin = fin.P.get(), *Pout = fout.P.get();

  for (unsigned m = 0; m < Mout; ++m)
    Xout[m] = 0;

  if (fclk > 0) {
    double pclk_old = pclk;
    pclk += fclk;
    unsigned tick_count = (unsigned)pclk;
    pclk -= tick_count;
    for (unsigned tick = 0; tick < tick_count; ++tick) {
      double d = (1 - pclk_old + tick) * (1 / fclk);
      d -= (unsigned)d;
      if ((ptick & 1) == 0) {
        fin.interpolate_G(d, Gin);
        cdouble s = 0;
        for (unsigned m = 0; m < Min; ++m)
          s += Gin[m] * Xin[m];
        mem[imem] = s.real();
        imem = ((imem + 1) < ns) ? (imem + 1) : 0;
      } else {
        /* std::cout << "tick" << std::endl; */

        fout.interpolate_G(d, Gout);
        double ybbd = mem[imem];
        double delta = ybbd - ybbd_old;
        ybbd_old = ybbd;
        for (unsigned m = 0; m < Mout; ++m)
          Xout[m] += Gout[m] * delta;
      }
      ++ptick;
    }
  }

  for (unsigned m = 0; m < Min; ++m)
    Xin[m] = Pin[m] * Xin[m] + cdouble(input);

  cdouble y = fout.H * ybbd_old;
  for (unsigned m = 0; m < Mout; ++m) {
    cdouble xout = Pout[m] * Xout_mem[m] + Xout[m];
    Xout_mem[m] = xout;
    y += xout;
  }

  float output = y.real();

  imem_ = imem;
  pclk_ = pclk;
  ptick_ = ptick;
  ybbd_old_ = ybbd_old;

  return output;
}
