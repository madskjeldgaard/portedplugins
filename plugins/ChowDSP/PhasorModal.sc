PhasorModal : UGen {
	*ar { |input, freq=100, decay=0.25, damp=1, amp=0.5, phase=0|
		/* TODO */
		^this.multiNew('audio', input, freq, decay, damp, amp, phase);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

