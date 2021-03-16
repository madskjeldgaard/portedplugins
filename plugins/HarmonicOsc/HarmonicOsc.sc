HarmonicOsc : UGen {
	*ar { |freq=100|
		^this.multiNew('audio', freq);
	}

	*kr { |freq=100|
		^this.multiNew('audio', freq);
	}

	checkInputs {
		^this.checkValidInputs;
	}
}

