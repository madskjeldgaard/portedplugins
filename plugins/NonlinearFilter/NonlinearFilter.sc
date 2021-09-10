NonlinearFilter : UGen {
	*ar { |input, freq=500, q=0.5, gain=1, shape=5, saturation=3|
		^this.multiNew('audio', input, freq, q, gain, shape, saturation);
	}

    *kr { |input, freq=500, q=0.5, gain=1, shape=5, saturation=3|
		^this.multiNew('control', input, freq, q, gain, shape, saturation);
    }

	checkInputs {
		^this.checkValidInputs;
	}
}
