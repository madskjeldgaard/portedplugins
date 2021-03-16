BLOsc : UGen {
	*ar { |freq=100, pulsewidth=0.5, waveform=0|
		^this.multiNew('audio', freq, pulsewidth, waveform);
	}

	*kr { |freq=100, pulsewidth=0.5, waveform=0|
		^this.multiNew('control', freq, pulsewidth, waveform);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

