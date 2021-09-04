// AnalogBBDDelay.cpp
// Mads Kjeldgaard (mail@madskjeldgaard.dk)

#include "AnalogBBDDelay.hpp"
#include "SC_PlugIn.hpp"
#include <iostream>

InterfaceTable *ft;

namespace AnalogBBDDelay {

namespace j60 {
static constexpr unsigned M_in = 5;
static constexpr cdouble R_in[M_in] = {{251589, 0},
                                       {-130428, -4165},
                                       {-130428, 4165},
                                       {4634, -22873},
                                       {4634, 22873}};
static constexpr cdouble P_in[M_in] = {{-46580, 0},
                                       {-55482, 25082},
                                       {-55482, -25082},
                                       {-26292, -59437},
                                       {-26292, 59437}};
static constexpr unsigned M_out = 5;
static constexpr cdouble R_out[M_out] = {{5092, 0},
                                         {11256, -99566},
                                         {11256, 99566},
                                         {-13802, -24606},
                                         {-13802, 24606}};
static constexpr cdouble P_out[M_out] = {{-176261, 0},
                                         {-51468, 21437},
                                         {-51468, -21437},
                                         {-26276, -59699},
                                         {-26276, 59699}};
} // namespace j60

AnalogBBDDelay::AnalogBBDDelay() {
  const float samplerate = sampleRate();

  m_delaytime_past = in0(DelayTime);
  m_number_of_stages = in0(NumStages);
  m_feedbackamount_past = in0(FeedbackAmount);

  // Juno 60 filter setup
  const BBD_Filter_Spec bbd_fin_j60 = {BBD_Filter_Kind::Input, j60::M_in,
                                       j60::R_in, j60::P_in};
  const BBD_Filter_Spec bbd_fout_j60 = {BBD_Filter_Kind::Output, j60::M_out,
                                        j60::R_out, j60::P_out};

  /* auto clockbufsize = fullBufferSize(); */
  /* this->clockbuf = (float *)RTAlloc(this->mWorld, clockbufsize *
   * sizeof(float)); */
  bbd_delay.setup(this->mWorld, samplerate, m_number_of_stages, bbd_fin_j60, bbd_fout_j60);
  bbd_delay.clear();

  mCalcFunc = make_calc_function<AnalogBBDDelay, &AnalogBBDDelay::next>();
  next(1);
}

AnalogBBDDelay::~AnalogBBDDelay() {
	bbd_delay.free(mWorld);
}

void AnalogBBDDelay::next(int nSamples) {
  const float *input = in(Input);
  float *outbuf = out(Out1);

  SlopeSignal<float> slopedFeedbackAmount =
      makeSlope(in0(FeedbackAmount), m_feedbackamount_past);

  const auto fSampleTime = (1.0f / sampleRate());

  SlopeSignal<float> slopedDelayTime =
      makeSlope(in0(DelayTime), m_delaytime_past);

  for (size_t i = 0; i < nSamples; i++) {

    const float feedbackamount = slopedFeedbackAmount.consume();
    const float delaytime = slopedDelayTime.consume();

    auto clockRate =
        bbd_delay.hz_rate_for_delay(delaytime, m_number_of_stages) *
        fSampleTime;

    const auto out = bbd_delay.process_single(
        input[i] + (feedbackamount * m_feedback), clockRate);
    m_feedback = out;
    outbuf[i] = out;
  }

  m_feedbackamount_past = slopedFeedbackAmount.value;
  m_delaytime_past = slopedDelayTime.value;
}

void AnalogBBDDelay::clear(int nSamples) { ClearUnitOutputs(this, nSamples); }

} // namespace AnalogBBDDelay

PluginLoad(AnalogBBDDelayUGens) {
  // Plugin magic
  ft = inTable;
  registerUnit<AnalogBBDDelay::AnalogBBDDelay>(ft, "AnalogBBDDelay", false);
}
