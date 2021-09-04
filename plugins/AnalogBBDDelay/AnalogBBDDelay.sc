AnalogBBDDelay : UGen {
	*ar { |input, delaytime=0.1|
		^this.multiNew('audio', input, delaytime);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
