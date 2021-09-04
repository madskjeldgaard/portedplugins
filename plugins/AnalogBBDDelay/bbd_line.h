#pragma once
#include "bbd_filter.h"
#include <algorithm>
#include <complex>
#include <memory>
#include <vector>

#include "SC_InterfaceTable.h"
#include "SC_World.h"
typedef std::complex<double> cdouble;

class BBD_Line {
public:
  /**
   * Initialize a delay line with the specified parameters. (non-RT)
   * @param fs audio sampling rate
   * @param ns number of stages / length of the virtual capacitor array
   * @param fsin analog specification of the input filter
   * @param fsout analog specification of the output filter
   */
  void setup(World * mWorld, double fs, unsigned ns, const BBD_Filter_Spec &fsin,
             const BBD_Filter_Spec &fsout);

	void free(World * mWorld);

  /**
   * Change the number of stages. (RT?)
   * @note It guarantees not to reallocate the buffer for \f$ns \leq 8192\f$.
   * @param ns number of stages / length of the virtual capacitor array
   */
  void set_delay_size(unsigned ns);

  /**
   * Reinitialize all the internal state to zero. (RT)
   */
  void clear();

  /**
   * Process a block of audio signal. (RT)
   * @note The clock input is defined as \f$F_{clk}/F_{s}\f$, where
   *       \f$F_{clk}\f$ is the desired BBD instantaneous clock rate. It is
   *       valid to have \f$F_{clk}>F_{s}/2\f$.
   * @param n number of frames to process
   * @param input input buffer of size @p n
   * @param output output buffer of size @p n
   * @param clock clock input buffer of size @p n
   */
  void process(unsigned n, const float *input, float *output,
               const float *clock);

  /**
   * Process a block of audio signal in place. (RT)
   * @param n number of frames to process
   * @param inout input/output buffer of size @p n
   * @param clock clock input buffer of size @p n
   */
  void process(unsigned n, float *inout, const float *clock);

  /**
   * Process a single audio frame. (RT)
   * @param input input sample
   * @param clock clock sample
   * @return output sample
   */
  float process_single(float input, float clock);

  /**
   * Get the discretization of the input filter. (RT)
   * @return digital filter model
   */
  const BBD_Filter_Coef &filter_in() const noexcept { return *fin_; }

  /**
   * Get the discretization of the output filter. (RT)
   * @return digital filter model
   */
  const BBD_Filter_Coef &filter_out() const noexcept { return *fout_; }

  /**
   * Determine the BBD clock rate \f$F_{clk}\f$ which obtains a given delay.
   * (RT)
   * @param delay delay in seconds
   * @param ns number of stages / length of the virtual capacitor array
   * @return BBD clock rate in Hz
   */
  static inline double hz_rate_for_delay(double delay, unsigned ns) {
    return 2 * ns / delay;
  }

  /**
   * Determine the delay obtained for a given BBD clock rate \f$F_{clk}\f$. (RT)
   * @param rate BBD clock rate in Hz
   * @param ns number of stages / length of the virtual capacitor array
   * @return delay in seconds
   */
  static inline double delay_for_hz_rate(double rate, unsigned ns) {
    return 2 * ns / rate;
  }

private:
  unsigned ns_;            // delay size
  std::vector<float> mem_; // delay memory
  unsigned imem_;          // delay memory index
  double pclk_;            // clock phase
  unsigned ptick_;         // clock tick counter
  double ybbd_old_;
  const BBD_Filter_Coef *fin_;
  const BBD_Filter_Coef *fout_;
  cdouble* Xin_;
  cdouble* Xout_;
  cdouble* Xout_mem_; // sample memory of output filter
  cdouble* Gin_;
  cdouble* Gout_;
};
