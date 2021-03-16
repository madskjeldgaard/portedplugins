ZOsc : UGen {
	*ar { |freq=100, formantfreq=91, shape=0.5, mode=0.5|
		^this.multiNew('audio', freq, formantfreq, shape, mode);
	}

	*kr { |freq=100, formantfreq=91, shape=0.5, mode=0.5|
		^this.multiNew('control', freq, formantfreq, shape, mode);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

