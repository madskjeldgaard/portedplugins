Resonator : UGen {
	*ar { |input=0, freq=100, position=0.001, resolution=24, structure=0.5, brightness=0.5, damping=0.5|
		/* TODO */
		^this.multiNew('audio', input, freq, position, resolution, structure, brightness, damping);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

