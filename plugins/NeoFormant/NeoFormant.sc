NeoFormant : UGen {
	*ar { |formantfreq=100, carrierfreq=200, phaseshift=0.5|
		^this.multiNew('audio', formantfreq, carrierfreq, phaseshift);
	}

	*kr { |formantfreq=100, carrierfreq=200, phaseshift=0.5|
		^this.multiNew('control', formantfreq, carrierfreq, phaseshift);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

