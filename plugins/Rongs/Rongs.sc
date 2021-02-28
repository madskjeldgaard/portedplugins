Rongs : MultiOutUGen {
	*ar { |trigger=0, sustain=1, f0=0.5, structure=0.5, brightness=0.5, damping=0.5, accent=0.5|
		^this.multiNew('audio', trigger, sustain, f0, structure, brightness, damping, accent);
	}

	init { arg ... theInputs;
		var numChannelsOut = 2;
		inputs = theInputs;
		^this.initOutputs(numChannelsOut, rate)
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

