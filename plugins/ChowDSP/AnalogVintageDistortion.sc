AnalogVintageDistortion : UGen {
	*ar { |input, drivegain=0.5, bias=0, lowgain=0.1, highgain=0.1, shelvingfreq=600, oversample=0|
		^this.multiNew('audio', input, drivegain, bias, lowgain, highgain, shelvingfreq, oversample);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

