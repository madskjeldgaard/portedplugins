AnalogPhaser : UGen {

	// skew -1.0 to 1.0
	// fb 0.0 to 0.95f
	// mod 0.0 1.0
	// stages 1.0 to 50.0
	*ar { |input, lfoinput, skew=0, feedback=0.25, modulation=0.5, stages=8|
		^this.multiNew('audio', input, lfoinput, skew, feedback, modulation, stages);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

