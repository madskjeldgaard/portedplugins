VadimFilter : UGen {
	*ar { |input, freq=500, resonance=1.0|
		^this.multiNew('audio', input, freq, resonance);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
