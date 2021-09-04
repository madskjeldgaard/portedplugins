AnalogBBDDelay : UGen {
	*ar { |input, delaytime=0.1, numStages=182|
		^this.multiNew('audio', input, delaytime, numStages);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
