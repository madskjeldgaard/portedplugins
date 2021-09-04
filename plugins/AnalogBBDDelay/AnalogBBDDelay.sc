AnalogBBDDelay : UGen {
	*ar { |input, delaytime=(1e-3), feedback=0, numStages=182|
		^this.multiNew('audio', input, delaytime, feedback, numStages);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
