AnalogChew : UGen {
	*ar { |input, depth=0.5, freq=0.5, variance=0.5|
		^this.multiNew('audio', input, depth, freq, variance);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

