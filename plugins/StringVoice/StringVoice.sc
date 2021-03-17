StringVoice : UGen {
	*ar { |trig=0, infsustain=0, freq=100, accent=0.5, structure=0.5, brightness=0.5, damping=0.5|
		^this.multiNew('audio', trig, infsustain, freq, accent, structure, brightness, damping);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

