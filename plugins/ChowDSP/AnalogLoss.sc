AnalogLoss : UGen {
	*ar { |input, gap=0.5, thick=0.5, space=0.5, speed=1|
		^this.multiNew('audio', input, gap, thick, space, speed);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

