Werner : UGen {
	*ar { |input, freq=0.5, damp=0.5, feedback=0.5, drive=0, oversample=1|
		/* TODO */
		^this.multiNew('audio', input, freq, damp, feedback, drive, oversample);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

