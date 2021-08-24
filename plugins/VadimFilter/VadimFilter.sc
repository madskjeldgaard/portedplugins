VadimFilter : UGen {
	*ar { |input, freq=500, resonance=0.25, type=0|
		^this.multiNew('audio', input, freq, resonance*4.0, type);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
