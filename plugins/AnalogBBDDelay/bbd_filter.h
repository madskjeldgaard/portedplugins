#pragma once
#include <memory>
#include <complex>
typedef std::complex<double> cdouble;

enum class BBD_Filter_Kind {
    Input,
    Output,
};

/*
  Analog specifications of BBD filters, input and output.
  M=order R=numerator P=denominator
  Analog transfer: H(s)=sum(m:1→M) (R[m]/(s-P[m]))
*/

struct BBD_Filter_Spec {
    BBD_Filter_Kind kind;
    unsigned M;
    const cdouble *R;/*[M]*/
    const cdouble *P;/*[M]*/
    //
    cdouble transfer(double frequency) const noexcept;
};

/*
  Discretized matrix of filters coefficients.
  M=order, N=interpolation steps, H=feedback factor
*/
struct BBD_Filter_Coef {
    unsigned M;
    unsigned N;
    std::unique_ptr<cdouble[]> G;/*[M*N]*/
    std::unique_ptr<cdouble[]> P;/*[M]*/
    double H;
    //
    void interpolate_G(double d, cdouble *g/*[M]*/) const noexcept;
};

namespace BBD {
BBD_Filter_Coef compute_filter(float fs, unsigned steps, const BBD_Filter_Spec &spec);
const BBD_Filter_Coef &compute_filter_cached(float fs, unsigned steps, const BBD_Filter_Spec &spec);
void clear_filter_cache();
} // namespace BBD

/*
  The model of BBD input and output filters from Juno 60.
*/
extern const BBD_Filter_Spec bbd_fin_j60;
extern const BBD_Filter_Spec bbd_fout_j60;
