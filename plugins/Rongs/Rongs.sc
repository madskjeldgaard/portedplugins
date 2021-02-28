Rongs : UGen {
	*ar { |trigger=0, sustain=1, f0=0.5, structure=0.5, brightness=0.5, damping=0.5, accent=0.5|
		^this.multiNew('audio', trigger, sustain, f0, structure, brightness, damping, accent);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

