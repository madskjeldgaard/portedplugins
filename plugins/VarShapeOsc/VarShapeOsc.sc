VarShapeOsc : UGen {
	*ar { |freq=100, pw=0.5, waveshape=0.5, sync=1, syncfreq=105|
		^this.multiNew('audio', freq, pw, waveshape, sync, syncfreq);
	}

	*kr { |freq=100, pw=0.5, waveshape=0.5, sync=1, syncfreq=105|
		^this.multiNew('control', freq, pw, waveshape, sync, syncfreq);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

