VadimFilter : UGen {
	*ar { |input, freq=500, resonance=1.0, type=0|
		^this.multiNew('audio', input, freq, resonance, type);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
