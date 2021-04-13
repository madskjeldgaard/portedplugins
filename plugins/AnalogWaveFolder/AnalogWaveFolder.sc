AnalogWaveFolder : UGen {
	*ar { |input, freq=100, amp=1|
		/* TODO */
		^this.multiNew('audio', input, freq, amp);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

