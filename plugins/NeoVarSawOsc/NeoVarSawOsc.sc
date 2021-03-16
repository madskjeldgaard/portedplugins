NeoVarSawOsc : UGen {
	*ar { |freq=100, pw=0.5, waveshape=0.5|
		^this.multiNew('audio', freq, pw, waveshape);
	}

	*kr { |freq=100, pw=0.5, waveshape=0.5|
		^this.multiNew('control', freq, pw, waveshape);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

