Rongs : UGen {
	*ar { |trigger=0, sustain=1, f0=0.01, structure=0.5, brightness=0.5, damping=0.75, accent=0.9, stretch=0.5, position=0.15, loss=0.15|
		^this.multiNew('audio', trigger, sustain, f0, structure, brightness, damping, accent, stretch, position, loss);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

