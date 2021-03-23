AnalogPhaserMod : UGen {
	*ar { |input, skew=0, modulation=0.5, stages=8|
		^this.multiNew('audio', input, skew=0, modulation=0.5, stages=8);
	}

	*kr { |input, skew=0, modulation=0.5, stages=8|
		^this.multiNew('control', input, skew=0, modulation=0.5, stages=8);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

