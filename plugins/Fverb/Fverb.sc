
Fverb : MultiOutUGen {

    *ar{|in0, in1,predelay(0), input_amount(100), input_lowpass_cutoff(10000), input_highpass_cutoff(100), input_diffusion_1(75), input_diffusion_2(62.5), tail_density(70), decay(50), damping(5500), modulator_frequency(1), modulator_depth(0.5)|
      ^this.multiNew('audio', in0, in1,predelay, input_amount, input_lowpass_cutoff, input_highpass_cutoff, input_diffusion_1, input_diffusion_2, tail_density, decay, damping, modulator_frequency, modulator_depth)
    }

    *kr{|in0, in1,predelay(0), input_amount(100), input_lowpass_cutoff(10000), input_highpass_cutoff(100), input_diffusion_1(75), input_diffusion_2(62.5), tail_density(70), decay(50), damping(5500), modulator_frequency(1), modulator_depth(0.5)|
      ^this.multiNew('control', in0, in1,predelay, input_amount, input_lowpass_cutoff, input_highpass_cutoff, input_diffusion_1, input_diffusion_2, tail_density, decay, damping, modulator_frequency, modulator_depth)
    }

    name { ^"Fverb" }

    info { ^"Generated with Faust" }
    

checkInputs {
    if (rate == 'audio', {
      2.do({|i|
        if (inputs.at(i).rate != 'audio', {
          ^(" input at index " + i + "(" + inputs.at(i) +
            ") is not audio rate");
        });
      });
    });
    ^this.checkValidInputs
  }


    

init { | ... theInputs |
      inputs = theInputs
      ^this.initOutputs(2, rate)
  }

    
}
