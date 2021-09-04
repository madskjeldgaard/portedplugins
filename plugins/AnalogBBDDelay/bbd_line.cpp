#include "bbd_line.h"
#include <algorithm>
#include <cassert>
#include <iostream>

static unsigned interp_size = 128;

void BBD_Line::setup(double fs, unsigned ns, const BBD_Filter_Spec &fsin,
                     const BBD_Filter_Spec &fsout) {
  mem_.reserve(8192);

  const BBD_Filter_Coef &fin =
      BBD::compute_filter_cached(fs, interp_size, fsin);
  const BBD_Filter_Coef &fout =
      BBD::compute_filter_cached(fs, interp_size, fsout);
  fin_ = &fin;
  fout_ = &fout;

  unsigned Min = fin.M;
  unsigned Mout = fout.M;
  /* Xin_ = RTAlloc(); */
  Xin_.reset(new cdouble[Min]);
  Xout_.reset(new cdouble[Mout]);
  Xout_mem_.reset(new cdouble[Mout]);
  Gin_.reset(new cdouble[Min]);
  Gout_.reset(new cdouble[Mout]);

  set_delay_size(ns);
  clear();
}

void BBD_Line::set_delay_size(unsigned ns) {
  mem_.clear();
  mem_.resize(ns);
  imem_ = 0;
  ns_ = ns;
}

void BBD_Line::clear() {
  std::fill(mem_.begin(), mem_.end(), 0);
  imem_ = 0;
  pclk_ = 0;
  ptick_ = 0;
  ybbd_old_ = 0;
  unsigned Min = fin_->M;
  unsigned Mout = fout_->M;
  std::fill(&Xin_[0], &Xin_[Min], 0);
  std::fill(&Xout_[0], &Xout_[Mout], 0);
  std::fill(&Xout_mem_[0], &Xout_mem_[Mout], 0);
}

void BBD_Line::process(unsigned n, const float *input, float *output,
                       const float *clock) {
  unsigned ns = ns_;
  float *mem = mem_.data();
  unsigned imem = imem_;
  double pclk = pclk_;
  unsigned ptick = ptick_;
  double ybbd_old = ybbd_old_;

  const BBD_Filter_Coef &fin = *fin_, &fout = *fout_;
  unsigned Min = fin.M, Mout = fout.M;
  cdouble *Xin = Xin_.get(), *Xout = Xout_.get();
  cdouble *Xout_mem = Xout_mem_.get();
  cdouble *Gin = Gin_.get(), *Gout = Gout_.get();
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
  float *mem = mem_.data();
  unsigned imem = imem_;
  double pclk = pclk_;
  unsigned ptick = ptick_;
  double ybbd_old = ybbd_old_;

  const BBD_Filter_Coef &fin = *fin_, &fout = *fout_;
  unsigned Min = fin.M, Mout = fout.M;
  cdouble *Xin = Xin_.get(), *Xout = Xout_.get();
  cdouble *Xout_mem = Xout_mem_.get();
  cdouble *Gin = Gin_.get(), *Gout = Gout_.get();
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
