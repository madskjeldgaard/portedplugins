AnalogWaveFolder : UGen {
	*ar { |freq=100, amp=1|
		/* TODO */
		^this.multiNew('audio', freq, amp);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

